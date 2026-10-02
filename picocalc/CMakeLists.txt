# PicoCalc hardware build - the full playable game (../src/main.c and the
# rest of the portable core, unchanged) with a graphical board and
# keyboard-driven move input via display_picocalc.c. The desktop build
# (../src/CMakeLists.txt, targets chess/chess_gfx) is separate; this is
# the only place the Pico SDK gets pulled in.

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# set(PICO_BOARD pico2 CACHE STRING "Board type")   # left commented out on
# purpose - pick the board via the VS Code Pico extension or -DPICO_BOARD=

include(pico_sdk_import.cmake)

project(chess-picocalc C CXX ASM)

# pico_generate_pio_header() (used below for our own audio.pio, and also
# internally by pico_sdk_init()'s pico_status_led component on some
# boards) needs the pioasm host tool, which the SDK builds from source the
# first time - unlike picotool, it has no built-in "use this existing
# build" override. If that source build is blocked locally (e.g. Windows
# Smart App Control refusing to run a freshly-built test executable),
# pass -DPIOASM_EXECUTABLE=<path to an already-built pioasm(.exe)> (from
# any other PicoCalc project's build directory) to skip rebuilding it.
# Must run before pico_sdk_init(), which is what first pulls pioasm in.
# Portable projects/CI just omit the flag.
if (DEFINED PIOASM_EXECUTABLE AND NOT TARGET pioasm)
    add_executable(pioasm IMPORTED GLOBAL)
    set_property(TARGET pioasm PROPERTY IMPORTED_LOCATION ${PIOASM_EXECUTABLE})
endif()

pico_sdk_init()

add_executable(chess-picocalc
    display_picocalc.c
    gfx_picocalc.c
    drivers/lcd.c
    drivers/font-8x10.c
    drivers/southbridge.c
    drivers/audio.c
    ../src/main.c
    ../src/board.c
    ../src/movegen.c
    ../src/eval.c
    ../src/search.c
    ../src/piece_glyphs.c
    ../src/board_render.c
)

pico_generate_pio_header(chess-picocalc ${CMAKE_CURRENT_LIST_DIR}/drivers/audio.pio)

pico_set_program_name(chess-picocalc "chess-picocalc")
pico_set_program_version(chess-picocalc "0.1")

pico_enable_stdio_uart(chess-picocalc 0)
pico_enable_stdio_usb(chess-picocalc 0)

target_compile_options(chess-picocalc PRIVATE -Wall -Wextra)

target_include_directories(chess-picocalc PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../src
)

target_link_libraries(chess-picocalc
    pico_stdlib
    hardware_spi
    hardware_watchdog   # watchdog_reboot(): leaving for the UF2 Loader
    hardware_i2c
    hardware_pio
    hardware_clocks
)

# The chip is part of every output file name (chess-picocalc-RP2040.uf2 /
# chess-picocalc-RP2350.uf2, and the matching .elf/.bin/.hex), so a downloaded file
# always says which board it was built for.
if(PICO_PLATFORM MATCHES "^rp2040")
    set(CHIP_NAME RP2040)
elseif(PICO_PLATFORM MATCHES "^rp2350")
    set(CHIP_NAME RP2350)
else()
    set(CHIP_NAME ${PICO_PLATFORM})
endif()
set_target_properties(chess-picocalc PROPERTIES OUTPUT_NAME chess-picocalc-${CHIP_NAME})

pico_add_extra_outputs(chess-picocalc)

# Copy the finished .uf2 (same name) to the top-level project folder, so the
# RP2040 and RP2350 builds can sit side by side.
add_custom_command(TARGET chess-picocalc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:chess-picocalc>/$<TARGET_FILE_BASE_NAME:chess-picocalc>.uf2
            ${CMAKE_CURRENT_LIST_DIR}/../
)

