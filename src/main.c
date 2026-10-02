#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chess.h"
#include "board.h"
#include "movegen.h"
#include "search.h"
#include "display.h"

static void print_computer_move(const move_t *m);

int main(void)
{
    board_t board;
    int human_color, depth;
    int player_quit = 0;

    printf("CHESS - a C rewrite of a 1980s DOS chess program\n\n");

    /* time(NULL) alone has 1-second resolution, so quick successive runs
       (e.g. restarting right after resigning) would otherwise seed
       identically and repeat the same "random" beginner moves. clock()
       doesn't help - it measures CPU time since process start, which is
       ~0 this early - so mix in the address of a local instead; stack
       addresses vary between process runs on any OS with ASLR. */
    srand((unsigned int)time(NULL) ^ (unsigned int)(size_t)&board);

    board_init(&board);
    display_board(&board);

    human_color = display_ask_color();
    depth = display_ask_depth();

    for (;;) {
        move_t moves[MAX_MOVES];
        int count = generate_moves(&board, moves);

        if (count == 0) {
            if (in_check(&board, board.side_to_move)) {
                display_message(board.side_to_move == human_color ? "MATE - I WIN" : "MATE - YOU WIN");
            } else {
                display_message("STALEMATE");
            }
            break;
        }

        if (board.side_to_move == human_color) {
            move_t chosen;
            if (!display_get_human_move(&board, &chosen)) {
                display_message("GOODBYE");
                player_quit = 1;
                break;
            }
            make_move(&board, &chosen);
        } else {
            move_t chosen = choose_computer_move(&board, depth);
            display_computer_thinking();
            print_computer_move(&chosen);
            make_move(&board, &chosen);
        }

        display_board(&board);
        if (in_check(&board, board.side_to_move)) {
            display_message("CHECK");
        }
    }

    display_finish(player_quit);
    return 0;
}

static void print_computer_move(const move_t *m)
{
    printf("COMPUTER: %c%d-%c%d\n",
           'a' + SQ_FILE(m->from), SQ_RANK(m->from) + 1,
           'a' + SQ_FILE(m->to), SQ_RANK(m->to) + 1);
}
