# -*- coding: utf-8 -*-
"""
05_step_sim.py — Mô phỏng step over (F8) và step into (F7) trên luồng lệnh giả.

Mục tiêu: phân biệt hai thao tác stepping trong debugger.
Đầu vào : không (chạy demo).
Đầu ra : vết thực thi theo hai chế độ step.
An toàn : chỉ mô phỏng danh sách lệnh bằng Python, không chạy binary.
"""

# Mỗi lệnh: (địa chỉ, mã lệnh, hàm chứa)
PROGRAM = [
    ("00401000", "MOV EAX, 5", "main"),
    ("00401005", "CALL 00402000", "main"),   # gọi hàm check()
    ("0040100A", "CMP EAX, 0", "main"),
    ("00402000", "PUSH EBP", "check"),        # bên trong check()
    ("00402004", "MOV EAX, 1", "check"),
    ("00402008", "RET", "check"),
]


def step_over(program):
    """Step over: khi gặp CALL, chạy trọn hàm bên trong rồi quay lại."""
    trace = []
    i = 0
    while i < len(program):
        addr, ins, func = program[i]
        trace.append((addr, ins, func))
        if ins.startswith("CALL"):
            # Nhảy qua toàn bộ phần thân hàm được gọi (không đi sâu).
            i += 1
            while i < len(program) and program[i][2] != "main":
                i += 1
            continue
        i += 1
    return trace


def step_into(program):
    """Step into: khi gặp CALL, đi sâu vào bên trong hàm được gọi."""
    return [(addr, ins, func) for addr, ins, func in program]


def main() -> None:
    print("== STEP OVER (F8) — không đi sâu vào CALL ==")
    for addr, ins, func in step_over(PROGRAM):
        print(f"  {addr}  {ins:<14} [{func}]")
    print("\n== STEP INTO (F7) — đi sâu vào bên trong CALL ==")
    for addr, ins, func in step_into(PROGRAM):
        print(f"  {addr}  {ins:<14} [{func}]")
    print("\nNhận xét: step into thấy cả thân hàm check(); step over lướt qua.")


if __name__ == "__main__":
    main()
