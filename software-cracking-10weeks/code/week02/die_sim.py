# -*- coding: utf-8 -*-
"""
02_die_sim.py — Mô phỏng Detect It Easy (DIE): nhận diện loại file & dấu hiệu compiler.

Mục tiêu: minh hoạ cách DIE đọc magic bytes và đặc trưng header để đoán
          file là PE/ELF/Mach-O và compiler có thể là gì.
Đầu vào : đường dẫn một file (chỉ ĐỌC, không thực thi).
Đầu ra : loại file, kiến trúc, gợi ý compiler.
An toàn : chỉ đọc vài byte đầu, không chạy binary, không ghi đè file.
"""
import sys

# Magic bytes nhận diện định dạng.
MAGIC = {
    b"MZ": ("PE (Windows)", "x86/x64 — xem thêm PE header"),
    b"\x7fELF": ("ELF (Linux)", "x86/x64/ARM — xem e_machine"),
    b"\xcf\xfa\xed\xfe": ("Mach-O (macOS 32-bit)", "little-endian"),
    b"\xfe\xed\xfa\xcf": ("Mach-O (macOS 32-bit)", "big-endian"),
    b"\xcf\xfa\xed\xfe\x0c": ("Mach-O 64-bit (arm64/x86_64)", "little-endian"),
}


def detect_magic(data: bytes) -> str:
    """Trả về mô tả định dạng dựa trên magic bytes."""
    for magic, desc in MAGIC.items():
        if data.startswith(magic):
            return f"{desc[0]} — {desc[1]}"
    return "Không nhận diện được (file text, script, hoặc định dạng khác)"


def guess_compiler(data: bytes) -> str:
    """Gợi ý compiler từ các chuỗi thường thấy trong binary (chỉ minh hoạ)."""
    # Chỉ quét một phần đầu để tránh đọc toàn bộ file lớn.
    sample = data[:1_000_000]
    hints = [
        (b"MinGW", "MinGW (GCC cho Windows)"),
        (b"GCC:", "GCC"),
        (b"MSVC", "Microsoft Visual C++"),
        (b".NETFramework", ".NET (C#/VB.NET)"),
        (b"UPX", "Có thể bị pack bằng UPX"),
        (b"Delphi", "Delphi/Borland"),
    ]
    found = [name for sig, name in hints if sig in sample]
    return ", ".join(found) if found else "Không rõ (thử DIE thật để xác nhận)"


def main() -> None:
    if len(sys.argv) < 2:
        print("Cách dùng: python die_sim.py <file>")
        sys.exit(1)

    path = sys.argv[1]
    try:
        with open(path, "rb") as f:
            data = f.read()
    except OSError as e:
        print(f"Lỗi đọc file: {e}")
        sys.exit(1)

    print(f"File      : {path}")
    print(f"Kích thước: {len(data)} bytes")
    print(f"Định dạng : {detect_magic(data)}")
    print(f"Compiler  : {guess_compiler(data)}")


if __name__ == "__main__":
    main()
