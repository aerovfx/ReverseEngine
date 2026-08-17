# -*- coding: utf-8 -*-
"""
09_winapi_sim.py — Mô phỏng luồng chương trình GUI kiểm tra serial.

Mục tiêu: minh hoạ cách GetWindowText (đọc input) và MessageBox (hiển thị)
          nối với lệnh so sánh serial trong chương trình GUI.
Đầu vào : --serial <chuỗi> (giả lập người dùng nhập).
Đầu ra : vết lời gọi API và kết quả đúng/sai.
An toàn : chỉ mô phỏng, không gọi Windows API thật.
"""
import argparse

CORRECT_SERIAL = "RE-2024"


def get_window_text(user_input: str) -> str:
    """Mô phỏng API GetWindowTextA: trả về chuỗi người dùng nhập vào ô textbox."""
    print(f"  [GetWindowTextA] đọc input = {user_input!r}")
    return user_input


def message_box(text: str) -> None:
    """Mô phỏng API MessageBoxA: hiển thị thông báo."""
    print(f"  [MessageBoxA] hiển thị: {text!r}")


def main() -> None:
    p = argparse.ArgumentParser(description="Mô phỏng GUI kiểm tra serial.")
    p.add_argument("--serial", default="sai-pass", help="Serial người dùng nhập.")
    args = p.parse_args()

    # 1. Người dùng nhập serial -> GetWindowText
    entered = get_window_text(args.serial)

    # 2. So sánh (tương ứng lệnh CMP + JE trong binary)
    ok = entered == CORRECT_SERIAL

    # 3. Hiển thị kết quả -> MessageBox
    message_box("Correct!" if ok else "Invalid serial")

    print(f"\nKết quả: {'ĐÚNG' if ok else 'SAI'} (serial đúng là {CORRECT_SERIAL!r})")
    print("Khi crack GUI: đặt BP trên GetWindowText/MessageBox rồi lần ngược tới CMP.")


if __name__ == "__main__":
    main()
