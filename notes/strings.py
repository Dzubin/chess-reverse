#!/usr/bin/env python3
# Minimal 'strings' replacement: print runs of >=4 printable ASCII chars
# with their file offset (hex). Usage: python strings.py FILE
import sys

def main():
    path = sys.argv[1]
    minlen = int(sys.argv[2]) if len(sys.argv) > 2 else 4
    with open(path, "rb") as f:
        data = f.read()
    run = bytearray()
    start = 0
    for i, b in enumerate(data):
        if 0x20 <= b < 0x7f:
            if not run:
                start = i
            run.append(b)
        else:
            if len(run) >= minlen:
                print(f"{start:06x}  {run.decode('ascii')}")
            run = bytearray()
    if len(run) >= minlen:
        print(f"{start:06x}  {run.decode('ascii')}")

if __name__ == "__main__":
    main()
