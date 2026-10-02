#include "search.h"
#include "movegen.h"
#include "eval.h"
#include <stdlib.h>

#define SCORE_INF     1000000
#define SCORE_MATE    100000

static int negamax(board_t *b, int depth, int alpha, int beta)
{
    move_t moves[MAX_MOVES];
    int count = generate_moves(b, moves);
    int best, i;

    if (count == 0) {
        if (in_check(b, b->side_to_move)) {
            /* prefer a shorter mate: deeper mates score slightly worse */
            return -SCORE_MATE - depth;
        }
        return 0;
    }

    if (depth == 0) {
        int e = evaluate(b);
        return (b->side_to_move == WHITE) ? e : -e;
    }

    best = -SCORE_INF;
    for (i = 0; i < count; i++) {
        int score;
        make_move(b, &moves[i]);
        score = -negamax(b, depth - 1, -beta, -alpha);
        unmake_move(b, &moves[i]);

        if (score > best) best = score;
        if (best > alpha) alpha = best;
        if (alpha >= beta) break;
    }
    return best;
}

move_t search_best_move(board_t *b, int depth)
{
    move_t moves[MAX_MOVES];
    int count = generate_moves(b, moves);
    move_t best_move = moves[0];
    int best_score = -SCORE_INF;
    int d, i;

    for (d = 1; d <= depth; d++) {
        int alpha = -SCORE_INF, beta = SCORE_INF;
        for (i = 0; i < count; i++) {
            int score;
            make_move(b, &moves[i]);
            score = -negamax(b, d - 1, -beta, -alpha);
            unmake_move(b, &moves[i]);

            if (d == depth && score > best_score) {
                best_score = score;
                best_move = moves[i];
            }
            if (score > alpha) alpha = score;
        }
    }
    return best_move;
}

move_t choose_computer_move(board_t *b, int depth)
{
    if (depth == SEARCH_DEPTH_BEGINNER) {
        if (rand() % 100 < BEGINNER_RANDOM_MOVE_PERCENT) {
            move_t moves[MAX_MOVES];
            int count = generate_moves(b, moves);
            return moves[rand() % count];
        }
        return search_best_move(b, 1);
    }
    return search_best_move(b, depth);
}
