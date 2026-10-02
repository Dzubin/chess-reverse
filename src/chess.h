#ifndef CHESS_H
#define CHESS_H

/* 10x12 padded mailbox board: a 1-square border of sentinel bytes all the
   way around an 8x8 playable area, so move generation can walk off the
   real board and just check for PIECE_OFFBOARD instead of doing range
   checks. Matches CHESS.EXE's own board layout exactly (verified against
   the original's back-rank template and king/queen start squares). */
#define BOARD_WIDTH   10
#define BOARD_HEIGHT  12
#define BOARD_SIZE    (BOARD_WIDTH * BOARD_HEIGHT)

/* rank 0..7 = ranks 1..8, file 0..7 = files a..h */
#define SQ(rank, file) (((rank) + 2) * BOARD_WIDTH + ((file) + 1))
#define SQ_RANK(sq)    (((sq) / BOARD_WIDTH) - 2)
#define SQ_FILE(sq)    (((sq) % BOARD_WIDTH) - 1)

#define PIECE_NONE      0
#define PIECE_OFFBOARD  0xFF

#define PIECE_PAWN      1
#define PIECE_KNIGHT    2
#define PIECE_BISHOP    3
#define PIECE_ROOK      4
#define PIECE_QUEEN     5
#define PIECE_KING      6
#define PIECE_TYPE_MASK 0x07

/* flag bits stored alongside the piece type in each square byte */
#define PIECE_MOVED     0x08  /* has this piece moved at least once */
#define PIECE_CASTLED   0x10  /* has this king castled (king square only) */
#define PIECE_COLOR_BIT 0x80  /* set = black, clear = white */

#define WHITE 0
#define BLACK 1
#define OTHER_COLOR(c) (1 - (c))

#define SEARCH_DEPTH_BEGINNER 0  /* plays a real but shallow search, MOST of the
                                    time - see BEGINNER_RANDOM_MOVE_PERCENT */
#define SEARCH_DEPTH_MIN 0
#define SEARCH_DEPTH_MAX 6

/* At SEARCH_DEPTH_BEGINNER, this percent of the computer's moves are a
   uniformly random legal move instead of the search's choice - gives a
   new player genuine chances to win without the engine ever looking like
   it's not trying. Tunable to taste. */
#define BEGINNER_RANDOM_MOVE_PERCENT 35

#define MAX_MOVES 128

/* Evaluation weights, all in centipawns (positive favours white). Material
   values are standard textbook values - the original CHESS.EXE derives
   piece worth from a ray-cast exposure scan (FUN_1000_00f8) rather than a
   flat table, which hasn't been fully decoded yet; these are a placeholder
   to refine once that's cracked. The three development/castling weights
   below ARE decoded from the original, byte-for-byte (see analysis.md). */
#define VALUE_PAWN    100
#define VALUE_KNIGHT  300
#define VALUE_BISHOP  300
#define VALUE_ROOK    500
#define VALUE_QUEEN   900
#define VALUE_KING    20000

#define UNDEVELOPED_MINOR_PENALTY 2   /* knight/bishop still on home square */
#define EARLY_MAJOR_MOVE_PENALTY  2   /* rook/queen moved, or king moved without castling */
#define CASTLED_BONUS             6

/* Weight for the decoded board-control/exposure term (FUN_1000_00f8):
   from each occupied square, count pieces of each color reachable by the
   8 sliding directions + 8 knight offsets; (white_count - black_count) is
   scaled by this and added to the score. See analysis.md for the decode.
   The original applies this unweighted (effectively 1) from every square
   on the board, occupied or not; the rewrite only scans occupied squares
   for performance, so this weight is a free knob to compensate if that
   scoping ends up mattering in practice. */
#define MOBILITY_WEIGHT 1

typedef unsigned char square_t;
typedef unsigned char piece_t;

#define PIECE_TYPE(p)  ((p) & PIECE_TYPE_MASK)
#define PIECE_COLOR(p) (((p) & PIECE_COLOR_BIT) ? BLACK : WHITE)

typedef struct {
    square_t from;
    square_t to;
    piece_t  moved_piece;     /* piece byte at 'from' before the move, for undo */
    piece_t  captured_piece;  /* PIECE_NONE if no capture */
    square_t captured_square; /* differs from 'to' only for en passant */
    int      is_castle;       /* king-side/queen-side rook comes along too */
    square_t rook_from;
    square_t rook_to;
    int      is_promotion;    /* pawn reaching the back rank; always promotes to queen */
    square_t prev_en_passant; /* board's en-passant target before this move, for undo */
} move_t;

typedef struct {
    piece_t  square[BOARD_SIZE];
    square_t king_square[2];   /* indexed by WHITE/BLACK */
    square_t queen_square[2];
    int      side_to_move;
    square_t en_passant_square; /* 0 = none; real squares are never index 0 */
} board_t;

#endif
