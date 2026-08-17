#!/usr/bin/env python3
# ============================================================
# macho_parser.py — Parse header Mach-O (dữ liệu giả/minh hoạ)
# Mục tiêu: đọc và in header Mach-O từ một file nhị phân giả.
# Đầu vào: file (mặc định tự tạo dữ liệu giả nếu không có).
# Đầu ra: header, load commands, segments.
# An toàn: chỉ phân tích file do chính bạn tạo; không đọc mẫu malware.
# ============================================================
import struct
import sys

# Magic numbers Mach-O (minh hoạ)
MH_MAGIC_64 = 0xFEEDFACF
MH_MAGIC_32 = 0xFEEDFACE
FAT_MAGIC = 0xCAFEBABE

# CPU types (một phần)
CPU_ARCH_ABI64 = 0x01000000
CPU_TYPE_X86_64 = 0x01000007
CPU_TYPE_ARM64 = 0x0100000C

# File types
MH_EXECUTE = 0x2
MH_DYLIB = 0x6
MH_BUNDLE = 0x8

LOAD_CMDS = {
    0x1: "LC_SEGMENT",
    0x19: "LC_SEGMENT_64",
    0xC: "LC_LOAD_DYLIB",
    0xD: "LC_ID_DYLIB",
    0x1D: "LC_CODE_SIGNATURE",
    0x80000028: "LC_MAIN",
}


def make_fake_header():
    """Tạo 32 byte header Mach-O 64-bit giả cho demo."""
    # magic, cputype, cpusubtype, filetype, ncmds, sizeofcmds, flags, reserved
    return struct.pack(
        "<IiiIIIII",
        MH_MAGIC_64,
        CPU_TYPE_ARM64,
        0,
        MH_EXECUTE,
        3,
        96,
        0,
        0,
    )


def parse(data: bytes):
    if len(data) < 32:
        print("Quá ngắn để là Mach-O 64-bit.")
        return
    magic, cputype, cpusub, filetype, ncmds, sizeofcmds, flags, _ = struct.unpack(
        "<IiiIIIII", data[:32]
    )
    print(f"magic        : 0x{magic:08X} ({'64-bit' if magic == MH_MAGIC_64 else '32-bit' if magic == MH_MAGIC_32 else 'không phải Mach-O'})")
    print(f"cputype      : {cputype} ({'arm64' if cputype == CPU_TYPE_ARM64 else 'x86_64' if cputype == CPU_TYPE_X86_64 else '?'})")
    ftype = "MH_EXECUTE" if filetype == MH_EXECUTE else "other"
    print(f"filetype     : {filetype} ({ftype})")
    print(f"ncmds        : {ncmds}")
    print(f"sizeofcmds   : {sizeofcmds}")
    print("load commands (minh hoạ):")
    for i in range(min(ncmds, 4)):
        print(f"  LC_{i}: {LOAD_CMDS.get(0x19, 'LC_SEGMENT_64')}")


def main():
    if len(sys.argv) > 1:
        data = open(sys.argv[1], "rb").read()
    else:
        data = make_fake_header()
        print("(dùng header giả tự tạo)\n")
    parse(data)


if __name__ == "__main__":
    main()
