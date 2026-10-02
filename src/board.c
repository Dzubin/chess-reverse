#include "board.h"

static const piece_t back_rank[8] = {
    PIECE_ROOK, PIECE_KNIGHT, PIECE_BISHOP, PIECE_QUEEN,
    PIECE_KING, PIECE_BISHOP, PIECE_KNIGHT, PIECE_ROOK
};

void board_init(board_t *b)
{
    int rank, file, i;

    for (i = 0; i < BOARD_SIZE; i++) {
        b->square[i] = PIECE_OFFBOARD;
    }
    for (rank = 0; rank < 8; rank++) {
        for (file = 0; file < 8; file++) {
            b->square[SQ(rank, file)] = PIECE_NONE;
        }
    }

    for (file = 0; file < 8; file++) {
        b->square[SQ(0, file)] = back_rank[file];
        b->square[SQ(1, file)] = PIECE_PAWN;
        b->square[SQ(7, file)] = (piece_t)(back_rank[file] | PIECE_COLOR_BIT);
        b->square[SQ(6, file)] = (piece_t)(PIECE_PAWN | PIECE_COLOR_BIT);
    }

    b->king_square[WHITE]  = SQ(0, 4);
    b->king_square[BLACK]  = SQ(7, 4);
    b->queen_square[WHITE] = SQ(0, 3);
    b->queen_square[BLACK] = SQ(7, 3);

    b->side_to_move = WHITE;
    b->en_passant_square = 0;
}
