# Changelog

All notable changes to the CHESS.EXE reverse engineering and its C rewrite.
Format loosely follows [Keep a Changelog](https://keepachangelog.com/).

## [Unreleased]

### Added
- Build outputs for the PicoCalc are named for what they are, with the chip or
  system at the end: `chess-picocalc-RP2040.uf2`, `chess-picocalc-RP2350.uf2`,
  `chess-picocalc-Windows.exe` and `chess-picocalc-Linux`.
- Leaving: when a game ends (a quit, mate or stalemate) the PicoCalc now leaves
  for the UF2 Loader menu through a new `display_finish()` hook in
  `src/display.h`, called by the shared `src/main.c`. It asks the loader for its
  menu through the watchdog scratch registers (constants in
  `picocalc/loader_const.h`); with no loader installed the program just
  restarts, and on a PC it closes. The console and graphics-file backends do
  nothing there. `~` still goes to BOOTSEL.
- Desktop build: the build logic that was repeated in `desktop/CMakeLists.txt` now
  lives in `desktop/shim_desktop.cmake` (that file now only names the program and
  its sources), and the shim files were refreshed from a shared copy.
- Windows build: `desktop/` holds a small SDL2 shim that re-implements the
  PicoCalc driver API (LCD, keyboard, audio) so the same PicoCalc code runs on a
  PC; SDL2 is linked statically, giving one self-contained
  `chess-picocalc-Windows.exe`.
- Linux build: the same `desktop/` build also builds on Linux and produces
  `chess-picocalc-Linux`, with SDL2 linked dynamically and `libSDL2-2.0.so.0`
  copied next to it.

## Earlier work

### Added
- Reverse engineering of the 1980s MS-DOS `CHESS.EXE` (CHESS88 V2.0) with
  Ghidra: generated disassembly, decompiler pseudocode, and working analysis
  notes in `notes/analysis.md`.
- A portable C rewrite in `src/`: legal move generation (castling, en passant,
  promotion), check, checkmate and stalemate detection, the original's four
  evaluation terms, and an iterative-deepening alpha-beta search, with a console
  backend and a graphical backend behind the swappable `display.h` layer.
- The original's piece artwork, extracted byte for byte as 40x24-pixel bitmaps,
  drawn by a portable board renderer sized for the 320x320 PicoCalc screen.
- The full game on the PicoCalc (`picocalc/`), played with the arrow keys and
  Enter, with a quit confirmation on `Q` and `~` rebooting to BOOTSEL.
- A "level 0" search depth that plays a shallow search most of the time and a
  random legal move the rest, to give a new player a real chance to win.
