# -*- coding: utf-8 -*-
"""
10_harden_check.py — Checklist đánh giá hardening sandbox.

Mục tiêu: cho khung đánh giá mức độ an toàn của một sandbox agent.
Đầu vào : không (demo); đánh dấu đạt/không bằng --passed "1,3,5".
Đầu ra : bảng trạng thái hardening theo tầng.
An toàn : chỉ đánh giá theo checklist, không thao tác hệ thống.
"""
import argparse

CHECKS = [
    ("Cô lập", "MicroVM (KVM), mỗi sandbox kernel riêng"),
    ("Seccomp", "VMM bị giới hạn syscall tối thiểu"),
    ("Storage CoW", "Snapshot/clone O(1), rollback nhanh"),
    ("Mạng", "Network policy default-deny"),
    ("Egress", "Domain allowlist + TLS inspection"),
    ("Secret", "Credential injection, secret không vào sandbox"),
    ("Vận hành", "Auto-pause, audit log, giới hạn tài nguyên"),
]


def main():
    p = argparse.ArgumentParser(description="Checklist hardening sandbox.")
    p.add_argument("--passed", default="", help="Danh sách chỉ số đạt (vd: 1,2,3).")
    args = p.parse_args()
    passed = {int(x) for x in args.passed.split(",") if x.strip().isdigit()}

    print("Hardening checklist:\n")
    for i, (layer, desc) in enumerate(CHECKS, 1):
        mark = "✅" if i in passed else "⬜"
        print(f"  {mark} {i}. {layer:<14} {desc}")

    score = len(passed)
    print(f"\nĐạt {score}/{len(CHECKS)} tầng.")
    print("Mục tiêu: nếu agent bị prompt injection, kẻ tấn công gần như không làm được gì.")


if __name__ == "__main__":
    main()
