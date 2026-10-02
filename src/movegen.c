#include "movegen.h"

/* Move-direction deltas on the 10-wide mailbox board, decoded from the
   original CHESS.EXE's own direction table (workspace offset 0x9c) and
   verified against every piece's real movement rules:
     slider8[0..3] = the 4 diagonals (bishop)
     slider8[4..7] = the 4 orthogonals (rook)
     slider8[0..7] = all 8 (queen, and king taking a single step)
   knight8 is the classic 8 L-jumps on a 10-wide board. */
static const signed char slider8[8] = { 9, 11, -11, -9, 10, -10, 1, -1 };
static const signed char knight8[8] = { -21, -12, 8, 19, 21, 12, -8, -19 };

static void add_move(move_t *list, int *count, square_t from, square_t to)
{
    move_t *m = &list[*count];
    m->from = from;
    m->to = to;
    m->is_castle = 0;
    m->rook_from = 0;
    m->rook_to = 0;
    (*count)++;
}

static void gen_pawn_moves(const board_t *b, square_t sq, move_t *list, int *count)
{
    int color = PIECE_COLOR(b->square[sq]);
    int forward = (color == WHITE) ? 10 : -10;
    int start_rank = (color == WHITE) ? 1 : 6;
    int rank = SQ_RANK(sq);
    square_t one = (square_t)(sq + forward);
    int i;

    if (b->square[one] == PIECE_NONE) {
        add_move(list, count, sq, one);
        if (rank == start_rank) {
            square_t two = (square_t)(sq + 2 * forward);
            if (b->square[two] == PIECE_NONE) {
                add_move(list, count, sq, two);
            }
        }
    }

    for (i = -1; i <= 1; i += 2) {
        square_t target = (square_t)(sq + forward + i);
        piece_t occ = b->square[target];
        if (occ == PIECE_OFFBOARD) continue;
        if (occ != PIECE_NONE && PIECE_COLOR(occ) != color) {
            add_move(list, count, sq, target);
        } else if (occ == PIECE_NONE && target == b->en_passant_square) {
            add_move(list, count, sq, target);
        }
    }
}

static void gen_ray_moves(const board_t *b, square_t sq, const signed char *deltas,
                           int ndelta, int slide, move_t *list, int *count)
{
    int color = PIECE_COLOR(b->square[sq]);
    int i;

    for (i = 0; i < ndelta; i++) {
        square_t t = sq;
        for (;;) {
            t = (square_t)(t + deltas[i]);
            piece_t occ = b->square[t];
            if (occ == PIECE_OFFBOARD) break;
            if (occ == PIECE_NONE) {
                add_move(list, count, sq, t);
            } else {
                if (PIECE_COLOR(occ) != color) {
                    add_move(list, count, sq, t);
                }
                break;
            }
            if (!slide) break;
        }
    }
}

static void gen_castle_moves(board_t *b, square_t sq, move_t *list, int *count)
{
    piece_t king = b->square[sq];
    int color = PIECE_COLOR(king);
    int rank = (color == WHITE) ? 0 : 7;
    int other = OTHER_COLOR(color);

    if (king & PIECE_MOVED) return;
    if (in_check(b, color)) return;

    {
        square_t rook_sq = SQ(rank, 7);
        piece_t rook = b->square[rook_sq];
        if (PIECE_TYPE(rook) == PIECE_ROOK && PIECE_COLOR(rook) == color && !(rook & PIECE_MOVED)) {
            square_t f = SQ(rank, 5), g = SQ(rank, 6);
            if (b->square[f] == PIECE_NONE && b->square[g] == PIECE_NONE &&
                !square_is_attacked(b, f, other) && !square_is_attacked(b, g, other)) {
                move_t *m = &list[*count];
                m->from = sq; m->to = g;
                m->is_castle = 1; m->rook_from = rook_sq; m->rook_to = f;
                (*count)++;
            }
        }
    }
    {
        square_t rook_sq = SQ(rank, 0);
        piece_t rook = b->square[rook_sq];
        if (PIECE_TYPE(rook) == PIECE_ROOK && PIECE_COLOR(rook) == color && !(rook & PIECE_MOVED)) {
            square_t d = SQ(rank, 3), c = SQ(rank, 2), knight_sq = SQ(rank, 1);
            if (b->square[d] == PIECE_NONE && b->square[c] == PIECE_NONE &&
                b->square[knight_sq] == PIECE_NONE &&
                !square_is_attacked(b, d, other) && !square_is_attacked(b, c, other)) {
                move_t *m = &list[*count];
                m->from = sq; m->to = c;
                m->is_castle = 1; m->rook_from = rook_sq; m->rook_to = d;
                (*count)++;
            }
        }
    }
}

static int generate_pseudo_moves(board_t *b, move_t list[MAX_MOVES])
{
    int count = 0;
    int sq;

    for (sq = 0; sq < BOARD_SIZE; sq++) {
        piece_t p = b->square[sq];
        if (p == PIECE_NONE || p == PIECE_OFFBOARD) continue;
        if (PIECE_COLOR(p) != b->side_to_move) continue;

        switch (PIECE_TYPE(p)) {
        case PIECE_PAWN:
            gen_pawn_moves(b, (square_t)sq, list, &count);
            break;
        case PIECE_KNIGHT:
            gen_ray_moves(b, (square_t)sq, knight8, 8, 0, list, &count);
            break;
        case PIECE_BISHOP:
            gen_ray_moves(b, (square_t)sq, slider8, 4, 1, list, &count);
            break;
        case PIECE_ROOK:
            gen_ray_moves(b, (square_t)sq, slider8 + 4, 4, 1, list, &count);
            break;
        case PIECE_QUEEN:
            gen_ray_moves(b, (square_t)sq, slider8, 8, 1, list, &count);
            break;
        case PIECE_KING:
            gen_ray_moves(b, (square_t)sq, slider8, 8, 0, list, &count);
            gen_castle_moves(b, (square_t)sq, list, &count);
            break;
        default:
            break;
        }
    }
    return count;
}

int generate_moves(board_t *b, move_t moves[MAX_MOVES])
{
    move_t pseudo[MAX_MOVES];
    int pseudo_count = generate_pseudo_moves(b, pseudo);
    int legal_count = 0;
    int mover_color = b->side_to_move;
    int i;

    for (i = 0; i < pseudo_count; i++) {
        move_t m = pseudo[i];
        make_move(b, &m);
        if (!in_check(b, mover_color)) {
            moves[legal_count++] = m;
        }
        unmake_move(b, &m);
    }
    return legal_count;
}

int square_is_attacked(const board_t *b, square_t sq, int by_color)
{
    int i;

    for (i = 0; i < 8; i++) {
        square_t t = (square_t)(sq + knight8[i]);
        piece_t p = b->square[t];
        if (p != PIECE_OFFBOARD && p != PIECE_NONE &&
            PIECE_COLOR(p) == by_color && PIECE_TYPE(p) == PIECE_KNIGHT) {
            return 1;
        }
    }
    for (i = 0; i < 8; i++) {
        square_t t = (square_t)(sq + slider8[i]);
        piece_t p = b->square[t];
        if (p != PIECE_OFFBOARD && p != PIECE_NONE &&
            PIECE_COLOR(p) == by_color && PIECE_TYPE(p) == PIECE_KING) {
            return 1;
        }
    }
    for (i = 0; i < 8; i++) {
        int is_diag = (i < 4);
        square_t t = sq;
        for (;;) {
            t = (square_t)(t + slider8[i]);
            piece_t p = b->square[t];
            if (p == PIECE_OFFBOARD) break;
            if (p != PIECE_NONE) {
                if (PIECE_COLOR(p) == by_color) {
                    int type = PIECE_TYPE(p);
                    if (type == PIECE_QUEEN ||
                        (is_diag && type == PIECE_BISHOP) ||
                        (!is_diag && type == PIECE_ROOK)) {
                        return 1;
                    }
                }
                break;
            }
        }
    }
    {
        int back = (by_color == WHITE) ? -10 : 10;
        int i2;
        for (i2 = -1; i2 <= 1; i2 += 2) {
            square_t t = (square_t)(sq + back + i2);
            piece_t p = b->square[t];
            if (p != PIECE_OFFBOARD && p != PIECE_NONE &&
                PIECE_COLOR(p) == by_color && PIECE_TYPE(p) == PIECE_PAWN) {
                return 1;
            }
        }
    }
    return 0;
}

int in_check(const board_t *b, int color)
{
    return square_is_attacked(b, b->king_square[color], OTHER_COLOR(color));
}

void make_move(board_t *b, move_t *m)
{
    piece_t moving = b->square[m->from];
    int color = PIECE_COLOR(moving);
    int type = PIECE_TYPE(moving);

    m->moved_piece = moving;
    m->prev_en_passant = b->en_passant_square;
    m->captured_piece = PIECE_NONE;
    m->captured_square = m->to;
    m->is_promotion = 0;

    if (type == PIECE_PAWN && b->square[m->to] == PIECE_NONE &&
        m->to == b->en_passant_square && SQ_FILE(m->to) != SQ_FILE(m->from)) {
        m->captured_square = (square_t)((color == WHITE) ? m->to - 10 : m->to + 10);
    }

    if (b->square[m->captured_square] != PIECE_NONE) {
        m->captured_piece = b->square[m->captured_square];
        b->square[m->captured_square] = PIECE_NONE;
        if (PIECE_TYPE(m->captured_piece) == PIECE_QUEEN) {
            b->queen_square[PIECE_COLOR(m->captured_piece)] = 0;
        }
    }

    b->square[m->from] = PIECE_NONE;

    if (type == PIECE_PAWN && (SQ_RANK(m->to) == 0 || SQ_RANK(m->to) == 7)) {
        m->is_promotion = 1;
        moving = (piece_t)(PIECE_QUEEN | (moving & PIECE_COLOR_BIT) | PIECE_MOVED);
    } else {
        moving = (piece_t)(moving | PIECE_MOVED);
    }
    b->square[m->to] = moving;

    if (type == PIECE_KING) {
        b->king_square[color] = m->to;
        if (m->is_castle) {
            moving = (piece_t)(moving | PIECE_CASTLED);
            b->square[m->to] = moving;
            b->square[m->rook_to] = (piece_t)(b->square[m->rook_from] | PIECE_MOVED);
            b->square[m->rook_from] = PIECE_NONE;
        }
    } else if (type == PIECE_QUEEN) {
        b->queen_square[color] = m->to;
    }

    b->en_passant_square = 0;
    if (type == PIECE_PAWN) {
        int from_rank = SQ_RANK(m->from);
        int to_rank = SQ_RANK(m->to);
        if (to_rank - from_rank == 2 || from_rank - to_rank == 2) {
            b->en_passant_square = (square_t)((m->from + m->to) / 2);
        }
    }

    b->side_to_move = OTHER_COLOR(color);
}

void unmake_move(board_t *b, const move_t *m)
{
    int color = PIECE_COLOR(m->moved_piece);

    b->square[m->from] = m->moved_piece;
    b->square[m->to] = PIECE_NONE;

    if (m->captured_piece != PIECE_NONE) {
        b->square[m->captured_square] = m->captured_piece;
        if (PIECE_TYPE(m->captured_piece) == PIECE_QUEEN) {
            b->queen_square[PIECE_COLOR(m->captured_piece)] = m->captured_square;
        }
    }

    if (PIECE_TYPE(m->moved_piece) == PIECE_KING) {
        b->king_square[color] = m->from;
        if (m->is_castle) {
            b->square[m->rook_from] = (piece_t)(b->square[m->rook_to] & (piece_t)~PIECE_MOVED);
            b->square[m->rook_to] = PIECE_NONE;
        }
    } else if (PIECE_TYPE(m->moved_piece) == PIECE_QUEEN) {
        b->queen_square[color] = m->from;
    }

    b->en_passant_square = m->prev_en_passant;
    b->side_to_move = color;
}
