#!/usr/bin/env bash
# ============================================================
# launchagent_scan.sh — Quét thư mục LaunchAgents
# Mục tiêu: liệt kê các LaunchAgent của user (phát hiện persistence).
# Đầu vào: không.
# Đầu ra: danh sách plist LaunchAgent.
# An toàn: chỉ đọc; chạy trong VM lab.
# ============================================================
set -u

DIR="${HOME}/Library/LaunchAgents"
echo "=== Quét $DIR ==="
if [ -d "$DIR" ]; then
  find "$DIR" -name '*.plist' -maxdepth 1 2>/dev/null | while read -r f; do
    echo "--- $f"
    plutil -p "$f" 2>/dev/null | head -10
  done
else
  echo "Không có thư mục LaunchAgents của user."
fi

echo
echo "=== launchctl list (một phần) ==="
launchctl list 2>/dev/null | head -20 || echo "không chạy được launchctl"
