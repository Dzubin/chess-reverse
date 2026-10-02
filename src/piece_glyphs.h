#ifndef PIECE_GLYPHS_H
#define PIECE_GLYPHS_H

/* Chess piece bitmaps extracted byte-for-byte from the original CHESS.EXE
   (workspace offset 0xa8c - see notes/analysis.md "Piece graphics
   extracted"). Pure data, no drawing code here - see gfx.h / board_render.h
   for how these get put on screen. */

#define PIECE_GLYPH_WIDTH     40
#define PIECE_GLYPH_HEIGHT    24
#define PIECE_GLYPH_ROW_BYTES 5   /* 40 bits packed MSB-first, 5 bytes/row */
#define PIECE_GLYPH_BYTES     (PIECE_GLYPH_HEIGHT * PIECE_GLYPH_ROW_BYTES)

#define PIECE_GLYPH_TYPES  6   /* indexed [PIECE_PAWN-1 .. PIECE_KING-1] */
#define PIECE_GLYPH_COLORS 2   /* 0 = black-filled, 1 = white-outline */

extern const unsigned char piece_glyph[PIECE_GLYPH_TYPES][PIECE_GLYPH_COLORS][PIECE_GLYPH_BYTES];

#endif
