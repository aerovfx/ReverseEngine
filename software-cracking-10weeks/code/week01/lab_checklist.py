# -*- coding: utf-8 -*-
"""
01_lab_checklist.py — Checklist môi trường lab cracking an toàn.

Mục tiêu: sinh danh sách kiểm tra (checklist) để học viên tự xác nhận
          môi trường lab cô lập trước khi phân tích binary.
Đầu vào : không (có thể truyền --out <file> để lưu).
Đầu ra : in checklist ra màn hình, tuỳ chọn ghi ra file.
An toàn : chỉ sinh văn bản, không chạy binary, không đụng hệ thống.
"""
import argparse
import datetime


# Các hạng mục bắt buộc trước khi phân tích bất kỳ binary nào.
CHECKLIST = [
    ("Mục tiêu hợp pháp", "binary là của tôi / crackme công khai / được phép bằng văn bản"),
    ("Cô lập", "đang chạy trong VM, có snapshot 'Base'"),
    ("Mạng", "mạng VM đã tắt hoặc dùng mạng giả lập"),
    ("Hash", "đã ghi SHA-256 của binary trước khi chạy"),
    ("Backup", "đã sao lưu binary gốc trước khi vá"),
    ("RoE", "đã viết Rules of Engagement (phạm vi + rollback)"),
]


def main() -> None:
    parser = argparse.ArgumentParser(description="Sinh checklist lab cracking an toàn.")
    parser.add_argument("--out", help="Ghi checklist ra file (tuỳ chọn).")
    args = parser.parse_args()

    lines = []
    stamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    lines.append(f"# LAB CHECKLIST — {stamp}")
    lines.append("")
    for i, (muc, ghi_chu) in enumerate(CHECKLIST, 1):
        lines.append(f"[ ] {i}. {muc}: {ghi_chu}")
    lines.append("")
    lines.append("Nếu bất kỳ mục nào chưa đạt, KHÔNG chạy mẫu.")

    text = "\n".join(lines)
    print(text)

    if args.out:
        # Ghi file ở chế độ ghi đè an toàn (chỉ file văn bản do chính ta tạo).
        with open(args.out, "w", encoding="utf-8") as f:
            f.write(text + "\n")
        print(f"\nĐã ghi checklist vào: {args.out}")


if __name__ == "__main__":
    main()
