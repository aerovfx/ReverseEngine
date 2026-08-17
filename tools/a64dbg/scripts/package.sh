#!/bin/bash
# Đóng gói a64dbg.app → dist/a64dbg-<version>.dmg.
# LƯU Ý: hdiutil cần chạy ngoài sandbox (tạo loopback device). Chạy trên terminal thường.
set -euo pipefail
cd "$(dirname "$0")/.."

VERSION=$(grep -m1 'project(a64dbg VERSION' CMakeLists.txt | sed -E 's/.*VERSION ([0-9.]+).*/\1/')
APP=build/mac-arm64-release/src/app/a64dbg.app
DMG="dist/a64dbg-${VERSION}.dmg"

if [ ! -d "$APP" ]; then
  echo "Chưa build release. Chạy: cmake --build --preset mac-arm64-release" >&2
  exit 1
fi

mkdir -p dist
rm -f "$DMG"

# 1. Tạo ảnh read-write tạm.
RW="$(mktemp -d)/a64dbg.rw.dmg"
hdiutil create -size 80m -volname "a64dbg" -fs HFS+ -layout NONE "$RW" >/dev/null

# 2. Mount, copy .app + symlink /Applications.
MP=$(hdiutil attach -nobrowse "$RW" | grep -o '/Volumes/.*' | tail -1)
trap 'hdiutil detach "$MP" >/dev/null 2>&1 || true' EXIT
cp -R "$APP" "$MP/"
ln -s /Applications "$MP/Applications"

# 3. Detach + convert sang UDZO (nén).
hdiutil detach "$MP" >/dev/null
trap - EXIT
hdiutil convert "$RW" -format UDZO -o "$DMG" >/dev/null

echo "Đã tạo: $DMG"
