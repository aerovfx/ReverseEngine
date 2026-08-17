# -*- coding: utf-8 -*-
"""
03_pe_entry_point.py — Đọc ImageBase và AddressOfEntryPoint từ header PE.

Mục tiêu: minh hoạ cách tính entry point  EntryPoint = ImageBase + AddressOfEntryPoint
          mà không cần mở debugger.
Đầu vào : đường dẫn một file PE (chỉ ĐỌC).
Đầu ra : ImageBase, AddressOfEntryPoint, EntryPoint(VA), số section.
An toàn : chỉ đọc header, không thực thi, không ghi file.
"""
import struct
import sys


def read_pe_info(path: str) -> dict:
    with open(path, "rb") as f:
        data = f.read()

    # 1. Kiểm tra magic "MZ".
    if len(data) < 64 or data[:2] != b"MZ":
        raise ValueError("Không phải file PE (thiếu magic MZ).")

    # 2. e_lfanew nằm tại offset 0x3C, trỏ tới PE header.
    e_lfanew = struct.unpack_from("<I", data, 0x3C)[0]
    if data[e_lfanew:e_lfanew + 4] != b"PE\x00\x00":
        raise ValueError("Không tìm thấy chữ ký PE.")

    # 3. Optional header nằm ngay sau PE signature + COFF header (20 bytes).
    opt = e_lfanew + 4 + 20
    magic = struct.unpack_from("<H", data, opt)[0]  # 0x10B=PE32, 0x20B=PE32+

    # 4. Đọc ImageBase và AddressOfEntryPoint theo loại 32/64-bit.
    if magic == 0x10B:  # PE32
        image_base = struct.unpack_from("<I", data, opt + 28)[0]
        entry_rva = struct.unpack_from("<I", data, opt + 16)[0]
        num_sections = struct.unpack_from("<H", data, opt + 2)[0]
        arch = "PE32 (x86)"
    elif magic == 0x20B:  # PE32+
        image_base = struct.unpack_from("<Q", data, opt + 24)[0]
        entry_rva = struct.unpack_from("<I", data, opt + 16)[0]
        num_sections = struct.unpack_from("<H", data, opt + 2)[0]
        arch = "PE32+ (x64)"
    else:
        raise ValueError(f"Magic optional header lạ: 0x{magic:04X}.")

    return {
        "arch": arch,
        "image_base": image_base,
        "entry_rva": entry_rva,
        "entry_va": image_base + entry_rva,
        "num_sections": num_sections,
    }


def main() -> None:
    if len(sys.argv) < 2:
        print("Cách dùng: python pe_entry_point.py <file.exe>")
        sys.exit(1)

    try:
        info = read_pe_info(sys.argv[1])
    except (OSError, ValueError) as e:
        print(f"Lỗi: {e}")
        sys.exit(1)

    print(f"Kiến trúc             : {info['arch']}")
    print(f"ImageBase             : 0x{info['image_base']:08X}")
    print(f"AddressOfEntryPoint   : 0x{info['entry_rva']:08X} (RVA)")
    print(f"EntryPoint (VA)       : 0x{info['entry_va']:08X}")
    print(f"Số section            : {info['num_sections']}")
    print("\nCông thức: EntryPoint = ImageBase + AddressOfEntryPoint")


if __name__ == "__main__":
    main()
