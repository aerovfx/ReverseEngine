---
layout: course
title: "Readme"
permalink: /macos-reverse-engineering-10weeks/code/week02/README.html
---

# Tuần 02 — Code mẫu

## `macho_parser.py`
Parse header Mach-O (magic, cputype, filetype, load commands) từ file hoặc dữ liệu giả.

```bash
python3 macho_parser.py            # dữ liệu giả
python3 macho_parser.py <file>
```

## `fat_detector.py`
Nhận diện universal (fat) binary từ magic bytes.

```bash
python3 fat_detector.py <file>
```

**An toàn:** chỉ phân tích file do chính bạn tạo / được cấp phép.
