#!/usr/bin/env bash
# ============================================================
# rechecklist.sh — Checklist RE tổng hợp
# Mục tiêu: nhắc nhở các bước khi phân tích app/mẫu.
# Đầu vào: không.
# Đầu ra: checklist.
# An toàn: chỉ đọc; chạy trong VM lab.
# ============================================================
set -u

echo "=== Checklist Reverse Engineering macOS ==="
echo "[ ] Xác định phạm vi hợp pháp (app của mình / được cấp phép / mã nguồn mở)"
echo "[ ] Chạy trong VM cô lập, snapshot trước khi chạy mẫu"
echo "[ ] Nhận dạng: file, shasum -a 256, otool -hv"
echo "[ ] Static: strings, nm, class-dump, Ghidra, codesign/entitlements"
echo "[ ] Dynamic: LLDB, Frida, fs_usage, log stream, Wireshark"
echo "[ ] Persistence: launchctl list, quét LaunchAgents"
echo "[ ] Ghi IOC: file, network, plist, hash"
echo "[ ] Revert snapshot, dọn mẫu khỏi VM"
echo
echo "Nhắc pháp lý: không patch app của người khác, không phát tán crack."
