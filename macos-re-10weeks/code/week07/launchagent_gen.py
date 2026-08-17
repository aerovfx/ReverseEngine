#!/usr/bin/env python3
# ============================================================
# launchagent_gen.py — Sinh LaunchAgent plist mẫu
# Mục tiêu: tạo file plist LaunchAgent để hiểu cấu trúc persistence.
# Đầu vào: tên app (mặc định com.example.app).
# Đầu ra: nội dung plist (in ra, không ghi hệ thống).
# An toàn: chỉ sinh nội dung; không tự đặt persistence trên máy thật.
# ============================================================
import sys

LABEL = sys.argv[1] if len(sys.argv) > 1 else "com.example.app"
PROGRAM = "/usr/bin/true"  # chương trình vô hại dùng cho minh hoạ


def main():
    plist = f"""<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
  "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>{LABEL}</string>
    <key>ProgramArguments</key>
    <array>
        <string>{PROGRAM}</string>
    </array>
    <key>RunAtLoad</key>
    <true/>
</dict>
</plist>
"""
    print(plist)
    print(f"(minh hoạ — KHÔNG tự ghi vào ~/Library/LaunchAgents trên máy chính)")


if __name__ == "__main__":
    main()
