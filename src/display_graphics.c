#include "display.h"
#include "board_render.h"
#include "movegen.h"
#include <stdio.h>
#include <string.h>

/* Alternate display.h backend: draws the board graphically (via
   board_render.c / gfx.h - see gfx_ppm.c for the desktop image-file
   backend, or swap in a real LCD backend on target hardware) instead of
   printing letters. Move prompts and text messages still go through
   stdio, same as display_console.c, since this machine has no PicoCalc
   keypad to read from - only display_board() actually differs. Link this
   file instead of display_console.c to get the graphical board (see the
   chess_gfx CMake target). */

void display_board(const board_t *b)
{
    board_render(b);
    printf("(board image updated - see chess_board.ppm)\n\n");
}

void display_message(const char *msg)
{
    printf("%s\n", msg);
}

int display_ask_color(void)
{
    char line[16];

    for (;;) {
        printf("WHICH COLOR DO YOU SELECT (W/B)? ");
        fflush(stdout);
        if (!fgets(line, sizeof line, stdin)) return WHITE;
        if (line[0] == 'W' || line[0] == 'w') return WHITE;
        if (line[0] == 'B' || line[0] == 'b') return BLACK;
    }
}

int display_ask_depth(void)
{
    char line[16];

    printf("(Level %d is beginner-friendly - it still thinks, but plays a\n"
           "weak move %d%% of the time so newer players have a real shot.)\n",
           SEARCH_DEPTH_BEGINNER, BEGINNER_RANDOM_MOVE_PERCENT);
    for (;;) {
        printf("WHICH LOOK AHEAD LEVEL DO YOU SELECT (%d-%d)? ",
               SEARCH_DEPTH_MIN, SEARCH_DEPTH_MAX);
        fflush(stdout);
        if (!fgets(line, sizeof line, stdin)) return SEARCH_DEPTH_MIN;
        if (line[0] >= '0' + SEARCH_DEPTH_MIN && line[0] <= '0' + SEARCH_DEPTH_MAX) {
            return line[0] - '0';
        }
    }
}

int display_get_human_move(board_t *b, move_t *out_move)
{
    move_t moves[MAX_MOVES];
    int count = generate_moves(b, moves);
    char line[32];

    for (;;) {
        printf("YOUR MOVE (e.g. e2e4, or \"quit\"): ");
        fflush(stdout);
        if (!fgets(line, sizeof line, stdin)) return 0;
        if (strncmp(line, "quit", 4) == 0) return 0;

        if (strlen(line) >= 4 &&
            line[0] >= 'a' && line[0] <= 'h' && line[1] >= '1' && line[1] <= '8' &&
            line[2] >= 'a' && line[2] <= 'h' && line[3] >= '1' && line[3] <= '8') {
            square_t from = (square_t)SQ(line[1] - '1', line[0] - 'a');
            square_t to   = (square_t)SQ(line[3] - '1', line[2] - 'a');
            int i;
            for (i = 0; i < count; i++) {
                if (moves[i].from == from && moves[i].to == to) {
                    *out_move = moves[i];
                    return 1;
                }
            }
        }
        display_message("INVALID MOVE - TRY AGAIN");
    }
}

void display_computer_thinking(void)
{
    /* no artificial delay on the desktop graphical build either */
}
