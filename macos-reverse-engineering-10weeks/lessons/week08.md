---
layout: course
title: "Week08"
permalink: /macos-reverse-engineering-10weeks/lessons/week08.html
---

# Tuần 08 — Reverse Engineering Malware macOS mẫu 1 (XCSSET / OSX.Dok)

## Mục tiêu

- Nhận diện đặc điểm malware macOS và cơ chế lây nhiễm.
- Phân tích mẫu **XCSSET** (hoặc **OSX.Dok**) trong VM cô lập bằng static + dynamic.
- Viết báo cáo IOC theo chuẩn.

## Công cụ và dữ liệu

- VM macOS cô lập, snapshot, mạng giả lập/tắt.
- Ghidra, LLDB, Frida, `fs_usage`, Wireshark.
- Mẫu malware: **chỉ dùng mẫu do giảng viên/người sở hữu cấp phép** — không tải từ nơi công cộng.

## Lý thuyết và ví dụ

**XCSSET** — malware macOS lây qua Xcode project, đánh cắp dữ liệu từ app (Safari, Telegram...) và cài persistence. **OSX.Dok** — trojan giả mạo cập nhật, đánh cắp dữ liệu. Cả hai dùng kỹ thuật mà các tuần trước đã học: dylib injection, LaunchAgent, Objective-C hook.

Quy trình phân tích mẫu:
1. **Nhận dạng** — hash, `file`, `otool -hv`.
2. **Static** — strings, Ghidra decompile, class-dump, entitlements.
3. **Dynamic** — chạy trong VM, theo dõi file (`fs_usage`), mạng (Wireshark), tiến trình.
4. **Persistence** — quét LaunchAgents.
5. **IOC** — file, network, registry tương đương (plist), hash.

## Lab từng bước

1. Nhận mẫu được cấp phép; đặt trong VM cô lập, **snapshot trước khi chạy**.
2. Tính hash (`shasum -a 256`), chạy `file`, `otool -hv`, `strings`.
3. Mở trong Ghidra, decompile hàm chính; chạy class-dump nếu Objective-C.
4. Chạy mẫu trong VM (mạng tắt/giả lập), ghi nhận bằng `fs_usage` + Wireshark + `launchctl list`.
5. **Revert snapshot**; viết báo cáo IOC.

## Liên kết code mẫu

Xem [code/week08/README.md](../code/week08/README.md) — `ioc_report.py`, `launchagent_scan.sh`.

## Thảo luận

- Vì sao XCSSET dùng dylib injection và Objective-C hook để đánh cắp dữ liệu?
- Điều gì khiến malware macOS khó phát hiện hơn malware Windows?

## Bài tập

- **Cơ bản:** Liệt kê các bước phân tích malware macOS và công cụ dùng.
- **Nâng cao:** Phân tích static mẫu XCSSET được cấp phép trong VM, ghi lại IOC.
- **Thử thách:** Viết báo cáo IOC hoàn chỉnh gồm static + dynamic + network + persistence.

## Nộp bài và rubric

Nộp báo cáo RE mẫu 1 + bằng chứng (log/ảnh/output). Rubric chuẩn 100 điểm.

> **Cảnh báo pháp lý:** chỉ phân tích mẫu được cấp phép trong VM cô lập, mạng giả lập, snapshot trước khi chạy. Không tải mẫu từ nơi công cộng, không phát tán, không chạy trên máy chính. Tuần 09 chuyển sang RE app thương mại hợp pháp.
