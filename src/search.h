#ifndef SEARCH_H
#define SEARCH_H

#include "chess.h"

/* Caller must confirm generate_moves(b, ...) > 0 before calling - this
   does not itself detect checkmate/stalemate at the root. */
move_t search_best_move(board_t *b, int depth);

/* What the game loop should actually call: same precondition as
   search_best_move, but at depth == SEARCH_DEPTH_BEGINNER this mixes in
   random legal moves per BEGINNER_RANDOM_MOVE_PERCENT instead of always
   playing the search's choice. */
move_t choose_computer_move(board_t *b, int depth);

#endif
