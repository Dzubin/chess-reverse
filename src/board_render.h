#ifndef BOARD_RENDER_H
#define BOARD_RENDER_H

#include "chess.h"
#include "gfx.h"

/* Target: PicoCalc's 320x320 pixel LCD. The board uses the full screen,
   one 8x8 grid of square pixel cells - 320 divides evenly by 8, and
   happens to exactly match the original glyphs' 40px width (see
   piece_glyphs.h), so pieces need no scaling, just centering. Change
   these two if porting to a different screen size; everything else in
   board_render.c is computed from them. */
#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  320
#define SQUARE_PIXELS  (SCREEN_WIDTH / 8)
#define HIGHLIGHT_BORDER_PIXELS 4

/* Draws the full board (squares + pieces) for the current position via
   gfx.h and calls gfx_present(). White's side is drawn at the bottom
   (rank 1 nearest the viewer), the usual over-the-board orientation. */
void board_render(const board_t *b);

/* Draws a colored border around one square, on top of whatever
   board_render() already drew there - a plain outline (via gfx_fill_rect,
   four thin strips) rather than a filled square, so the piece underneath
   stays visible. Used for cursor/selection feedback during move input;
   callers must call board_render() first in the same frame; there's no
   "unhighlight" - redraw the board to clear it. Calls gfx_present(). */
void board_highlight_square(square_t sq, gfx_color_t color);

#endif
