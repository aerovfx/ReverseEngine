#!/usr/bin/env python3
# ============================================================
# fat_detector.py — Nhận diện universal (fat) binary
# Mục tiêu: đọc magic đầu file để nhận biết fat binary và số slice.
# Đầu vào: file.
# Đầu ra: kết luận loại binary.
# An toàn: chỉ phân tích file do chính bạn tạo; không đọc mẫu malware.
# ============================================================
import struct
import sys

FAT_MAGIC = 0xCAFEBABE
FAT_CIGAM = 0xBEBAFECA  # endian đảo


def main():
    if len(sys.argv) < 2:
        print("Cách dùng: python3 fat_detector.py <file>")
        return
    path = sys.argv[1]
    with open(path, "rb") as f:
        head = f.read(4)
    if len(head) < 4:
        print(f"{path}: quá ngắn")
        return
    magic = struct.unpack("<I", head)[0]
    if magic == FAT_MAGIC:
        with open(path, "rb") as f:
            nfat = struct.unpack("<I", f.read(4)[:4] or b"\x00\x00\x00\x00")[0]
        print(f"{path}: FAT/universal binary, {nfat} slice")
    elif magic == FAT_CIGAM:
        print(f"{path}: FAT binary (endian đảo)")
    elif magic in (0xFEEDFACF, 0xFEEDFACE):
        print(f"{path}: Mach-O thường (không fat)")
    else:
        print(f"{path}: không phải Mach-O/fat (magic 0x{magic:08X})")


if __name__ == "__main__":
    main()
