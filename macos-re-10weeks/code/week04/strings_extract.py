#!/usr/bin/env python3
# ============================================================
# strings_extract.py — Trích strings ASCII/UTF-8 từ file
# Mục tiêu: tìm chuỗi đọc được (URL, key, message) trong binary.
# Đầu vào: file.
# Đầu ra: các chuỗi dài >= MIN_LEN.
# An toàn: chỉ phân tích file do chính bạn tạo / được cấp phép.
# ============================================================
import re
import sys

MIN_LEN = 4


def extract(data: bytes):
    # ASCII printable
    for m in re.finditer(rb"[\x20-\x7e]{%d,}" % MIN_LEN, data):
        yield m.group().decode("ascii", errors="replace")


def main():
    if len(sys.argv) < 2:
        print("Cách dùng: python3 strings_extract.py <file>")
        return
    data = open(sys.argv[1], "rb").read()
    print(f"=== Strings (>= {MIN_LEN} ký tự) trong {sys.argv[1]} ===")
    for s in extract(data):
        print(s)


if __name__ == "__main__":
    main()
