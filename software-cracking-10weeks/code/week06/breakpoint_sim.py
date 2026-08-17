# -*- coding: utf-8 -*-
"""
06_breakpoint_sim.py — Mô phỏng cơ chế breakpoint phần mềm (INT3 / 0xCC).

Mục tiêu: minh hoạ debugger dừng khi CPU chạm lệnh INT3 tại địa chỉ đặt BP.
Đầu vào : không (demo; có thể truyền --bp <hex> để đặt BP khác).
Đầu ra : vết thực thi và vị trí dừng.
An toàn : chỉ mô phỏng, không ghi INT3 vào binary thật.
"""
import argparse

# Chương trình giả: địa chỉ -> lệnh.
PROGRAM = [
    ("00401000", "MOV EAX, [serial]"),
    ("00401005", "CMP EAX, 0x1337"),
    ("0040100A", "JE  00401020"),   # nếu đúng -> "correct"
    ("0040100F", "PUSH 'try harder'"),
    ("00401014", "CALL MessageBox"),
    ("00401020", "PUSH 'correct'"),
    ("00401025", "CALL MessageBox"),
]


def run(breakpoints: set):
    """Chạy chương trình giả, dừng khi gặp địa chỉ đặt BP."""
    for addr, ins in PROGRAM:
        if addr in breakpoints:
            print(f"  [BP hit] dừng tại {addr}: {ins}")
            return
        print(f"  {addr}  {ins}")
    print("  (kết thúc chương trình, không chạm BP)")


def main() -> None:
    parser = argparse.ArgumentParser(description="Mô phỏng breakpoint phần mềm.")
    parser.add_argument("--bp", default="0040100A", help="Địa chỉ đặt breakpoint (hex).")
    args = parser.parse_args()

    try:
        bp = int(args.bp, 16)
    except ValueError:
        print("Địa chỉ BP phải là số hex, ví dụ 0040100A")
        return

    bp_addr = f"{bp:08X}"
    print(f"Đặt software BP (INT3) tại {bp_addr}\n")
    run({bp_addr})
    print("\nThực tế: debugger ghi 0xCC (INT3) vào địa chỉ đó, CPU dừng rồi khôi phục byte gốc.")


if __name__ == "__main__":
    main()
