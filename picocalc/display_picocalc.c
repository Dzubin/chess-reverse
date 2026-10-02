#include "display.h"
#include "board_render.h"
#include "movegen.h"
#include "drivers/lcd.h"
#include "drivers/southbridge.h"
#include "drivers/keyboard.h"
#include "drivers/audio.h"
#include "pico/bootrom.h"  /* reset_usb_boot() - BOOTSEL reboot ('~') */
#include "hardware/watchdog.h" /* watchdog_hw, watchdog_reboot() - exit to the UF2 Loader */
#include "pico/stdlib.h"
#include "loader_const.h"  /* the UF2 Loader hand-off values */
#include "pico/time.h"     /* time_us_64() - random seed, see display_computer_thinking() */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* PicoCalc display.h backend.
   - Color/level selection is a full-screen splash (see draw_splash()),
     not an overlay on the board.
   - Moves are picked with the keyboard's arrow keys and Enter, the same
     lift-and-place idea the original used with DEL/INS: move a cursor
     onto your piece, Enter picks it up, move the cursor to the target,
     Enter drops it. An illegal Enter (empty/enemy square to pick up, or
     an illegal destination) beeps instead of doing anything. Enter again
     on the picked-up square's own position cancels the pick-up; ESC also
     cancels a pick-up, and does nothing at all if nothing's picked up -
     it never quits. Q or q brings up a "QUIT GAME?" confirmation screen;
     only Y/y actually quits, any other key dismisses it and returns to
     the game exactly where it was. When the game ends (a quit, mate or
     stalemate) display_finish() leaves for the PicoCalc UF2 Loader menu.
   - '~' (SHIFT+backtick) reboots to BOOTSEL from every single keyboard
     read in this file - splash, prompts, move input, all of it - via the
     one shared read_key_blocking() helper below.
   - The computer's move gets a random 1-5 second pause (with a "COMPUTER
     THINKING..." message) before it's shown - see
     display_computer_thinking() - since search at low depths is fast
     enough to feel instant/unnatural otherwise. Requires sb_init()
     (south-bridge/keyboard I2C bus) to have run before the FIRST keyboard
     read of the game - read_key_blocking() does that lazily; nothing else
     in this file, or in the shared ../src/main.c it's linked with, does
     it, so don't remove that lazy-init without putting it somewhere
     else. */

static int audio_ready = 0;
static int sb_ready = 0;
static int rand_seeded = 0;
static board_t last_board;
static int have_last_board = 0;

static uint8_t read_key_blocking(void)
{
    if (!sb_ready) {
        sb_init();
        sb_ready = 1;
    }

    for (;;) {
        uint16_t event = sb_read_keyboard();
        uint8_t state = (uint8_t)(event >> 8);
        uint8_t code = (uint8_t)(event & 0xFF);

        if (state != KEY_STATE_PRESSED) continue;

        if (code == '~') {
            reset_usb_boot(0, 0);
            for (;;) tight_loop_contents();
        }
        return code;
    }
}

static void beep_illegal(void)
{
    if (!audio_ready) {
        audio_init();
        audio_ready = 1;
    }
    audio_play_sound_blocking(HIGH_BEEP, HIGH_BEEP, 120);
}

static void splash_center(int row, const char *text)
{
    int col = ((int)lcd_get_columns() - (int)strlen(text)) / 2;
    if (col < 0) col = 0;
    lcd_putstr((uint8_t)col, (uint8_t)row, text);
}

static void draw_splash(const char *prompt)
{
    lcd_solid_rectangle(RGB(20, 20, 40), 0, 0, WIDTH, HEIGHT);
    lcd_set_foreground(RGB(255, 255, 255));
    lcd_set_background(RGB(20, 20, 40));
    splash_center(4, "C H E S S");
    splash_center(6, "a C rewrite of a 1980s");
    splash_center(7, "DOS chess program");
    lcd_set_foreground(RGB(255, 220, 0));
    splash_center(12, prompt);
}

/* Shows a "QUIT GAME?" screen and reads exactly one key: Y/y confirms,
   anything else (including another accidental Q) dismisses it. Caller is
   responsible for redrawing the board afterwards if it doesn't quit. */
static int confirm_quit(void)
{
    lcd_solid_rectangle(RGB(40, 0, 0), 0, 0, WIDTH, HEIGHT);
    lcd_set_foreground(RGB(255, 255, 255));
    lcd_set_background(RGB(40, 0, 0));
    splash_center(7, "QUIT GAME?");
    splash_center(9, "(Y) YES   (anything else) NO");

    {
        uint8_t c = read_key_blocking();
        return (c == 'Y' || c == 'y');
    }
}

void display_board(const board_t *b)
{
    last_board = *b;
    have_last_board = 1;
    board_render(b);
}

/* Writes msg on a text row over the top of the board, on a black bar. */
static void message_row(int row, const char *msg)
{
    lcd_solid_rectangle(RGB(0, 0, 0), 0, (uint16_t)(row * GLYPH_HEIGHT), WIDTH, GLYPH_HEIGHT);
    lcd_set_foreground(RGB(255, 255, 255));
    lcd_set_background(RGB(0, 0, 0));
    lcd_putstr(0, (uint8_t)row, msg);
}

void display_message(const char *msg)
{
    message_row(0, msg);
}

/* Leaves the program for the PicoCalc UF2 Loader's menu: shows a short
   message, asks the loader for its menu (see LOADER_COMMAND_MAGIC in
   loader_const.h) and reboots with the watchdog. If the program was flashed
   straight to the chip with no loader, nothing reads the request and it
   simply restarts. The Windows / Linux build uses a stand-in watchdog that
   closes the program. */
static void exit_to_loader(void)
{
    message_row(0, "BACK TO THE LOADER...");
    sleep_ms(LOADER_EXIT_MSG_MS);

    watchdog_hw->scratch[LOADER_SCRATCH_MODE] = LOADER_BOOT_MODE_SD;
    watchdog_hw->scratch[LOADER_SCRATCH_ARGUMENT] = 0;
    watchdog_hw->scratch[LOADER_SCRATCH_MAGIC] = LOADER_COMMAND_MAGIC;
    watchdog_reboot(0, 0, LOADER_REBOOT_DELAY_MS);

    for (;;)
        tight_loop_contents();      /* the reboot comes in a few milliseconds */
}

/* The game is over. After a quit the loader menu follows at once (GOODBYE is
   already on screen); after mate or stalemate the result stays up until a key
   is pressed, so it can be read. '~' still goes to BOOTSEL. */
void display_finish(int player_quit)
{
    if (!player_quit) {
        message_row(FINISH_ROW_PROMPT, "ANY KEY: BACK TO THE LOADER");
        (void)read_key_blocking();
    }
    exit_to_loader();
}

int display_ask_color(void)
{
    for (;;) {
        draw_splash("WHICH COLOR? (W/B)");
        uint8_t c = read_key_blocking();
        if (c == 'W' || c == 'w') return WHITE;
        if (c == 'B' || c == 'b') return BLACK;
    }
}

int display_ask_depth(void)
{
    char prompt[48];
    int depth;

    snprintf(prompt, sizeof prompt, "LEVEL? (%d-%d, %d=BEGINNER)",
              SEARCH_DEPTH_MIN, SEARCH_DEPTH_MAX, SEARCH_DEPTH_BEGINNER);
    for (;;) {
        draw_splash(prompt);
        uint8_t c = read_key_blocking();
        if (c >= '0' + SEARCH_DEPTH_MIN && c <= '0' + SEARCH_DEPTH_MAX) {
            depth = c - '0';
            break;
        }
    }

    if (have_last_board) {
        board_render(&last_board);
    }
    return depth;
}

int display_get_human_move(board_t *b, move_t *out_move)
{
    move_t moves[MAX_MOVES];
    int count = generate_moves(b, moves);
    int cursor_rank = SQ_RANK(b->king_square[b->side_to_move]);
    int cursor_file = SQ_FILE(b->king_square[b->side_to_move]);
    int has_selection = 0;
    square_t selected_sq = 0;

    for (;;) {
        square_t cursor_sq = (square_t)SQ(cursor_rank, cursor_file);
        uint8_t c;

        board_render(b);
        if (has_selection) {
            board_highlight_square(selected_sq, GFX_SELECTED_HIGHLIGHT);
        }
        board_highlight_square(cursor_sq, GFX_CURSOR_HIGHLIGHT);

        c = read_key_blocking();

        if (c == KEY_ESC) {
            has_selection = 0;  /* no-op if nothing was selected */
            continue;
        }
        if (c == 'Q' || c == 'q') {
            if (confirm_quit()) {
                return 0;
            }
            continue;
        }
        if (c == KEY_UP)    { if (cursor_rank < 7) cursor_rank++; continue; }
        if (c == KEY_DOWN)  { if (cursor_rank > 0) cursor_rank--; continue; }
        if (c == KEY_LEFT)  { if (cursor_file > 0) cursor_file--; continue; }
        if (c == KEY_RIGHT) { if (cursor_file < 7) cursor_file++; continue; }
        if (c != KEY_ENTER && c != KEY_RETURN) continue;

        if (!has_selection) {
            piece_t p = b->square[cursor_sq];
            int i, has_move = 0;

            if (p == PIECE_NONE || p == PIECE_OFFBOARD || PIECE_COLOR(p) != b->side_to_move) {
                beep_illegal();
                continue;
            }
            for (i = 0; i < count; i++) {
                if (moves[i].from == cursor_sq) { has_move = 1; break; }
            }
            if (!has_move) {
                beep_illegal();
                continue;
            }
            selected_sq = cursor_sq;
            has_selection = 1;
            continue;
        }

        if (cursor_sq == selected_sq) {
            has_selection = 0;  /* put the piece back down, unselected */
            continue;
        }

        {
            int i;
            for (i = 0; i < count; i++) {
                if (moves[i].from == selected_sq && moves[i].to == cursor_sq) {
                    *out_move = moves[i];
                    return 1;
                }
            }
        }
        beep_illegal();
    }
}

void display_computer_thinking(void)
{
    int delay_ms;

    /* main.c already seeds rand() (for beginner-mode blunders) with
       time(NULL) ^ an address, but there's no RTC here so time(NULL) is
       typically a fixed epoch, and stack addresses don't vary between
       boots the way they do with a desktop OS's ASLR - so that seed can
       easily be the same every power-on. Re-seed from the RP2350's
       always-running microsecond timer instead, once, the first time an
       actual random delay is needed; this also fixes the same weakness
       for beginner mode's random moves, since they share one rand()
       stream. */
    if (!rand_seeded) {
        srand((unsigned int)time_us_64());
        rand_seeded = 1;
    }

    display_message("COMPUTER THINKING...");
    delay_ms = 1000 + (rand() % 4001);  /* 1.000 - 5.000 seconds */
    sleep_ms((uint32_t)delay_ms);
}
