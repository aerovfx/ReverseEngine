#!/usr/bin/env python3
# ============================================================
# plist_parse.py — Parse plist dạng XML đơn giản (không dùng thư viện ngoài)
# Mục tiêu: đọc các key/value trong plist LaunchAgent.
# Đầu vào: file plist (mặc định dùng plist giả).
# Đầu ra: danh sách key -> value.
# An toàn: dữ liệu giả / plist của chính bạn.
# ============================================================
import re
import sys

FAKE = """<?xml version="1.0"?>
<plist version="1.0"><dict>
  <key>Label</key><string>com.example.app</string>
  <key>RunAtLoad</key><true/>
  <key>ProgramArguments</key>
  <array><string>/usr/bin/true</string></array>
</dict></plist>
"""


def parse(plist: str):
    pairs = {}
    # Bắt cặp <key>...</key><type>...</type> đơn giản
    for m in re.finditer(
        r"<key>(.*?)</key>\s*<([a-z]+)>(.*?)</\2>", plist, re.S
    ):
        key, typ, val = m.group(1), m.group(2), m.group(3)
        if typ == "true":
            val = True
        elif typ == "false":
            val = False
        pairs[key] = val
    return pairs


def main():
    content = open(sys.argv[1]).read() if len(sys.argv) > 1 else FAKE
    if len(sys.argv) == 1:
        print("(dùng plist giả)\n")
    for k, v in parse(content).items():
        print(f"{k:24} = {v!r}")


if __name__ == "__main__":
    main()
