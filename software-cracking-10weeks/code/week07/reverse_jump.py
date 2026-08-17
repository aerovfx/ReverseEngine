# -*- coding: utf-8 -*-
"""
07_reverse_jump.py — Mô phỏng đảo nhảy điều kiện để quan sát nhánh ẩn.

Mục tiêu: minh hoạ việc đổi JE<->JNE (hoặc lật cờ ZF) làm luồng đi nhánh khác.
Đầu vào : không (demo thuật toán kiểm tra serial giả).
Đầu ra : kết quả trước và sau khi "đảo nhảy".
An toàn : chỉ mô phỏng bằng dữ liệu, không sửa binary thật.
"""

CORRECT = "RE-2024"  # serial giả cho mục đích minh hoạ


def check(user_input: str, invert: bool = False) -> str:
    """Mô phỏng: CMP input, correct; JE success. invert=True sẽ đảo quyết định."""
    equal = user_input == CORRECT
    if invert:
        equal = not equal  # đảo nhảy: JE -> JNE
    return "FLAG{bypass_ok}" if equal else "try harder"


def main() -> None:
    print(f"Serial đúng (ẩn) : {CORRECT}\n")

    for attempt in ("sai-pass", CORRECT):
        print(f"Input '{attempt}':")
        print(f"   bình thường : {check(attempt)}")
        print(f"   đảo nhảy    : {check(attempt, invert=True)}")
        print("-" * 40)

    print("Kết luận: đảo nhảy làm luồng rẽ sang nhánh 'thành công' dù input sai.")
    print("Lưu ý: đây là kỹ thuật HIỂU luồng trên crackme/binary của bạn, không dùng cho phần mềm thương mại.")


if __name__ == "__main__":
    main()
