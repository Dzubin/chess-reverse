#include "eval.h"

/* Mirrors movegen.c's direction tables - see the comment there. Kept as a
   separate copy rather than a shared header/extern to avoid coupling eval
   to movegen's internals for two 8-byte constant arrays. */
static const signed char slider8[8] = { 9, 11, -11, -9, 10, -10, 1, -1 };
static const signed char knight8[8] = { -21, -12, 8, 19, 21, 12, -8, -19 };

/* The original's FUN_1000_00f8: from a square, sweep the 8 sliding
   directions (stopping at the first piece or the edge) plus the 8 knight
   offsets (single probe each), and count pieces of each color found.
   Decoded from CHESS.EXE - see analysis.md "FUN_1000_00f8 CRACKED". The
   original runs this from EVERY square on the board (occupied or not);
   here it only runs from occupied squares, since evaluate() is called at
   every search leaf and a full 64-square sweep per call would be far
   more expensive for a difference this small. */
static void scan_exposure(const board_t *b, square_t from, int *white_count, int *black_count)
{
    int i;

    *white_count = 0;
    *black_count = 0;

    for (i = 0; i < 8; i++) {
        square_t t = from;
        for (;;) {
            piece_t p;
            t = (square_t)(t + slider8[i]);
            p = b->square[t];
            if (p == PIECE_OFFBOARD) break;
            if (p != PIECE_NONE) {
                if (PIECE_COLOR(p) == WHITE) (*white_count)++; else (*black_count)++;
                break;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        square_t t = (square_t)(from + knight8[i]);
        piece_t p = b->square[t];
        if (p != PIECE_OFFBOARD && p != PIECE_NONE) {
            if (PIECE_COLOR(p) == WHITE) (*white_count)++; else (*black_count)++;
        }
    }
}

int evaluate(const board_t *b)
{
    int score = 0;
    int sq;

    for (sq = 0; sq < BOARD_SIZE; sq++) {
        piece_t p = b->square[sq];
        int type, sign;

        if (p == PIECE_NONE || p == PIECE_OFFBOARD) continue;

        type = PIECE_TYPE(p);
        sign = (PIECE_COLOR(p) == WHITE) ? 1 : -1;

        switch (type) {
        case PIECE_PAWN:
            score += sign * VALUE_PAWN;
            break;
        case PIECE_KNIGHT:
            score += sign * VALUE_KNIGHT;
            if (!(p & PIECE_MOVED)) score -= sign * UNDEVELOPED_MINOR_PENALTY;
            break;
        case PIECE_BISHOP:
            score += sign * VALUE_BISHOP;
            if (!(p & PIECE_MOVED)) score -= sign * UNDEVELOPED_MINOR_PENALTY;
            break;
        case PIECE_ROOK:
            score += sign * VALUE_ROOK;
            if (p & PIECE_MOVED) score -= sign * EARLY_MAJOR_MOVE_PENALTY;
            break;
        case PIECE_QUEEN:
            score += sign * VALUE_QUEEN;
            if (p & PIECE_MOVED) score -= sign * EARLY_MAJOR_MOVE_PENALTY;
            break;
        case PIECE_KING:
            if (p & PIECE_CASTLED) {
                score += sign * CASTLED_BONUS;
            } else if (p & PIECE_MOVED) {
                score -= sign * EARLY_MAJOR_MOVE_PENALTY;
            }
            break;
        default:
            break;
        }

        {
            int white_seen, black_seen;
            scan_exposure(b, (square_t)sq, &white_seen, &black_seen);
            score += (white_seen - black_seen) * MOBILITY_WEIGHT;
        }
    }

    return score;
}
