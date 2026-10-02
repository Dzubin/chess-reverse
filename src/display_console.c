#include "display.h"
#include "movegen.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static char piece_letter(piece_t p)
{
    char c;

    switch (PIECE_TYPE(p)) {
    case PIECE_PAWN:   c = 'P'; break;
    case PIECE_KNIGHT: c = 'N'; break;
    case PIECE_BISHOP: c = 'B'; break;
    case PIECE_ROOK:   c = 'R'; break;
    case PIECE_QUEEN:  c = 'Q'; break;
    case PIECE_KING:   c = 'K'; break;
    default: return '.';
    }
    return (PIECE_COLOR(p) == BLACK) ? (char)tolower((unsigned char)c) : c;
}

void display_board(const board_t *b)
{
    int rank, file;

    for (rank = 7; rank >= 0; rank--) {
        printf("%d  ", rank + 1);
        for (file = 0; file < 8; file++) {
            printf("%c ", piece_letter(b->square[SQ(rank, file)]));
        }
        printf("\n");
    }
    printf("   a b c d e f g h\n\n");
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
    /* no artificial delay on the console - depth already sets the pace */
}
