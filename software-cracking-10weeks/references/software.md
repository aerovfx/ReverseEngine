---
layout: course
title: "Software"
permalink: /software-cracking-10weeks/references/software.html
---

# Công cụ phân tích

| Công cụ | Vai trò | Ghi chú |
|---|---|---|
| **x64dbg** | Debugger mã nguồn mở cho Windows | Snapshot trên x64dbg.com/SourceForge; giao diện đồ hoạ, phím tắt F2/F7/F8/F9 |
| **Detect It Easy (DIE)** | Nhận diện file type, compiler, packer | Chạy độc lập, đọc header PE/ELF, phát hiện compiler (MinGW, MSVC...) |
| **Ghidra** | Disassembler/decompiler | Hỗ trợ PE/ELF/Mach-O; dùng khi cần xem luồng cấp cao |
| **crackmes.one** | Kho crackme công khai | Nguồn binary hợp pháp để luyện tập |
| **Python 3** | Viết script minh hoạ an toàn | Thay thế việc chạy mẫu thật trong lab |

## Cài đặt nhanh (VM Windows)

```powershell
# 1. x64dbg: tải snapshot từ https://x64dbg.com -> giải nén
# 2. DIE: tải từ ntinfo.biz -> chạy bản portable
# 3. Nếu thiếu api-ms-win-crt-runtime-l1-1-0.dll:
#    cài Microsoft Visual C++ Redistributable 2015-2022 (x86/x64)
```

## Lưu ý

- Luôn chạy mẫu trong VM, không cài công cụ lên máy chính khi không cần.
- Xác minh hash của công cụ tải về khi có thể.
