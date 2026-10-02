#include "gfx.h"
#include <stdio.h>
#include <stdlib.h>

/* Reference/testing backend for gfx.h: renders into an in-memory RGB
   framebuffer and writes it out as a .ppm image (the simplest possible
   real image format - a short text header followed by raw RGB bytes, no
   compression library needed) each time gfx_present() is called. Not
   meant for PicoCalc itself - it's here so the portable board_render.c
   code can be seen working on any machine with a C compiler, before a
   real LCD backend exists. A PicoCalc backend would implement these same
   four functions against its display driver instead of a file. */

#define PPM_FILENAME "chess_board.ppm"

static unsigned char *framebuffer = NULL; /* width*height*3 bytes, row-major RGB */
static int fb_width = 0, fb_height = 0;

static void color_to_rgb(gfx_color_t color, unsigned char rgb[3])
{
    switch (color) {
    case GFX_LIGHT_SQUARE:      rgb[0] = 240; rgb[1] = 217; rgb[2] = 181; break;
    case GFX_DARK_SQUARE:       rgb[0] = 181; rgb[1] = 136; rgb[2] =  99; break;
    /* board_render.c already picked the contrasting variant for the
       square this piece sits on - see the gfx_color_t comment in gfx.h. */
    case GFX_WHITE_PIECE_DARK_INK:  rgb[0] =  25; rgb[1] =  35; rgb[2] =  95; break;
    case GFX_WHITE_PIECE_LIGHT_INK: rgb[0] = 250; rgb[1] = 245; rgb[2] = 225; break;
    case GFX_BLACK_PIECE:       rgb[0] =  20; rgb[1] =  20; rgb[2] =  20; break;
    case GFX_CURSOR_HIGHLIGHT:   rgb[0] =   0; rgb[1] = 220; rgb[2] = 220; break;
    case GFX_SELECTED_HIGHLIGHT: rgb[0] =   0; rgb[1] = 220; rgb[2] =  60; break;
    case GFX_BACKGROUND:
    default:                     rgb[0] =   0; rgb[1] =   0; rgb[2] =   0; break;
    }
}

static void put_pixel(int x, int y, const unsigned char rgb[3])
{
    unsigned char *p;

    if (x < 0 || y < 0 || x >= fb_width || y >= fb_height) return;
    p = &framebuffer[((size_t)y * fb_width + x) * 3];
    p[0] = rgb[0];
    p[1] = rgb[1];
    p[2] = rgb[2];
}

void gfx_init(int width, int height)
{
    unsigned char bg[3];
    size_t i, n;

    free(framebuffer);
    fb_width = width;
    fb_height = height;
    framebuffer = malloc((size_t)width * height * 3);

    color_to_rgb(GFX_BACKGROUND, bg);
    n = (size_t)width * height;
    for (i = 0; i < n; i++) {
        framebuffer[i*3 + 0] = bg[0];
        framebuffer[i*3 + 1] = bg[1];
        framebuffer[i*3 + 2] = bg[2];
    }
}

void gfx_fill_rect(int x, int y, int w, int h, gfx_color_t color)
{
    unsigned char rgb[3];
    int row, col;

    color_to_rgb(color, rgb);
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            put_pixel(x + col, y + row, rgb);
        }
    }
}

void gfx_draw_bitmap_1bpp(int x, int y, const unsigned char *bits,
                           int w, int h, int row_bytes,
                           gfx_color_t fg, gfx_color_t bg)
{
    unsigned char fg_rgb[3], bg_rgb[3];
    int row, col;

    color_to_rgb(fg, fg_rgb);
    color_to_rgb(bg, bg_rgb);
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            int byte_index = row * row_bytes + col / 8;
            int bit_index = 7 - (col % 8);
            int set = bits[byte_index] & (1 << bit_index);
            put_pixel(x + col, y + row, set ? fg_rgb : bg_rgb);
        }
    }
}

void gfx_present(void)
{
    FILE *f = fopen(PPM_FILENAME, "wb");
    if (!f) return;
    fprintf(f, "P6\n%d %d\n255\n", fb_width, fb_height);
    fwrite(framebuffer, 1, (size_t)fb_width * fb_height * 3, f);
    fclose(f);
}
