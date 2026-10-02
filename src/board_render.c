#include "board_render.h"
#include "gfx.h"
#include "piece_glyphs.h"

void board_render(const board_t *b)
{
    int rank, file;

    gfx_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    for (rank = 0; rank < 8; rank++) {
        int y = (7 - rank) * SQUARE_PIXELS;  /* rank 8 at the top, rank 1 at the bottom */

        for (file = 0; file < 8; file++) {
            int x = file * SQUARE_PIXELS;
            int dark = ((file + rank) % 2) == 0;  /* a1 is a dark square */
            gfx_color_t square_color = dark ? GFX_DARK_SQUARE : GFX_LIGHT_SQUARE;
            piece_t p = b->square[SQ(rank, file)];

            gfx_fill_rect(x, y, SQUARE_PIXELS, SQUARE_PIXELS, square_color);

            if (p != PIECE_NONE && p != PIECE_OFFBOARD) {
                int type = PIECE_TYPE(p);
                int is_black = (PIECE_COLOR(p) == BLACK);
                const unsigned char *bits = piece_glyph[type - 1][is_black ? 0 : 1];
                int px = x + (SQUARE_PIXELS - PIECE_GLYPH_WIDTH) / 2;
                int py = y + (SQUARE_PIXELS - PIECE_GLYPH_HEIGHT) / 2;
                gfx_color_t fg;

                if (is_black) {
                    fg = GFX_BLACK_PIECE;
                } else {
                    /* pick whichever white-piece ink contrasts against
                       this square - see the gfx_color_t comment in gfx.h */
                    fg = dark ? GFX_WHITE_PIECE_LIGHT_INK : GFX_WHITE_PIECE_DARK_INK;
                }

                gfx_draw_bitmap_1bpp(px, py, bits, PIECE_GLYPH_WIDTH, PIECE_GLYPH_HEIGHT,
                                     PIECE_GLYPH_ROW_BYTES, fg, square_color);
            }
        }
    }

    gfx_present();
}

void board_highlight_square(square_t sq, gfx_color_t color)
{
    int rank = SQ_RANK(sq);
    int file = SQ_FILE(sq);
    int x = file * SQUARE_PIXELS;
    int y = (7 - rank) * SQUARE_PIXELS;
    int t = HIGHLIGHT_BORDER_PIXELS;

    gfx_fill_rect(x, y, SQUARE_PIXELS, t, color);                        /* top */
    gfx_fill_rect(x, y + SQUARE_PIXELS - t, SQUARE_PIXELS, t, color);    /* bottom */
    gfx_fill_rect(x, y, t, SQUARE_PIXELS, color);                        /* left */
    gfx_fill_rect(x + SQUARE_PIXELS - t, y, t, SQUARE_PIXELS, color);    /* right */

    gfx_present();
}
