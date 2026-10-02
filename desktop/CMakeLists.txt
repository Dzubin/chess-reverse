# PicoCalc Chess - desktop (SDL2) build for Windows and Linux, by Thomas Dzubin.
#
# Builds the same game source as the firmware. Instead of the PicoCalc drivers it
# links the small SDL2 shim in this folder, which re-implements the
# picocalc-text-starter driver API (lcd / audio / south-bridge keyboard, by
# Blair Leduc) - see shim.h. The vendored driver headers and fonts are used
# as-is and nothing in the vendored folder is modified.
#
# Build on Windows (MSYS2 UCRT64 shell with SDL2 installed):
#     cmake -S desktop -B build-windows -G Ninja
#     ninja -C build-windows
# Output: chess-picocalc-Windows.exe, copied to the top-level project folder. SDL2 is
# linked statically, so it is one self-contained file.
#
# Build on Linux (needs the SDL2 development package, e.g. libsdl2-dev):
#     cmake -S desktop -B build-linux
#     cmake --build build-linux
# Output: chess-picocalc-Linux, copied to the top-level project folder together with
# libSDL2-2.0.so.0 (SDL2 is NOT linked in statically on Linux - keep the two
# files side by side; the executable finds the library through an $ORIGIN
# run-path).

cmake_minimum_required(VERSION 3.13)

project(chess-picocalc-desktop C)

set(CMAKE_C_STANDARD 11)

get_filename_component(TOP ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)
set(DRV ${TOP}/picocalc/drivers)

# Shared build logic (SDL2, output names, copying the result to the top level).
include(${CMAKE_CURRENT_LIST_DIR}/shim_desktop.cmake)

picocalc_desktop_app(
    NAME     chess-picocalc
    TITLE    "PicoCalc Chess"
    SOURCES
        ${TOP}/picocalc/display_picocalc.c
        ${TOP}/picocalc/gfx_picocalc.c
        ${TOP}/src/main.c
        ${TOP}/src/board.c
        ${TOP}/src/movegen.c
        ${TOP}/src/eval.c
        ${TOP}/src/search.c
        ${TOP}/src/piece_glyphs.c
        ${TOP}/src/board_render.c
        ${DRV}/font-8x10.c
    INCLUDES
        ${TOP}
        ${DRV}
        ${DRV}/..
        ${TOP}/src
)
