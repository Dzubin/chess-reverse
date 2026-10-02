# CHESS.EXE reverse-engineering notes

## Identity

The strings table identifies the program precisely:

    CHESS88 V2.0 - A CHESS PROGRAM FOR THE IBM PERSONAL COMPUTER BY DON BERG

## File format

Classic MS-DOS MZ EXE, single 64KB-ish code/data segment (Ghidra loads it at
segment 1000h). See `header.txt` for the full field dump. Key facts:

- Header: 512 bytes (0x20 paragraphs)
- Load module: 8928 bytes
- Entry point: CS:IP = 0000:0448 (relative to load segment)
- One relocation entry: fixes up the segment immediate in the
  `MOV AX,0x10FF` / `MOV DS,AX` / `MOV ES,AX` sequence at the very start of
  `entry` (0448-0451). This repoints DS/ES to a *second* segment roughly
  0x10FF paragraphs (~68KB) above the load segment — i.e. the program asks
  DOS for a big chunk of memory (e_minalloc=0, e_maxalloc=0xFFFF = "give me
  everything") and uses a chunk of it as a working data segment separate
  from the code segment. The original DS (≈ PSP segment) is saved off to
  a variable at code-segment offset 0x9ee before being overwritten.
- **Ghidra quirk to remember**: its listing prints segment-register MOV
  instructions in a confusing order. Byte pattern `8E D8` is genuinely
  `MOV DS,AX` (verified independently with objdump -m i8086), but Ghidra's
  text rendering shows it as `MOV AX,DS`. Don't trust the printed operand
  order for `8E xx` opcodes — check the raw opcode byte (8E = load INTO
  segment register; 8C = store segment register OUT).

## Runtime environment / interrupts used

- `INT 21h` (DOS): AH=09 (print $-terminated string) — by far the most
  common; AH=08 (read char, no echo); AH=01 (read char, with echo);
  AH=0B (check stdin status); AH=02 (write char); AH=2C (get system time —
  used near startup and again around offset 0x606, almost certainly to
  seed the "random" move-ordering / opening variety, since there's no other
  obvious source of randomness).
- `INT 10h` (BIOS video): AH=02 (set cursor position) — used to draw the
  board/pieces at fixed text-mode row/col positions; AH=08 (read
  char+attribute at cursor); one call at entry+0x24 with AX=0x0005 (AH=05,
  select active display page — NOT a mode set, just page select, so this
  program does not change video mode; it runs in whatever text mode DOS
  started in, typically 80x25 color text).

This is a **text-mode, keyboard-driven** program: it draws a board with
BIOS cursor positioning + DOS character output, no graphics.

## User interface (from strings.txt)

- `WHICH COLOR DO YOU SELECT (W/B)? $`
- `WHICH LOOK AHEAD LEVEL DO YOU SELECT (1-6)? $` — search depth 1-6 is
  player-selectable, confirms a depth-limited tree search (minimax /
  alpha-beta) engine rather than anything book-only.
- Piece movement is a **cursor/lift-and-place scheme**, not typed algebraic
  notation: `PLAYER(B): PICK UP PIECE USING "DEL"` /
  `PLAYER(B): PLACE PIECE USING "INS"`.
- `INVALID MOVE - TRY AGAIN `
- Computer's status/output line: `CHECK $COMPUTER(W): WORKING LEVEL X $`
  and `COMPUTER(W): $XX-XX $O-O $O-O-O $PxPep $` — so the engine prints
  its own move in `e2-e4` / `O-O` / `O-O-O` / `PxPep` (en passant) style
  while it's "WORKING" at a given ply.
- End of game: `MATE - I WIN`, `$MATE IN X MOVES$`, `MATE - YOU WIN`.
- `GIOQHKMPRS` — short odd string near the message table; not yet
  understood. Candidate theories: piece-letter table (King/Queen/Rook/
  Bishop/kNight/Pawn plus board glyphs), or part of an adjacent packed data
  table that happens to disassemble as ASCII. Needs a cross-reference check
  (who reads this address).

## Program structure (Ghidra auto-analysis)

Ghidra's "Old-style DOS Executable (MZ)" loader + `x86:LE:16:Real Mode`
language imported and auto-analyzed cleanly, finding **38 functions**.
Exports:

- `disasm/chess_disasm.txt` — full linear disassembly, address / raw bytes
  / Intel-syntax mnemonic / any end-of-line comment Ghidra attached.
- `disasm/chess_decompiled.c` — Ghidra decompiler's C-like pseudocode for
  every one of the 38 functions, one section per function. This is a
  *starting point*, not trustworthy C — 16-bit real-mode segmented code
  decompiles roughly (expect wrong types, raw segment:offset pointer math,
  wrong signedness) and needs to be read against the disassembly, not
  taken at face value.
- `entry` (0448) is the real program entry; it calls, in order, two
  init/setup routines (FUN_1000_0964, FUN_1000_090e), sets video page,
  calls a board-draw routine (FUN_1000_036e), then loops: alternating
  between the human-move routine (FUN_1000_0298) and computer-move routine
  (FUN_1000_0bf2), incrementing a "which side" counter at 0x9d4 each turn,
  branching on a flag at 0x9ce.7 to decide who (human/computer) moves first.

## Regenerating the Ghidra exports

Requires the JDK at `C:\Program Files\Java\jdk-27` (not on PATH by
default) and Ghidra unpacked at
`C:\Users\talne\ghidra_12.1.3_PUBLIC_20260817\ghidra_12.1.3_PUBLIC`.
Ghidra 12.1.3's headless analyzer treats `.py` postScripts as PyGhidra
(real CPython via jep), which isn't set up here — so the export script is
written in **Java** instead (`notes/ghidra_scripts/ExportChess.java`),
which Ghidra compiles and runs on the fly with no extra setup.

    export JAVA_HOME="/c/Program Files/Java/jdk-27"
    export PATH="$JAVA_HOME/bin:$PATH"
    GHIDRA=/c/Users/talne/ghidra_12.1.3_PUBLIC_20260817/ghidra_12.1.3_PUBLIC

    # one-time import + auto-analysis (creates ghidra_project/ChessProject)
    "$GHIDRA/support/analyzeHeadless.bat" \
      "C:\Users\talne\Desktop\ClaudeCodePicoCalc\chess-reverse\ghidra_project" ChessProject \
      -import "C:\Users\talne\Desktop\ClaudeCodePicoCalc\chess-reverse\original\CHESS.EXE"

    # re-run the export any time (after adding comments/labels in the GUI, etc.)
    "$GHIDRA/support/analyzeHeadless.bat" \
      "C:\Users\talne\Desktop\ClaudeCodePicoCalc\chess-reverse\ghidra_project" ChessProject \
      -process "CHESS.EXE" -noanalysis \
      -scriptPath "C:\Users\talne\Desktop\ClaudeCodePicoCalc\chess-reverse\notes\ghidra_scripts" \
      -postScript ExportChess.java

## Open questions / next steps

1. Run the program under DOSBox-X to see the board layout, piece glyphs,
   and cursor UI first-hand — will make correlating disasm to behavior far
   faster than pure static reading.
2. Work through the 38 functions in `chess_decompiled.c` against
   `chess_disasm.txt`, in call order starting from `entry`, naming/
   commenting them in the Ghidra GUI as their purpose becomes clear
   (init, board draw, human input/DEL-INS handling, move generator, board
   evaluation, search/minimax, computer move display, check/mate
   detection).
3. Figure out the board's in-memory representation (likely an 8x8 or
   0x88-style byte array in the DS-relative workspace segment) and the
   piece encoding.
4. Decide the C rewrite's structure up front, consistent with Thomas's
   standing hardware-independence convention: a portable core (board
   state, move generation, evaluation, search) plus a thin console I/O
   layer (cursor positioning, key input) that's the only part that would
   need to change to run somewhere other than a PC.

## Board representation (confirmed)

10x12 padded "mailbox" board, 120 bytes at workspace-segment offset 0xd1
(indices 0..119, playable interior indices ~0x15..0x62 stepping through
FUN_1000_036e's loop bounds). Encoding per square byte:

- `0xFF` = off-board sentinel (border squares, never touched by move gen)
- `0x00` = empty, playable square
- bits 0-6 = piece type (1 = pawn, others TBD as we map more code)
- bit 7 = color (0 = white, 1 = black)

Standard-issue 1970s/80s padded-mailbox trick: move generation can walk off
the real board and just check for 0xFF instead of doing range checks.

Back-rank piece-order template lives at workspace offset 0xc9 (8 bytes,
presumably R N B Q K B N R or similar - not yet decoded byte-by-byte).
White back rank -> offset 0xe6, White pawns -> 0xf0, ranks 3-6 (0xfa, 0x104,
0x10e, 0x118) cleared to empty, Black pawns -> 0x122 (= white pawn code |
0x80), Black back rank -> 0x12c (= template byte | 0x80).

Four bytes at 0x179-0x17c initialized to 0x19, 0x5f, 0x18, 0x5e. In mailbox
coordinates (index = row*10+col) these are E1, E8, D1, D8. 0x179/0x17a look
like a king-position cache (white king, black king) for fast check
detection - a common optimization. Purpose of 0x17b/0x17c (D1/D8) not
identified yet - guessed queen-square cache but unconfirmed; needs a
cross-reference check (who reads 0x17b/0x17c).

## Function map so far (by entry-point call order)

- `entry` (0448) - startup: relocate DS/ES to workspace segment, print
  title banner, call ask-color-and-depth, call board-init, select video
  page, draw board, then loop human-turn/computer-turn alternating on
  flag 0x9ce (bit7 = computer's color) and turn counter 0x9d4.
- `FUN_1000_0964` - **ask color + depth**: prompts "WHICH COLOR DO YOU
  SELECT (W/B)?" (sets 0x9ce = 0x80 if human picked... actually sets 0x9ce
  from the *reply* - re-check polarity when we look at entry's use of it)
  and "WHICH LOOK AHEAD LEVEL DO YOU SELECT (1-6)?" (stores digit 1-6,
  validated, at 0x9d5).
- `FUN_1000_090e` - **board init**, see above.
- `FUN_1000_0386` - **draw one board square's piece/empty state**,
  writes directly to B800:0000 (CGA video segment) using bit-rotate +
  XOR against a per-piece glyph table at (workspace-relative) 0xa8c+.
  DX=0x80 in / erase signal. Cosmetic only - not needed for the C rewrite's
  logic, will be replaced by a plain-text board renderer.
- `FUN_1000_036e` - **draw whole board**: loops all mailbox indices
  0x15..0x62, skips 0xFF (off-board) squares, calls FUN_1000_0386 for the
  rest.
- `FUN_1000_0298` - **human turn handler**: patches 'W'/'B' and depth
  digit into message templates, gets DOS time (move clock), redraws
  board/prompt, waits for DEL/INS pick-up-place input (via FUN_1000_09b4 /
  FUN_1000_052a), applies the move (FUN_1000_0e46?), checks for check via
  FUN_1000_08ec and prints "CHECK " if true.
- `FUN_1000_0bf2` - **computer turn handler** (not yet examined in
  detail - next up, along with the search/eval routines it calls).

## Design decision for the C rewrite

Keep the actual chess engine (board representation, move generation,
check/mate detection, search/evaluation) faithful to the original. Do NOT
try to bit-for-bit replicate the CGA text-mode block-graphic piece
renderer (FUN_1000_0386) - that's squarely hardware-specific IBM-PC-CGA
stuff. Replace it with a simple portable ASCII/letter board display
(K Q R B N P, or similar) behind a small display abstraction, consistent
with the standing hardware-independence convention: portable core, thin
platform-specific I/O shell.

## Confirmed by running the original in DOSBox-X

Thomas ran CHESS.EXE live: pieces render as small graphic chess-piece
shapes (not text letters), confirming FUN_1000_0386 is exactly what the
static analysis suggested - a CGA text-mode block-graphic sprite renderer.
This locks in the plan to reimplement the C version's display with a
plain portable board (letters/ASCII) rather than replicating the sprite
renderer.

## The search algorithm: FUN_1000_0606 (the big one, 130 decompiled lines)

This is the engine's core: **iterative-deepening tree search with
alpha-beta-style pruning**, over a linked-list game tree.

- Ply counter at 0x9d6, incremented each pass of the outer iterative-
  deepening loop, checked against selected depth at 0x9d5 (1-6, or the
  "special first move" case which shortcuts to FUN_1000_01c8 when it's the
  human's very first move as White and they haven't picked a search depth
  role yet - see `*(char*)0x9d4=='\x01' && (*(0x9ce)&0x80)==0` branch).
- Move-tree nodes are linked-list records: pointer/next at offset 0,
  looks like score/alpha/beta-style byte fields at offsets 1 and 2
  (compared as `pbVar8[0] < bVar6` / updates `pbVar8[1]`), a from/to
  square pair further in (matches FUN_1000_0f48's use of offsets +1/+3 as
  the move's square pair when searching this same kind of list for a
  specific move).
- FUN_1000_0b24 and FUN_1000_0ca8 are called together at the top of each
  ply AND again mid-search on backtrack - almost certainly move
  generation (FUN_1000_0b24 likely "generate pseudo-legal moves for the
  side to move", FUN_1000_0ca8 likely "score/order them" or "generate
  captures" - not yet disambiguated, need to read both).
- FUN_1000_004c (small, 28 lines) is called at each node comparison point
  - very likely the alpha-beta bound update / node scoring helper.
- FUN_1000_09b4 = **make move** (apply to board) - now confirmed by
  context (called before check-testing a candidate).
- FUN_1000_08ec = **is side to move in check** - confirmed by context
  (called right after making a move, to test legality / detect check or
  mate at the search horizon).
- FUN_1000_0ec2 = **unmake/undo move** - confirmed by context (called
  paired with 09b4 as the tree is walked and backtracked).
- Depth is shortened by 2 (`*(0x9d5) -= 2`) when a forced mate is found
  at the current ply and it's the top of the tree (`(short)*(0x17e)==-1`
  check) - looks like an early-exit optimization once mate is proven, to
  stop searching deeper variations pointlessly.
- The `INT 0x10` call once per node (right before FUN_1000_0e46 at the
  bottom of the main loop) is almost certainly just advancing/redrawing
  the on-screen "WORKING LEVEL" ply counter so the search visibly
  progresses - not gameplay logic.

Next to read: FUN_1000_0b24, FUN_1000_0ca8 (move generation, likely the
other really substantial routines), and FUN_1000_004c (scoring/alpha-beta
compare helper).

## Full engine architecture (high confidence now)

Reading FUN_1000_0b24, FUN_1000_0ca8, and FUN_1000_004c against the search
driver (FUN_1000_0606) ties the whole engine together:

- **FUN_1000_0b24 = move generator.** Walks a *piece list* (NOT the board
  array) starting at 0x179 - so 0x179 is not just "king square", it's the
  start of an array of board-square-index bytes, one per piece belonging
  to the side to move, terminated by 0xFF. (Revises the earlier guess
  that 0x179-0x17c were four separate king/queen caches - they're just the
  first four slots of this piece list; need to confirm slot ordering and
  list length/layout.) For each piece: looks up its board content, then
  steps through up to 8 move directions using a per-direction delta table
  at 0x9c, calling FUN_1000_0af6 once per step ("take one step this
  direction, classify what's on the landing square" - 2=blocked by own
  piece/stop, 3=capture/stop, etc., matching sliding-piece move
  generation on a mailbox board). Valid destinations get appended to a
  candidate-move array (parallel arrays at 0x170/0x166, count at 0x9c0).
  A special case (piece type 5, i.e. probably the king) reuses the
  piece-square lookup (FUN_1000_00f8) to test move safety - almost
  certainly the castling-through-check / king-safety filter.

- **FUN_1000_0ca8 = static evaluation.** Scans the *entire* board array
  (0x15..0x62, board start 0xd1 - reuses the exact same bounds as the
  board-draw routine), sums material value per piece (with specific
  handling for pawns, bVar2<4, checking bit 0x08 - doubled/passed pawn
  flag? - and king, bVar2==6, checking bit 0x10 - castling-rights flag),
  calls FUN_1000_00f8 (piece-square positional value, shared with move
  gen's king-safety check) and FUN_1000_0f9e (looks like an
  attacker/defender or mobility counter, tracks max values into 0x9db-
  0x9dd - reads like a lightweight "most valuable attacker on this
  square" heuristic). Combines everything via two calls to the tiny
  FUN_1000_09a0 into a final signed score, stored both at 0x9d9 and into
  the *current search tree node's* byte+5 field - this is exactly the
  field FUN_1000_0606 checks as "has this node been scored yet"
  (`*(char*)(*(int*)0x9c4+5)==0`).

- **FUN_1000_004c = pop one ply / backtrack.** Flips the side-to-move
  flag (0x9cf ^= 0x80), decrements the ply counter (0x9d6) and the
  per-ply scratch-stack pointer (0x9c6, a byte-granularity stack that
  grows on descend / shrinks on backtrack inside FUN_1000_0606's main
  loop), pops a 4-byte tree-node-link record off the move-tree pointer
  (0x9c2 -= 4, restoring 0x9cc/0x9c4 from it), then calls FUN_1000_0ec2 to
  physically undo the move on the board. This is the alpha-beta
  "unmake" step.

- **FUN_1000_0606 (search driver) is called from BOTH turn handlers**,
  not just the computer's. It's shared machinery: called from
  FUN_1000_0298 (human turn) too, where it doubles as the *move
  legality validator* (generate the reply tree, confirm the human's
  chosen move doesn't leave their own king in check) as well as being
  the actual minimax/alpha-beta search when it's the computer's move to
  find. One search+move-generation core serves both purposes - a sensible
  space-saving design for an ~9KB program. The very first move of the
  game (ply counter 0x9d4==1, human plays White) short-circuits to
  FUN_1000_01c8 without searching - probably prints/pre-selects an
  opening move rather than searching from the start position.

Confirmed roles now:
- FUN_1000_09b4 = make move (apply to board)
- FUN_1000_0ec2 = unmake move (undo on board)
- FUN_1000_08ec = side-to-move-in-check test
- FUN_1000_0f48 = look up a proposed move in the already-generated move
  list (used to validate/locate the human's DEL/INS-entered move)
- FUN_1000_0af6 = step one square in a direction and classify the result
  (empty / own piece / capture / off-board) - the primitive move-gen uses
- FUN_1000_00f8 = piece-square positional value lookup (table-driven,
  shared between evaluation and the king-safety special case in move gen)
- FUN_1000_0f9e = attacker/defender tracking helper used during evaluation
- FUN_1000_09a0 = small final-score combining helper (evaluation tail)

## Remaining unknowns worth resolving before writing the C rewrite

1. Exact piece-type numeric encoding (1=pawn confirmed; need king=?,
   queen=?, rook=?, bishop=?, knight=? - probably 2-6 in some order,
   readable from the back-rank template table at workspace offset 0xc9
   and the move-direction-count table referenced in FUN_1000_0b24).
2. The 8-direction delta table at workspace offset 0x9c and how it's
   indexed per piece type (mailbox offsets like -1,+1,-10,+10,-9,-11,+9,+11
   are the classic 10-wide-mailbox deltas - need to confirm the actual
   byte values and per-piece direction counts).
3. Full layout/length of the piece list starting at 0x179 (slot order,
   how many slots per side, how captures free a slot).
4. Exact material/positional scoring weights (constants inside
   FUN_1000_0ca8 and the table FUN_1000_00f8 reads from).
5. GIOQHKMPRS string - still unexplained; worth a cross-reference check
   once piece-type encoding (item 1) is nailed down, since this looks
   piece-letter-ish (G I O Q H K M P R S - odd assortment though, doesn't
   read as a clean KQRBNP set, could be coincidental data rather than a
   real piece-letter table).

## SOLVED: the "second segment" mystery, and the piece/direction tables

**The segment puzzle**: DS/ES are NOT relocated to some far-away 68KB-beyond
workspace as I originally guessed from raw arithmetic. The relocation fixup
adds `0xFF` *paragraphs* (0xFF0 = 4080 bytes) to the load segment, not
`0x10FF` paragraphs - I misread the immediate `MOV AX,0x10FF` as "the whole
delta" when it's actually "Ghidra's chosen code-segment base (0x1000) plus
a 0xFF-paragraph data-segment offset". Ghidra's own MZ loader resolved this
correctly and created two memory blocks: `CODE_0` at `1000:0000-1000:0fef`
(exactly 0xFF0 bytes - the code segment) and `CODE_1` at `10ff:0000` (the
data/workspace segment, starting immediately after). **This is a normal
small-model DOS EXE with a separate CS and DS, physically contiguous in the
same file image** - not two disjoint far-apart regions. Confirmed
empirically by dumping real, sensible, non-garbage bytes at `10ff:xxxx` via
a Ghidra script (`notes/ghidra_scripts/DumpMemory.java`) reading straight
from Ghidra's own resolved memory map - much more reliable than hand
re-deriving file offsets.

### Piece-type codes (confirmed via the back-rank template at ws+0xc9)

    ws+0xc9: 04 02 03 05 06 03 02 04  =  R  N  B  Q  K  B  N  R

Standard back rank, byte-for-byte. So:

    1 = Pawn, 2 = Knight, 3 = Bishop, 4 = Rook, 5 = Queen, 6 = King

(Pawn=1 was already known from the board-init code; this nails down the
rest. Bit 7 of the square byte is still the color flag, 0=white/1=black -
so e.g. a black knight = 0x82.)

### Move-direction system (workspace offsets 0x9c-0xc1)

A single 24-byte pool of signed mailbox-deltas at ws+0x9c:

    index:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
    value:  9  11 -11  -9  10 -10   1  -1 -21 -12   8  19  21  12  -8 -19  10  10  11   9 -10 -10 -11  -9

Plus two 7-byte tables indexed by piece type (0-6, 0 unused/never looked
up in practice):

    offset table (ws+0xb4): 20 16  8  0  4  0  0
    count  table (ws+0xbb):  4  4  8  4  4  8  8

Slicing the pool by (offset,count) per type gives exactly the right move
directions for every piece, cross-checked numerically:

| type | piece  | offset | count | deltas |
|------|--------|--------|-------|--------|
| 1 | Pawn   | 16 | 4 | 10, 10, 11, 9 (push, push-again, capture-right, capture-left; sign/color handled elsewhere via the `& 0x80` XOR-negate pattern seen throughout) |
| 2 | Knight |  8 | 8 | -21, -12, 8, 19, 21, 12, -8, -19 (the classic 8 L-jumps on a 10-wide mailbox) |
| 3 | Bishop |  0 | 4 | 9, 11, -11, -9 (the 4 diagonals) |
| 4 | Rook   |  4 | 4 | 10, -10, 1, -1 (the 4 orthogonals) |
| 5 | Queen  |  0 | 8 | all 8 (diagonals + orthogonals) |
| 6 | King   |  0 | 8 | same 8 as queen - sliding-vs-single-step (king takes exactly one step, queen slides until blocked) is handled by separate logic elsewhere, not by this table |

This is read via `FUN_1000_0a30` (`MOV CH,[BX+0xbb]` = count, `MOV BL,[BX+0xb4]` =
pool offset, then indexes into the pool at ws+0x9c) - so `FUN_1000_0a30`
is confirmed as a **per-piece "try all my move directions" driver**, calling
`FUN_1000_0af6` (step-one-square-and-classify) for each direction in turn.

### Other workspace contents identified in the same dump

- ws+0x00-0x0c: a small stray code stub (NEG/ADD/CMP/JZ/MOV/JMP/RET) -
  unexplained, low priority, may be a leftover thunk.
- ws+0x0d-0x2f: a handful of what look like near-address/pointer-ish word
  pairs - not yet decoded, low priority.
- ws+0x43 onward: the message-string table (`$CHECK $COMPUTER(W): WORKING
  LEVEL X $...` etc, matching strings.txt) - confirms messages live in the
  data segment too, not just the code segment (the CHESS88/DON BERG title
  banner and a couple of others are elsewhere, presumably still in CS,
  since they print before DS is repointed - worth double-checking which
  segment each string table belongs to before writing the C rewrite's
  string table, but not blocking).

### Revised understanding of ws+0x179-0x17c

Confirmed by the same dump as **fixed single-value caches, not a growing
piece list**: `19 5f 18 5e ff 00 00 00` sitting right where
`FUN_1000_090e` writes them (E1, E8, D1, D8 in mailbox coordinates) and
nothing after byte 4 is ever touched. Best guess remains **White king,
Black king, White queen, Black queen square caches** (used for fast
lookups - e.g. check detection or evaluation - without a full board scan).
This revises (back to) the earlier, simpler reading; the intermediate
"piece list" theory from reading `FUN_1000_0b24` was likely wrong -
`FUN_1000_0b24`'s exact role (why it reads *this* 4-byte array rather than
scanning pieces generally) is still open and worth revisiting given
`FUN_1000_0a30`/`FUN_1000_0af6` now look like the *real* general move
generator primitives.

## Scoring weights found in FUN_1000_0ca8 (evaluation)

Traced the raw disassembly (0cdd-0d18) precisely against the decompiled
pseudocode. Every square's development/castling contribution to the
material accumulator (workspace 0x9da) resolves to concrete integers:

- **Undeveloped Knight or Bishop** (still on its starting square - bit
  0x08 of the square byte clear, meaning "hasn't moved yet"): **-2**
  points for the side that owns it (a genuine "develop your minor pieces"
  penalty).
- **Rook or Queen that has moved** (bit 0x08 set), or a **King that has
  moved without castling** (reaches the same shared code path when the
  "has castled" bit 0x10 is clear): **-2** points for the side that owns
  it (a "don't move your queen/rook early, and castle promptly" penalty -
  the King-hasn't-castled-but-has-moved case forfeits castling rights,
  which this treats as equally bad).
- **King that has successfully castled** (bit 0x10 set on the king's
  square byte): **+6** points bonus for that side.

All three share one code path (LAB_1000_0d0f) that applies the magnitude
(2 or 6) with sign = -1 for white, +1 for black - i.e. the accumulator is
consistently "positive favors white," to be flipped by side-to-move
later.

Bit meanings on a piece's square byte, now fully decoded by tracing
FUN_1000_09b4 (make-move) bit-setting logic against the evaluator's
bit-testing logic:

- bit 0x08 = "this piece has moved at least once" (set unconditionally on
  every non-special move in FUN_1000_09b4's default path)
- bit 0x10 = "this king has castled" (set specifically when make-move
  processes a move flagged as castling - see move-flag bits below)
- bit 0x20 (on a *move record*, not a square) = promotion flag - seen in
  FUN_1000_09b4 forcing the piece type to 5 (Queen) before placing it
  (auto-queen promotion, no underpromotion choice)
- bit 0x40 (on a move record) = "this move has a companion move record
  immediately following it" - the castling rook's own move, applied in
  the same make-move pass right after the king's
- the low 3 bits of a move record's flags byte double as "captured piece
  type" (0 = no capture) - confirmed by FUN_1000_09b4 checking for
  captured-type==5 (queen) specifically to clear the captured queen's
  square-cache entry at ws+0x17b/0x17c

## FUN_1000_00f8: not a flat piece-value table - a ray-cast exposure/mobility scanner

Revised finding: there is **no simple flat material-value table** (like a
plain "pawn=1, knight=3, bishop=3, rook=5, queen=9" array) found anywhere
in the workspace data. Material/positional scoring for non-flag-based
value comes from `FUN_1000_00f8`, which:

- Starts a distance counter at 16 (`iVar2=0x1000`, high byte used as the
  remaining-distance value) and calls `FUN_1000_0af6` (the shared
  step-and-classify primitive) repeatedly along a pre-selected direction,
  walking outward from the piece's square.
- Branches on distance thresholds **9** and **13** (0xd) - almost
  certainly concentric "zone" boundaries (near/mid/far) around whatever
  square is being probed from, consistent with a king-safety/exposure
  scan rather than plain mobility counting.
- Has piece-type-specific rules (checking `*(char*)0x9bc` - the
  type of whatever's encountered along the ray - against 1, 2, 3, 4, 5, 6
  with different distance-threshold cutoffs per type) for what happens
  when a piece is found at a given distance.
- Feeds results into a 7-word (14-byte) table at ws+0x149, later reduced
  via `table[0] - table[7]` (ws+0x149 minus ws+0x157) into a single
  signed delta added to the material accumulator - and the same
  `[0x149]-[0x157]` reduction pattern is reused in the king-safety special
  case inside the move generator (FUN_1000_0b24), suggesting one shared
  "how exposed/mobile is a piece at this square" primitive used in two
  different evaluation contexts.
- `FUN_1000_09a0` (called twice at the very end of FUN_1000_0ca8) is a
  small **clamp helper**: `AL = min(|AL|, CH)`, sign of AL preserved -
  not a scoring weight itself, just caps a magnitude before combining it
  into the final score.

This is a materially different (and more sophisticated) evaluation
design than a classic material+piece-square-table engine: piece "value"
here appears to emerge from mobility/exposure ray-casts rather than fixed
per-type constants. Fully decoding FUN_1000_00f8's exact point
contributions per distance/piece-type combination would take significant
further work (it's dense, ~65 decompiled lines of nested distance/type
branching) and is the natural next target if exact-weight fidelity in the
C rewrite matters; otherwise a reasonable approximation (standard
material values + a simplified mobility term) would capture the spirit
without matching Don Berg's exact numbers square-for-square.

## FUN_1000_00f8 CRACKED: it's a whole-board space/control scan, not king-safety

Full trace of FUN_1000_00f8 + its helper FUN_1000_007a against the raw
disassembly (not just decompiled pseudocode, which obscured the loop
structure). Corrects the earlier "king-safety scan" guess.

### The scan itself (FUN_1000_00f8)

Outer loop runs **16 times** (`CH` counts down from 0x10=16), indexing
`DI` into the direction table at ws+0x9c:

- Iterations 1-8 (`CH` 16..9, `DI` 0..7): the 8 **sliding** king/queen
  deltas from ws+0x9c. For each, step repeatedly (`FUN_1000_0af6`) while
  landing on empty squares - `CMP CH,9 / JNC` is exactly "am I still in
  the sliding-direction range" and is what makes these rays keep going;
  stop on a piece (own or enemy) or the edge.
- Iterations 9-16 (`CH` 8..1, `DI` 8..15): the 8 **knight** jump deltas
  (same table, indices 8-15). Each is a single probe, no sliding (`CH<9`
  fails the "keep going" check).

So this is a combined "everything that could see or knight-hop to this
square" sweep - 8 rook/bishop/queen-style rays plus 8 knight offsets, from
whatever square is currently the scan origin (ws+0x9b6, set by the
caller).

### The accumulator (FUN_1000_007a), called for every piece found along the sweep

- Picks an output table by the FOUND piece's color: **ws+0x149 for white,
  ws+0x157 for black** - two parallel 7-word (14-byte) tables, not one
  table read twice as the decompiler's flat pseudocode made it look.
- `INC byte[table_base]` - this is `table[0]`'s LOW BYTE, which real piece
  types (1-6) never index into (word slots are addressed as
  `table_base + 2*type`, and type 0 is never a real piece) - so `table[0]`
  purely accumulates **a count of pieces found**, of that color, during
  this whole 16-direction sweep.
- Separately, `table[type]` (the word slot for the found piece's actual
  type) gets a value derived from a **piece-value table at ws+0xc2**,
  indexed by type - confirmed by direct memory dump:

      ws+0xc2:  00 01 03 03 05 09 0a
      index:     0  1  2  3  4  5  6
                    P  N  B  R  Q  K

  i.e. **Pawn=1, Knight=3, Bishop=3, Rook=5, Queen=9, King=10** - the
  standard textbook values (the King's "10" is just an arbitrary
  highest-priority marker, not a real trade value). This table exists and
  is genuine, but per-type slots aren't what feeds the score in the
  general per-square case (see below) - they're populated for the
  separate `ws+0x9ba==7` "sentinel" scan mode (used for actual king-safety
  checks elsewhere, e.g. inside the castling-safety special case in
  FUN_1000_0b24), which hasn't been fully traced bit-for-bit.
- There's an exclusion check at the top of FUN_1000_007a: if the square
  found is already one of the current piece's own just-generated legal
  destinations (matched against the candidate-move list FUN_1000_0b24
  built, comparing the recorded direction with a sign-flip allowance),
  the finding is skipped entirely - avoids double-counting a piece's own
  reachable squares. Not implemented in the C rewrite (see below).

### What FUN_1000_0ca8 actually does with it

The call to FUN_1000_00f8 in the evaluator runs **unconditionally for
every non-offboard square on the board - occupied AND empty** (it's after
all the development-bonus branches, with no piece-type guard). The result
consumed is just `byte[0x149] - byte[0x157]`: **white-pieces-found minus
black-pieces-found**, added straight into the material accumulator
(0x9da) for every one of the 64 squares.

**Conclusion: this is not a king-safety heuristic and not a material
value sum. It's a whole-board space/control metric** - "for every square,
who has more pieces that can reach it." A genuinely different, and
somewhat unusual for the era, design choice from classic material +
piece-square-table evaluation. The confirmed value table (1,3,3,5,9,10)
is real but belongs to a separate, still-only-partially-traced
king-safety-specific code path, not to this general term.

### C rewrite implementation

Implemented in `eval.c` as `scan_exposure()` - the same 8-ray + 8-knight
sweep, returning a (white_count, black_count) pair. Deliberately scoped
down from the original for performance: only run from **occupied**
squares rather than all 64 (evaluate() is called at every search leaf, so
a full 64-square sweep per call would be considerably more expensive) -
the count difference is added to the score exactly as in the original.
Not implemented: the own-move exclusion filter, and the separate
`0x9ba==7` king-safety-specific accumulation path (still not fully
traced). Both are candidates for a later, closer pass if exact fidelity
ever matters more than it does now.

## Piece graphics extracted: FUN_1000_0386's glyph table

Full trace of the CGA piece-sprite renderer (FUN_1000_0386) plus a direct
memory dump and visual render confirms the sprite data completely.

- Glyph table lives at workspace offset **0xa8c**, 12 glyphs of 120 bytes
  each (1440 bytes total) - confirmed by dumping 1600 bytes and rendering
  every 120-byte chunk as a bitmap; the 13th chunk is garbage/unrelated
  data, confirming the table is exactly 12 glyphs.
- Each glyph is a **40x24 pixel, 1-bit-per-pixel bitmap**, 5 bytes per row
  (40 bits), 24 rows, MSB-first bit order within each byte (confirmed by
  the rendered image being immediately recognizable, no bit-order fudging
  needed).
- Glyph order is exactly **Pawn, Knight, Bishop, Rook, Queen, King**
  (matching the piece-type numbering 1-6 already confirmed from the
  back-rank template) - each type stored as **two glyphs**: a solid black-
  filled version first, then a white-outline version. So the index is
  `(type-1)*2 + (0 for black-filled, 1 for white-outline)`.
- The original's rendering (24x40 sliding through interlaced CGA
  even/odd-scanline banks at B800:0000/B800:2000, XOR'd against the
  square's background pattern, with 1bpp source bits expanded to 2bpp for
  CGA's 4-color graphics mode) is pure IBM-PC-CGA plumbing and was NOT
  reimplemented - only the logical 40x24 bitmap *data* was extracted. It
  now lives in `src/piece_glyphs.h`/`.c` as plain data (no drawing code),
  drawn through a new generic pixel interface `src/gfx.h` by
  `src/board_render.c` (portable board layout logic) and
  `src/gfx_ppm.c` (the one backend that exists - writes a `.ppm` image
  file, so it can be viewed without real display hardware). See the
  README's "Graphics (chess_gfx target)" section for how the pieces fit
  together and how to port to real hardware.
- Raw dump preserved at `notes/glyph_raw_hex.txt` (1600 bytes from
  ws+0xa8c), the parser at `notes/parse_hex.py`, and the identification
  render at `notes/glyph_raw_hex_preview.png` (generated by
  `notes/decode_glyphs.py`) - kept for anyone wanting to re-derive
  `src/piece_glyphs.c` from scratch. Gotcha hit and fixed while building
  this: Ghidra's headless logger re-prefixes `INFO ... DumpMemory.java> `
  on some internal line flushes of a single multi-line `println()`, not
  just the first line - a naive "whole line must be pure hex" parser
  silently drops those lines and misaligns every byte after them.
  `parse_hex.py` strips the marker per-line instead of requiring a clean
  whole-line match.
