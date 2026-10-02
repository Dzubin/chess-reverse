#!/usr/bin/env python3
# Parse an MS-DOS MZ EXE header and print the fields, plus write out a
# "load image" (the flat memory image DOS would have built, with the one
# relocation table's segment fixups applied against a chosen load segment).
#
# Usage: python parse_mz.py CHESS.EXE

import struct
import sys

def main():
    path = sys.argv[1]
    with open(path, "rb") as f:
        data = f.read()

    fmt = "<2s13H"
    size = struct.calcsize(fmt)
    hdr = struct.unpack(fmt, data[0:size])
    (sig, e_cblp, e_cp, e_crlc, e_cparhdr, e_minalloc, e_maxalloc,
     e_ss, e_sp, e_csum, e_ip, e_cs, e_lfarlc, e_ovno) = hdr

    print(f"signature      : {sig!r}")
    print(f"e_cblp         : 0x{e_cblp:04x} (bytes on last page)")
    print(f"e_cp           : 0x{e_cp:04x} (pages, 512 bytes each)")
    print(f"e_crlc         : {e_crlc} (relocation entries)")
    print(f"e_cparhdr      : 0x{e_cparhdr:04x} (header size, paragraphs)")
    print(f"e_minalloc     : 0x{e_minalloc:04x}")
    print(f"e_maxalloc     : 0x{e_maxalloc:04x}")
    print(f"e_ss:e_sp      : {e_ss:04x}:{e_sp:04x} (initial stack)")
    print(f"e_csum         : 0x{e_csum:04x}")
    print(f"e_cs:e_ip      : {e_cs:04x}:{e_ip:04x} (entry point, seg rel to load segment)")
    print(f"e_lfarlc       : 0x{e_lfarlc:04x} (relocation table file offset)")
    print(f"e_ovno         : {e_ovno} (overlay number)")

    header_size = e_cparhdr * 16
    image_size = e_cp * 512
    if e_cblp != 0:
        image_size -= (512 - e_cblp)
    load_module_size = image_size - header_size

    print()
    print(f"header size (bytes)      : {header_size}")
    print(f"total image size (bytes) : {image_size}")
    print(f"load module size (bytes) : {load_module_size}")
    print(f"actual file size on disk : {len(data)}")
    print(f"trailing bytes past image: {len(data) - image_size}")

    print()
    print("relocation table:")
    relocs = []
    for i in range(e_crlc):
        off = e_lfarlc + i * 4
        reloc_off, reloc_seg = struct.unpack("<HH", data[off:off + 4])
        relocs.append((reloc_seg, reloc_off))
        print(f"  [{i}] segment={reloc_seg:04x} offset={reloc_off:04x}")

    load_module = data[header_size:header_size + load_module_size]

    entry_file_offset_in_module = e_cs * 16 + e_ip
    print()
    print(f"entry point offset within load module: 0x{entry_file_offset_in_module:04x}")

    out_path = path.rsplit(".", 1)[0] + "_loadmodule.bin"
    with open(out_path, "wb") as f:
        f.write(load_module)
    print()
    print(f"wrote raw load module (header stripped, no relocation applied) to: {out_path}")

if __name__ == "__main__":
    main()
