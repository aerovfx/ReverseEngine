---
layout: course
title: "Week08"
permalink: /agent-sandbox-10weeks/lessons/week08.html
---

# Tuần 08 — Test tool an toàn trong sandbox

## Mục tiêu

- Dùng SDK (E2B-compatible) để tạo sandbox và chạy code.
- Áp dụng whitelist, timeout, giới hạn tài nguyên khi chạy tool.
- Phân tích mô hình đe doạ của chính sandbox.

## Công cụ và dữ liệu

- `sdk/`, `examples/` (E2B SDK, `Sandbox.create()`).
- Script `code/week08/tool_runner.py` (mô phỏng wrapper chạy tool an toàn).

## Lý thuyết

Test tool an toàn = chạy tool trong sandbox **kèm** các ràng buộc:

- **Whitelist** — chỉ cho phép danh sách lệnh/package đã biết.
- **Timeout** — giới hạn thời gian chạy, tránh treo/loop.
- **Giới hạn tài nguyên** — CPU, RAM, disk (cgroup/VM).
- **Cô lập mạng** — egress allowlist (tuần 07).
- **Output capture** — ghi log để audit.

Với CubeSandbox, code client đơn giản như:

```python
from e2b import Sandbox
sb = Sandbox.create(template="base")     # tạo microVM trong ms
out = sb.process.run("ls -la")           # chạy lệnh, lấy stdout
sb.kill()
```

## Lab từng bước

1. Đọc `examples/` và `sdk/` để hiểu API.
2. Chạy `python tool_runner.py` để mô phỏng whitelist + timeout.
3. Liệt kê 3 rủi ro khi agent chạy tool và cách sandbox giảm rủi ro.
4. Giải thích vì sao "thoát sandbox" (VM escape) là rủi ro tối thượng cần giám sát.

## Liên kết code mẫu

Xem [code/week08/README.md](../code/week08/README.md) — `tool_runner.cpp` (C++ chi tiết) và `tool_runner.py` (Python mô phỏng nhanh).

## Thảo luận

- Whitelist khác denylist ở độ an toàn nào?
- Vì sao cần vừa timeout vừa giới hạn tài nguyên?

## Bài tập

- **Cơ bản:** Liệt kê 5 ràng buộc khi chạy tool an toàn.
- **Nâng cao:** Viết wrapper chạy lệnh có whitelist + timeout.
- **Thử thách:** Thiết kế chính sách cho phép agent chạy `pip install` nhưng chặn `rm -rf /`.

## Rubric (100 điểm)

- Đúng chức năng (35): wrapper chạy đúng, có whitelist/timeout.
- An toàn (25): xử lý lỗi, không vượt ràng buộc.
- Code (20): có chú thích, cleanup.
- Phân tích (20): giải thích mô hình đe doạ.

## An toàn & phạm vi

Chỉ chạy tool trong sandbox của bạn. Không test escape trên hệ thống người khác.
