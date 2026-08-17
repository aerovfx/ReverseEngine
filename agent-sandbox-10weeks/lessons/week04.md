---
layout: course
title: "Week04"
permalink: /agent-sandbox-10weeks/lessons/week04.html
---

# Tuần 04 — Khởi động nhanh & snapshot/restore

## Mục tiêu

- Giải thích kỹ thuật khởi động microVM dưới 100ms.
- Phân biệt snapshot bộ nhớ (memory) với snapshot ổ đĩa.
- Hiểu restore path của RustVMM.

## Công cụ và dữ liệu

- `docs/guide/snapshot-rollback-clone.md`; `CubeShim/`, `hypervisor/`.
- Script `code/week04/snapshot_sim.py` (mô phỏng snapshot/restore trạng thái).

## Lý thuyết

Khởi động VM bình thường tốn vài giây (POST, kernel boot, init). CubeSandbox đạt **<100ms** bằng cách:

1. **Template đã snapshot sẵn** — rootfs + memory image đã "nóng" (kernel đã boot).
2. **Restore** — RustVMM nạp lại snapshot bộ nhớ thay vì boot lại từ đầu.
3. **CoW storage** — mỗi sandbox chỉ clone tham chiếu (không copy byte).

**Snapshot bộ nhớ** lưu trạng thái vCPU + RAM (dirty pages); **snapshot ổ đĩa** lưu thay đổi filesystem. Cả hai dùng CoW để tiết kiệm dung lượng.

## Lab từng bước

1. Đọc tài liệu snapshot-rollback-clone.
2. Chạy `python snapshot_sim.py` để mô phỏng lưu/phục hồi trạng thái.
3. So sánh "boot lạnh" (cold boot) vs "restore" (warm boot) về thời gian.
4. Giải thích dirty-page tracking giúp snapshot nhỏ đi thế nào.

## Liên kết code mẫu

Xem [code/week04/README.md](../code/week04/README.md) — `snapshot.cpp` (C++ chi tiết) và `snapshot_sim.py` (Python mô phỏng nhanh).

## Thảo luận

- Vì sao restore nhanh hơn boot lại từ đầu?
- Dirty-page tracking liên quan gì đến CoW?

## Bài tập

- **Cơ bản:** Giải thích snapshot bộ nhớ khác snapshot ổ đĩa.
- **Nâng cao:** Mô tả 4 bước của restore path.
- **Thử thách:** Ước lượng vì sao snapshot nhỏ hơn nhiều so với RAM tổng.

## Rubric (100 điểm)

- Đúng chức năng (35): mô phỏng chạy đúng, giải thích đúng.
- An toàn (25): không đụng hệ thống thật.
- Tài liệu (20): sơ đồ snapshot rõ.
- Phân tích (20): giải thích được trade-off dung lượng/tốc độ.

## An toàn & phạm vi

Chỉ mô phỏng bằng dữ liệu, không thao tác máy ảo thật của người khác.
