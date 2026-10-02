#ifndef DISPLAY_H
#define DISPLAY_H

#include "chess.h"

/* Thin, portable console I/O layer - the only part of the program that
   would need to change to run somewhere other than a stdio terminal
   (the original ran this same job through direct CGA video-memory writes
   and raw DOS/BIOS keyboard calls; here it's plain ANSI-free stdio so it
   builds and runs anywhere a C compiler does). */

void display_board(const board_t *b);
void display_message(const char *msg);

int display_ask_color(void);          /* returns WHITE or BLACK for the human */
int display_ask_depth(void);          /* returns SEARCH_DEPTH_MIN..SEARCH_DEPTH_MAX */

/* Prompts for and reads one legal move for the side to move, retrying on
   bad input. Returns 1 with *out_move filled in, or 0 if the player typed
   "quit". */
int display_get_human_move(board_t *b, move_t *out_move);

/* Called once the computer has picked its move but before it's shown or
   applied - a hook for a platform that wants to add a short artificial
   pause so the computer's reply doesn't feel instant (search at low
   depths genuinely is near-instant). Desktop backends no-op; PicoCalc
   adds a random delay - see display_picocalc.c. */
void display_computer_thinking(void);

/* Called once when the game is over (mate, stalemate, or the player quit),
   just before main() returns. player_quit is 1 if the player chose to quit.
   Desktop backends no-op and main() simply returns; PicoCalc leaves for the
   UF2 Loader's menu (after a key press when the game ended on its own, so
   the result can be read) - see display_picocalc.c. */
void display_finish(int player_quit);

#endif
