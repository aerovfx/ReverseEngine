---
layout: course
title: "Week05"
permalink: /agent-sandbox-10weeks/lessons/week05.html
---

# Tuần 05 — Storage Copy-on-Write & clone O(1)

## Mục tiêu

- Giải thích Copy-on-Write (CoW) và reflink FICLONE trên XFS.
- Phân tích CubeCoW: snapshot/clone O(1), dirty-page tracking.
- Hiểu templates và volumes trong CubeSandbox.

## Công cụ và dữ liệu

- `cubecow/` (CubeCoW); `docs/guide/persistent-storage.md`.
- Script `code/week05/cow_storage.py` (mô phỏng shared extent + copy-on-write).

## Lý thuyết

CoW nghĩa là "chỉ sao chép khi ghi": nhiều sandbox chia sẻ cùng **extent** (khối dữ liệu) của template. Khi một sandbox ghi, kernel tạo bản sao riêng cho extent đó (reflink trên XFS qua ioctl **FICLONE**).

```
Template (read-only)
   ├── Sandbox A  →  extent1, extent2 (shared)
   └── Sandbox B  →  extent1, extent3 (extent2 đã bị B ghi → copy riêng)
```

Kết quả: snapshot/clone **O(1)** về thời gian (chỉ tạo metadata), gần như **0 byte** copy ban đầu. CubeCoW còn theo dõi **dirty pages** để snapshot chỉ lưu phần đã thay đổi.

## Lab từng bước

1. Đọc persistent-storage và code `cubecow/`.
2. Chạy `python cow_storage.py` để mô phỏng chia sẻ extent.
3. Mô tả điều gì xảy ra khi sandbox A và B cùng ghi vào một extent.
4. Giải thích vì sao cần XFS (reflink) chứ không phải ext4.

## Liên kết code mẫu

Xem [code/week05/README.md](../code/week05/README.md) — script `cow_storage.py`.

## Thảo luận

- FICLONE khác `cp` truyền thống ở chỗ nào?
- Vì sao "xóa một snapshot không ảnh hưởng snapshot khác" (flat snapshot)?

## Bài tập

- **Cơ bản:** Giải thích CoW bằng một ví dụ 2 sandbox.
- **Nâng cao:** Mô tả dirty-page tracking giúp snapshot nhỏ.
- **Thử thách:** Ước lượng dung lượng tiết kiệm khi 100 sandbox clone 1 template 2GB.

## Rubric (100 điểm)

- Đúng chức năng (35): mô phỏng chạy đúng.
- An toàn (25): không thao tác filesystem thật.
- Tài liệu (20): sơ đồ extent rõ.
- Phân tích (20): giải thích O(1) và CoW.

## An toàn & phạm vi

Chỉ mô phỏng bằng Python, không chạy FICLONE trên hệ thống người khác.
