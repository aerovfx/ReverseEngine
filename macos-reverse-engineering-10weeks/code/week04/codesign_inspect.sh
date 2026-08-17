#!/usr/bin/env bash
# ============================================================
# codesign_inspect.sh — Đọc code signature + entitlements của app
# Mục tiêu: xem bundle id, team, entitlements của app.
# Đầu vào: đường dẫn app (.app) hoặc binary.
# Đầu ra: thông tin codesign.
# An toàn: chỉ đọc; dùng trên app của chính bạn / được cấp phép.
# ============================================================
set -u

TARGET="${1:?Cách dùng: codesign_inspect.sh <path-to-app-or-binary>}"

echo "=== Code signature: $TARGET ==="
codesign -dvvv "$TARGET" 2>&1 | head -20

echo
echo "=== Entitlements ==="
codesign -d --entitlements - "$TARGET" 2>&1 | head -40

echo
echo "=== Xác minh chữ ký ==="
codesign --verify --deep --strict "$TARGET" 2>&1 && echo "Chữ ký hợp lệ" || echo "Chữ ký KHÔNG hợp lệ (có thể đã bị sửa)"
