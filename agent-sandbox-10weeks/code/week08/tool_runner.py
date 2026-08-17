# -*- coding: utf-8 -*-
"""
08_tool_runner.py — Wrapper chạy tool an toàn (whitelist + timeout).

Mục tiêu: minh hoạ cách hạn chế rủi ro khi agent chạy lệnh.
Đầu vào : không (demo); có thể truyền --cmd "..." để thử.
Đầu ra : kết quả cho phép/chặn + lý do.
An toàn : KHÔNG chạy lệnh thật — chỉ mô phỏng quyết định an toàn.
"""
import argparse
import re

# Danh sách lệnh được phép (whitelist).
ALLOWED = ["ls", "cat", "echo", "pwd", "python --version"]

# Mẫu lệnh nguy hiểm bị chặn (denylist bổ sung).
BLOCKED_PATTERNS = [r"rm\s+-rf", r"mkfs", r"dd\s+if", r">\s*/dev/"]


def evaluate(cmd):
    """Đánh giá lệnh: whitelist trước, denylist sau."""
    base = cmd.split()[0]
    if base not in ALLOWED:
        return False, f"lệnh '{base}' không nằm trong whitelist"
    for pat in BLOCKED_PATTERNS:
        if re.search(pat, cmd):
            return False, f"khớp mẫu nguy hiểm: {pat}"
    return True, "hợp lệ (chạy trong sandbox, có timeout)"


def main():
    p = argparse.ArgumentParser(description="Mô phỏng wrapper chạy tool an toàn.")
    p.add_argument("--cmd", default="ls -la", help="Lệnh cần đánh giá.")
    args = p.parse_args()

    for cmd in [args.cmd, "ls -la", "cat secret.txt", "rm -rf /", "python --version"]:
        ok, why = evaluate(cmd)
        print(f"  {'✅' if ok else '⛔'} {cmd:<22} -> {why}")

    print("\nThực tế: sandbox còn thêm timeout, giới hạn CPU/RAM, egress allowlist và audit log.")


if __name__ == "__main__":
    main()
