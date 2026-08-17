#!/usr/bin/env bash
# ============================================================
# macos_env_check.sh — Checklist môi trường lab macOS
# Mục tiêu: kiểm tra Homebrew, Xcode CLT, công cụ RE đã cài chưa.
# Đầu vào: không.
# Đầu ra: checklist trạng thái từng thành phần.
# An toàn: chỉ đọc, không thay đổi hệ thống; không chạy mẫu.
# ============================================================
set -u

echo "=== macOS Reverse Engineering — Environment Check ==="
sw_vers

echo
echo "[1] Xcode Command Line Tools"
if xcode-select -p >/dev/null 2>&1; then
  echo "  OK   : $(xcode-select -p)"
else
  echo "  MISS : chạy 'xcode-select --install'"
fi

echo
echo "[2] Homebrew"
if command -v brew >/dev/null 2>&1; then
  echo "  OK   : $(brew --version | head -1)"
else
  echo "  MISS : cài Homebrew"
fi

echo
echo "[3] Công cụ RE"
for tool in lldb radare2 ghidra otool nm lipo file frida; do
  if command -v "$tool" >/dev/null 2>&1; then
    echo "  OK   : $tool"
  else
    echo "  MISS : $tool"
  fi
done

echo
echo "[4] Kiến trúc"
echo "  $(uname -m) / $(uname -s)"

echo
echo "=== Xong. Ghi kết quả vào file mô tả lab. ==="
