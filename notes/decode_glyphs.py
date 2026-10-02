import zlib, struct
from parse_hex import parse_hex_dump

data = bytes(parse_hex_dump(r"C:\Users\talne\Desktop\ClaudeCodePicoCalc\chess-reverse\notes\glyph_raw_hex.txt"))
print("total bytes parsed:", len(data))

GLYPH_BYTES = 120
ROWS = 24
ROWBYTES = 5
N_GLYPHS = len(data) // GLYPH_BYTES
print("glyphs available:", N_GLYPHS)

def glyph_rows(offset):
    rows = []
    for r in range(ROWS):
        row_bits = []
        for b in range(ROWBYTES):
            byte = data[offset + r*ROWBYTES + b]
            bits = [(byte >> (7-i)) & 1 for i in range(8)]
            row_bits.extend(bits)
        rows.append(row_bits)
    return rows

def write_png(path, width, height, pixels):
    def chunk(tag, d):
        return struct.pack('>I', len(d)) + tag + d + struct.pack('>I', zlib.crc32(tag + d))
    sig = b'\x89PNG\r\n\x1a\n'
    ihdr = struct.pack('>IIBBBBB', width, height, 8, 0, 0, 0, 0)
    raw = bytearray()
    for row in pixels:
        raw.append(0)
        raw.extend(row)
    idat = zlib.compress(bytes(raw), 9)
    png = sig + chunk(b'IHDR', ihdr) + chunk(b'IDAT', idat) + chunk(b'IEND', b'')
    with open(path, 'wb') as f:
        f.write(png)

SCALE = 6
GLYPH_W = ROWBYTES * 8
PAD = 4
cell_w = GLYPH_W * SCALE + PAD*2
cell_h = ROWS * SCALE + PAD*2
cols = 4
rows_grid = (N_GLYPHS + cols - 1) // cols
img_w = cell_w * cols
img_h = cell_h * rows_grid

pixels = [[200]*img_w for _ in range(img_h)]

for gi in range(N_GLYPHS):
    rows = glyph_rows(gi * GLYPH_BYTES)
    gx = (gi % cols) * cell_w + PAD
    gy = (gi // cols) * cell_h + PAD
    for ry, row in enumerate(rows):
        for rx, bit in enumerate(row):
            color = 0 if bit else 255
            for sy in range(SCALE):
                py = gy + ry*SCALE + sy
                if py >= img_h: continue
                for sx in range(SCALE):
                    px = gx + rx*SCALE + sx
                    if px >= img_w: continue
                    pixels[py][px] = color

out = r"C:\Users\talne\AppData\Local\Temp\claude\C--Users-talne-Desktop-ClaudeCodePicoCalc-picocalc-music-maker\abf1a0c1-f660-46fc-8634-0d1c6298cf80\scratchpad\glyphs_preview2.png"
write_png(out, img_w, img_h, pixels)
print("wrote", out, img_w, img_h)
