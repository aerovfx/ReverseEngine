---
layout: course
title: "Readme"
permalink: /macos-reverse-engineering-10weeks/exercises/week02/README.html
---

# Bài tập Tuần 02 — Mach-O & Universal Binaries

## Cơ bản
Liệt kê các load command quan trọng và chức năng của chúng.

## Nâng cao
Dùng `otool -l` tìm các dylib mà binary phụ thuộc và giải thích ý nghĩa.

## Thử thách
Viết một file Mach-O giả bằng tay (theo format) và parse nó bằng `macho_parser.py`.

## Nộp bài và rubric
Nộp bảng phân tích Mach-O của `hello` (header, load commands, segments) + output script. Rubric 100 điểm.

> **Pháp lý:** chỉ phân tích binary do chính bạn tạo / được cấp phép.
