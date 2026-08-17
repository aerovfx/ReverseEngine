---
layout: course
title: "Readme"
permalink: /macos-reverse-engineering-10weeks/code/week04/README.html
---

# Tuần 04 — Code mẫu

## `strings_extract.py`
Trích strings ASCII/UTF-8 từ binary.

```bash
python3 strings_extract.py <file>
```

## `codesign_inspect.sh`
Đọc code signature + entitlements của app.

```bash
bash codesign_inspect.sh <path-to-app>
```

**An toàn:** chỉ dùng trên app của chính bạn / được cấp phép.
