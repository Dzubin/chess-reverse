#include "gfx.h"
#include "drivers/lcd.h"

/* Real hardware backend for gfx.h: the PicoCalc's ST7789P LCD driven
   through Blair Leduc's picocalc-text-starter lcd.c (see drivers/README.md).
   This is the only file that knows the LCD exists - board_render.c and
   piece_glyphs.c are unchanged from the desktop build. */

static uint16_t color_to_rgb565(gfx_color_t color)
{
    switch (color) {
    case GFX_LIGHT_SQUARE:      return RGB(240, 217, 181);
    case GFX_DARK_SQUARE:       return RGB(181, 136,  99);
    /* board_render.c already picked the contrasting variant for the
       square this piece sits on - see the gfx_color_t comment in gfx.h. */
    case GFX_WHITE_PIECE_DARK_INK:  return RGB( 25,  35,  95);
    case GFX_WHITE_PIECE_LIGHT_INK: return RGB(250, 245, 225);
    case GFX_BLACK_PIECE:       return RGB( 20,  20,  20);
    case GFX_CURSOR_HIGHLIGHT:   return RGB(  0, 220, 220);
    case GFX_SELECTED_HIGHLIGHT: return RGB(  0, 220,  60);
    case GFX_BACKGROUND:
    default:                     return RGB(  0,   0,   0);
    }
}

void gfx_init(int width, int height)
{
    (void)width;   /* the LCD is always 320x320 - see board_render.h */
    (void)height;
    lcd_init();
}

void gfx_fill_rect(int x, int y, int w, int h, gfx_color_t color)
{
    lcd_solid_rectangle(color_to_rgb565(color), (uint16_t)x, (uint16_t)y,
                        (uint16_t)w, (uint16_t)h);
}

void gfx_draw_bitmap_1bpp(int x, int y, const unsigned char *bits,
                           int w, int h, int row_bytes,
                           gfx_color_t fg, gfx_color_t bg)
{
    /* lcd_blit() writes a full opaque row at a time (it can't read the
       screen back or skip pixels), so build one row of real pixels -
       every pixel, fg or bg - and blit it. Piece glyphs are 40px wide
       (see piece_glyphs.h); this buffer covers any width up to the whole
       320px screen, same size lcd_solid_rectangle uses internally. */
    static uint16_t row_pixels[320];
    uint16_t fg565 = color_to_rgb565(fg);
    uint16_t bg565 = color_to_rgb565(bg);
    int row, col;

    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            int byte_index = row * row_bytes + col / 8;
            int bit_index = 7 - (col % 8);
            row_pixels[col] = (bits[byte_index] & (1 << bit_index)) ? fg565 : bg565;
        }
        lcd_blit(row_pixels, (uint16_t)x, (uint16_t)(y + row), (uint16_t)w, 1);
    }
}

void gfx_present(void)
{
    /* lcd_blit()/lcd_solid_rectangle() write straight to the display's
       own frame memory - nothing to flush. */
}
