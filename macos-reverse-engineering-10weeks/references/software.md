---
layout: course
title: "Software"
permalink: /macos-reverse-engineering-10weeks/references/software.html
---

# Software — Công cụ và phần mềm trong khoá học

## Cài đặt môi trường

| Công cụ | Nguồn | Ghi chú |
|---|---|---|
| macOS (Sonoma/Sequoia) | Apple | Hệ điều hành mục tiêu |
| UTM / Parallels | Trang chủ | Trình ảo hoá cho VM macOS |
| Xcode Command Line Tools | Apple | `xcode-select --install` |
| Homebrew | brew.sh | Quản lý gói |

## Công cụ RE (cài qua Homebrew / pip)

| Công cụ | Cài đặt | Chức năng |
|---|---|---|
| Ghidra | `brew install --cask ghidra` | Decompiler (Mach-O + arm64) |
| Hopper | Trang chủ | Disassembler/decompiler (tuỳ chọn) |
| radare2 / rizin | `brew install radare2` | Framework RE dòng lệnh |
| lldb | Xcode CLT | Debugger |
| frida-tools | `pip install frida-tools` | Runtime hooking |
| class-dump | `brew install class-dump` | Khôi phục metadata Objective-C |
| Wireshark | `brew install --cask wireshark` | Bắt gói mạng |
| Little Snitch | Trang chủ | Giám sát kết nối mạng |
| swift-demangle | Xcode CLT | Giải mã symbol Swift |

## Công cụ hệ thống (đi kèm macOS)

- `otool`, `nm`, `file`, `lipo`, `size` — đọc/kiểm tra Mach-O.
- `codesign` — đọc code signature, entitlements.
- `fs_usage`, `log stream`, `sample`, `launchctl` — quan sát hành vi.

## Python 3 (cho code minh hoạ)

- Cài Python 3.8+ trên máy chính (chỉ chạy script minh hoạ, không phải mẫu).
- Không cần thư viện ngoài; toàn bộ script dùng thư viện chuẩn.

## Lưu ý

- Chỉ tải công cụ từ nguồn chính thức.
- Kiểm tra hash của công cụ trước khi cài.
- Không cài công cụ phân tích trên máy chính nếu không cần; ưu tiên chạy trong VM.
