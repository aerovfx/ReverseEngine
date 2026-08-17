# -*- coding: utf-8 -*-
"""
08_patcher.py — Vá byte trên BẢN SAO của file, có backup + hash trước/sau.

Mục tiêu: minh hoạ quy trình patch an toàn (backup, hash, vá, xác minh).
Đầu vào : --file <file> --offset <hex> --bytes <hex> [--out <file>]
Đầu ra : file bản vá mới + in SHA-256 trước/sau.
An toàn : KHÔNG sửa file gốc; chỉ tạo bản sao đã vá.
"""
import argparse
import hashlib
import os
import shutil


def sha256(path: str) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def patch_copy(src: str, offset: int, patch: bytes, out: str) -> None:
    """Tạo bản sao và ghi patch vào bản sao (không đụng file gốc)."""
    shutil.copyfile(src, out)  # backup = bản sao trước khi sửa
    with open(out, "r+b") as f:
        f.seek(offset)
        f.write(patch)


def main() -> None:
    p = argparse.ArgumentParser(description="Vá byte trên bản sao, an toàn.")
    p.add_argument("--file", required=True, help="File gốc (chỉ đọc).")
    p.add_argument("--offset", required=True, help="Offset cần vá (hex).")
    p.add_argument("--bytes", required=True, help="Chuỗi byte ghi vào (hex, ví dụ 9090).")
    p.add_argument("--out", help="Đường dẫn file bản vá (mặc định: <file>.patched).")
    args = p.parse_args()

    offset = int(args.offset, 16)
    patch = bytes.fromhex(args.bytes)
    out = args.out or args.file + ".patched"

    print(f"Hash TRƯỚC ({args.file}): {sha256(args.file)}")
    patch_copy(args.file, offset, patch, out)
    print(f"Đã vá {len(patch)} byte tại offset 0x{offset:X} -> {out}")
    print(f"Hash SAU  ({out}): {sha256(out)}")
    print("\nLưu ý: chỉ vá bản sao; giữ nguyên file gốc để rollback.")


if __name__ == "__main__":
    main()
