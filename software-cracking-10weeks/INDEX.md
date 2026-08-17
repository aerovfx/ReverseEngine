---
layout: course
title: "Index"
permalink: /software-cracking-10weeks/INDEX.html
---

# Software Cracking & Ethical Hacking — 10 tuần

Khoá học 10 tuần về **phân tích và vá phần mềm (software cracking) trong phạm vi hợp pháp**, xây dựng từ nội dung khóa học *"Software Ethical Hacking – Crack Software like A Pro"* (Udemy, 12 module) — được tái cấu trúc theo chuẩn ReverseEngine: bài học, code minh hoạ chạy được, bài tập 3 mức và dự án cuối khoá.

> Nguồn tham khảo: Udemy – Software Ethical Hacking – Crack Software like A Pro. Khoá học **không sao chép** video/phụ đề; toàn bộ bài viết là nội dung tiếng Việt được biên soạn lại.

## Kết quả đầu ra

Sau 10 tuần, người học có thể:

- Giải thích ranh giới đạo đức/pháp lý của reverse engineering và cracking.
- Dựng môi trường lab Windows cô lập, cài **x64dbg** và **Detect It Easy (DIE)**.
- Đọc cấu trúc **PE** (base address, entry point) và nhận diện compiler bằng DIE.
- Điều khiển debugger: run, step into/over, đặt **breakpoints**.
- **Đảo nhảy điều kiện** (reversing jumps) để hiểu luồng kiểm tra.
- **Vá (patch)** binary và lưu lại bản vá trên bản sao lab.
- Phân tích chương trình **GUI** (Windows API) và tìm **serial key** qua breakpoint trên chuỗi.
- Báo cáo phân tích có bằng chứng và đề xuất phòng thủ.

## Cấu trúc

- 10 bài học tăng dần: môi trường → PE/binary → debugger → breakpoint → patch → GUI/serial.
- 10 bộ code minh hoạ Python mô phỏng an toàn các kỹ thuật (không cần mẫu thật).
- Bài tập cơ bản, nâng cao, thử thách mỗi tuần.
- Rubric 100 điểm và dự án cuối khoá (phân tích crackme hoặc app của chính bạn).

## Quy tắc an toàn và pháp lý (BẮT BUỘC)

> Khoá học chỉ dạy kỹ thuật trong **phạm vi hợp pháp**: phân tích **phần mềm bạn sở hữu**, phần mềm **được cấp phép bằng văn bản** cho mục đích nghiên cứu, hoặc **crackme** công khai (như crackmes.one) được tạo ra để luyện tập. **Cấm** phát tán bản crack, né kiểm soát DRM/license để dùng lậu, hoặc dùng kỹ thuật để vi phạm bản quyền. Mọi lab chạy trong VM cô lập, có snapshot. Xem [references/safety.md](references/safety.md).

## Tài liệu tham khảo

- [Lịch học](schedule.md)
- [Công cụ](references/software.md) · [Thành phần binary](references/components.md) · [An toàn & pháp lý](references/safety.md)
- [Dự án cuối khóa](projects/final_project.md)
