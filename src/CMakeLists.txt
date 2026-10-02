cmake_minimum_required(VERSION 3.10)
project(chess C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CHESS_CORE_SOURCES
    main.c
    board.c
    movegen.c
    eval.c
    search.c
)

# Plain-text console board (letters, any terminal).
add_executable(chess ${CHESS_CORE_SOURCES} display_console.c)

# Same game, graphical board drawn from the original's extracted piece
# bitmaps (see piece_glyphs.h) via the generic gfx.h interface. Writes
# chess_board.ppm after every move - see gfx_ppm.c. Swap gfx_ppm.c for a
# real display backend (e.g. PicoCalc's LCD driver) to run this on
# target hardware; board_render.c and piece_glyphs.c need no changes.
add_executable(chess_gfx ${CHESS_CORE_SOURCES}
    display_graphics.c
    board_render.c
    piece_glyphs.c
    gfx_ppm.c
)

# The platform is part of each executable's file name (chess-Windows.exe,
# chess_gfx-Windows.exe / chess-Linux, ...), so a downloaded file always says
# which system it was built for.
if(WIN32)
    set(PLATFORM_NAME Windows)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_NAME Linux)
else()
    set(PLATFORM_NAME ${CMAKE_SYSTEM_NAME})
endif()
foreach(target chess chess_gfx)
    set_target_properties(${target} PROPERTIES OUTPUT_NAME ${target}-${PLATFORM_NAME})
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    endif()
endforeach()
