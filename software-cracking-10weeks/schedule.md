---
layout: course
title: "Schedule"
permalink: /software-cracking-10weeks/schedule.html
---

# Lộ trình Software Cracking & Ethical Hacking — 10 tuần

| Tuần | Chủ đề | Nguồn module | Lab local | Sản phẩm nộp | Nối tiếp |
|---:|---|---|---|---|---|
| 01 | Tổng quan ethical hacking & môi trường lab | M1 | Dựng VM Windows + RoE + checklist | Rules of Engagement + lab checklist | Nền tảng cho mọi lab |
| 02 | x64dbg & Detect It Easy | M2 | Cài x64dbg + DIE; sửa lỗi VC++ runtime | Ảnh cài xong + báo cáo DIE | Công cụ cho tuần sau |
| 03 | PE, base address & entry point | M3 | Dùng DIE đọc PE app tự viết | Bảng phân tích PE | Hiểu binary trước khi debug |
| 04 | Bước cơ bản trong debugger | M4 | Run/step over, nhận diện jump | Bản đồ luồng + jump | Nền tảng stepping |
| 05 | Stepping & call stack | M5 | Step into/over trên crackme | Báo cáo stepping | Kỹ thuật điều hướng luồng |
| 06 | Breakpoints | M6 | Đặt BP + tìm chuỗi (string references) | Báo cáo breakpoint | Đầu vào cho vá lỗi |
| 07 | Reversing jumps | M7 | Đảo nhảy điều kiện bypass "try harder" | Báo cáo đảo nhảy | Kỹ thuật bypass luồng |
| 08 | Patch binary & tổng kết quy trình | M8+M9 | NOP/byte patch + lưu bản vá | Bản vá + hash trước/sau | Quy trình hoàn chỉnh |
| 09 | Cracking GUI & phân tích PE | M10+M11 | Phân tích MessageBox/GetWindowText | Báo cáo GUI | Ứng dụng thực tế |
| 10 | Crack serial key & dự án cuối | M12 | BP trên chuỗi serial + keygen hợp pháp | Serial + final project | Lộ trình cá nhân tiếp |

## Cách đánh giá

Mỗi tuần dùng rubric 100 điểm: đúng chức năng 35, an toàn và xử lý lỗi 25, chất lượng code/tài liệu 20, phân tích và bằng chứng chạy 20.

## Quy tắc lab và pháp lý

- Mọi phân tích chạy trong **VM Windows cô lập**, snapshot trước khi chạy mẫu.
- Chỉ phân tích **phần mềm của bạn**, **được phép bằng văn bản**, hoặc **crackme công khai**.
- **Cấm** phát tán crack/patch cho phần mềm thương mại; cấm né DRM/license để dùng lậu.
- Ghi hash SHA-256 trước/sau khi vá; giữ chain of custody.
