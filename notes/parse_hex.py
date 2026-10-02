"""Shared, careful parser for the Ghidra dump log: strips known log-noise
markers per line (Ghidra re-prefixes some lines with 'INFO ... DumpMemory.java> '
on internal flushes), then keeps ONLY exact 2-hex-digit whitespace-separated
tokens from what remains - avoids accidentally matching hex-looking
substrings inside words like 'GhidraScript' (e.g. 'da')."""
import re

MARKER = re.compile(r"^.*DumpMemory\.java>\s*")

def parse_hex_dump(path):
    bytes_list = []
    with open(path, encoding="utf-8") as f:
        for line in f:
            line = line.rstrip("\n")
            m = MARKER.match(line)
            if m:
                line = line[m.end():]
            if "GLYPHDATA_START" in line or "GLYPHDATA_END" in line:
                continue
            if "(GhidraScript)" in line:
                line = line.split("(GhidraScript)")[0]
            for tok in line.split():
                if re.fullmatch(r"[0-9a-fA-F]{2}", tok):
                    bytes_list.append(int(tok, 16))
                else:
                    raise ValueError(f"unexpected token {tok!r} in line {line!r}")
    return bytes_list

if __name__ == "__main__":
    b = parse_hex_dump(r"C:\Users\talne\Desktop\ClaudeCodePicoCalc\chess-reverse\notes\glyph_raw_hex.txt")
    print("total bytes:", len(b))
    print("first 20:", [f"{x:02x}" for x in b[:20]])
