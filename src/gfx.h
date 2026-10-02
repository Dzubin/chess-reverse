#ifndef GFX_H
#define GFX_H

/* Generic pixel-drawing interface. This header has NO hardware-specific
   code in it - every function is implemented once per platform (see
   gfx_ppm.c for the desktop/testing backend that writes a .ppm image
   file; a PicoCalc backend would implement the same functions against
   its LCD driver instead). board_render.c is the only caller and never
   touches a framebuffer, register, or driver directly - that's the whole
   point of the split. */

/* A small named palette rather than raw RGB, so a platform with a
   different display (4-color CGA, 16-color, full RGB565, monochrome...)
   just picks its own closest values - see gfx_ppm.c / gfx_picocalc.c for
   the reference RGB mappings.

   The white-piece glyphs are thin outline art (see piece_glyphs.h), not a
   filled shape - a single fixed ink color can't have good contrast
   against BOTH a light and a dark square (a dark outline vanishes on a
   dark square; a light one vanishes on a light square). So there are two
   white-piece variants, and board_render.c - which already knows which
   square colour it just filled - picks whichever one contrasts: the
   *_DARK_INK variant (for a light square) or *_LIGHT_INK variant (for a
   dark square). This is the same problem the original's CGA renderer
   solved by XOR'ing the piece against the square's own background
   pattern (see analysis.md) - here it's solved by choosing the ink up
   front instead. GFX_BLACK_PIECE needs no such split: a solid dark fill
   already contrasts fine against both square tones. */
typedef enum {
    GFX_LIGHT_SQUARE,
    GFX_DARK_SQUARE,
    GFX_WHITE_PIECE_DARK_INK,   /* white piece, drawn on a light square */
    GFX_WHITE_PIECE_LIGHT_INK,  /* white piece, drawn on a dark square */
    GFX_BLACK_PIECE,
    GFX_CURSOR_HIGHLIGHT,       /* border around the square the cursor is on */
    GFX_SELECTED_HIGHLIGHT,     /* border around a picked-up piece's square */
    GFX_BACKGROUND
} gfx_color_t;

/* Called once before any drawing, with the pixel dimensions the caller
   wants to draw into (board_render.c always asks for SCREEN_WIDTH x
   SCREEN_HEIGHT from board_render.h). A fixed-framebuffer backend (e.g.
   a real LCD) can just ignore width/height if they always match its
   native resolution. */
void gfx_init(int width, int height);

/* Solid-fill an axis-aligned rectangle. */
void gfx_fill_rect(int x, int y, int w, int h, gfx_color_t color);

/* Draw a 1-bit-per-pixel bitmap: bits[row*row_bytes + byte], MSB-first
   within each byte. A set bit paints 'fg', a clear bit paints 'bg' -
   every pixel in the w*h box is written, none left untouched. Deliberately
   NOT "transparent over whatever's already there": a real LCD's blit call
   (see gfx_picocalc.c) can only write full opaque rectangles, it can't
   read back or selectively skip pixels, so board_render.c always passes
   the piece's own square color as bg - that's how the original's
   outline-style white pieces still read correctly against either square
   color, without needing the hardware to support transparency at all. */
void gfx_draw_bitmap_1bpp(int x, int y, const unsigned char *bits,
                           int w, int h, int row_bytes,
                           gfx_color_t fg, gfx_color_t bg);

/* Called once after a frame's drawing is complete (flush to the real
   display, or in gfx_ppm.c's case, write the image file). */
void gfx_present(void);

#endif
