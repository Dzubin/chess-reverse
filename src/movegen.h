#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "chess.h"

/* Fills moves[] with every LEGAL move for the side to move and returns
   the count (0 = checkmate or stalemate; distinguish with in_check()). */
int generate_moves(board_t *b, move_t moves[MAX_MOVES]);

int  square_is_attacked(const board_t *b, square_t sq, int by_color);
int  in_check(const board_t *b, int color);

/* m must have from/to (and, for castling, is_castle/rook_from/rook_to)
   already set; make_move fills in the rest for unmake_move to use. */
void make_move(board_t *b, move_t *m);
void unmake_move(board_t *b, const move_t *m);

#endif
