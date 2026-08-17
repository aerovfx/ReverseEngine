# -*- coding: utf-8 -*-
"""
10_serial_check.py — Thuật toán kiểm tra serial + keygen (minh hoạ, cho app của bạn).

Mục tiêu: cho thấy vì sao kiểm tra serial client-side dễ bị reverse; từ đó
          hiểu cách bảo vệ (chuyển kiểm tra lên server, dùng chữ ký số).
Đầu vào : --check <serial> (kiểm tra) hoặc --keygen <name> (sinh serial).
Đầu ra : kết quả kiểm tra hoặc serial sinh ra.
An toàn : chỉ minh hoạ thuật toán trên dữ liệu giả, không liên quan phần mềm thật.
"""
import argparse
import hashlib


def derive_serial(name: str) -> str:
    """Sinh serial theo tên (thuật toán đơn giản chỉ để minh hoạ)."""
    h = hashlib.sha256(("RE-" + name.upper()).encode()).hexdigest()
    return "RE-" + h[:8].upper()


def check_serial(name: str, serial: str) -> bool:
    """Kiểm tra serial có khớp tên hay không."""
    return serial.upper() == derive_serial(name)


def main() -> None:
    p = argparse.ArgumentParser(description="Minh hoạ kiểm tra serial + keygen.")
    p.add_argument("--check", nargs=2, metavar=("NAME", "SERIAL"), help="Kiểm tra serial.")
    p.add_argument("--keygen", metavar="NAME", help="Sinh serial hợp lệ cho tên.")
    args = p.parse_args()

    if args.keygen:
        print(f"Name   : {args.keygen}")
        print(f"Serial : {derive_serial(args.keygen)}")
    elif args.check:
        name, serial = args.check
        ok = check_serial(name, serial)
        print(f"Name   : {name}")
        print(f"Serial : {serial}")
        print(f"Kết quả: {'HỢP LỆ' if ok else 'KHÔNG hợp lệ'}")
    else:
        p.print_help()

    print("\nBài học: thuật toán nằm ngay trong client thì ai cũng reverse được.")
    print("Phòng thủ: kiểm tra phía server, ký số serial, chống debug (anti-tamper).")


if __name__ == "__main__":
    main()
