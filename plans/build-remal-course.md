# Xây khoá học "Reverse Engineering & Malware Analysis Fundamentals" (10 tuần) từ tài liệu Google Drive

## Bối cảnh

Người dùng có thư mục Google Drive công khai chứa khoá học "Reverse Engineering and Malware Analysis Fundamentals" gồm 22 module (video mp4, phụ đề srt, slide PDF, công cụ và mẫu malware kèm password). Yêu cầu xây khoá học theo mẫu CyberLearn (repo aerovfx/cybersercurity) với nội dung tiếng Việt. Thư mục làm việc hiện tại đang trống. Skill chuẩn `cyberlearn-course-creator` (đã đọc SKILL.md + course-blueprint.md) và khoá mẫu hoàn chỉnh `cybersecurity-fundamentals-10weeks` dùng làm khuôn cấu trúc. Người dùng đã chốt: tạo trong thư mục làm việc hiện tại; gom 22 module thành 10 tuần theo đề xuất; tài liệu gốc xử lý bằng link về Drive + tóm tắt (không tải video/slide về, an toàn bản quyền).

## File sẽ đụng tới

- `reverse-engineering-malware-10weeks/INDEX.md`
- `reverse-engineering-malware-10weeks/schedule.md`
- `reverse-engineering-malware-10weeks/lessons/week01.md ... week10.md`
- `reverse-engineering-malware-10weeks/code/README.md`
- `reverse-engineering-malware-10weeks/code/WEEKLY_EXAMPLES.md`
- `reverse-engineering-malware-10weeks/code/week01/...week10/ (README + scripts)`
- `reverse-engineering-malware-10weeks/exercises/week01.md ... week10.md`
- `reverse-engineering-malware-10weeks/projects/final_project.md`
- `reverse-engineering-malware-10weeks/references/components.md`
- `reverse-engineering-malware-10weeks/references/safety.md`
- `reverse-engineering-malware-10weeks/references/software.md`

## Ràng buộc không được phá

Theo skill cyberlearn-course-creator: tên thư mục kebab-case kết thúc `-10weeks`; 10 bài lessons/week01..week10; code minh hoạ chạy được, dữ liệu lab local an toàn, không secret/mục tiêu thật; bài tập 3 mức (cơ bản/nâng cao/thử thách); rubric 100 điểm (35/25/20/20); final project có rubric + tiêu chí thất bại bắt buộc; references gồm components/safety/software; chỉ dùng tài nguyên được phép — mẫu malware trên Drive CHỈ link về Drive kèm cảnh báo phạm vi pháp lý, không tải về/tự ý phân phối; nội dung tiếng Việt, thuật ngữ chuyên môn giữ tiếng Anh.

## Các bước

1. Tạo cấu trúc thư mục `reverse-engineering-malware-10weeks/` (lessons, code, exercises, projects, references).
2. Viết `INDEX.md` (mô tả, kết quả đầu ra, cấu trúc, quy tắc an toàn) và `schedule.md` (ma trận 10 tuần: năng lực chính, lab, sản phẩm nộp, nối tiếp).
3. Viết 10 bài `lessons/week01..10.md` theo nhóm module Drive: T1(01–03) giới thiệu+VM+FlareVM; T2(04–05) file formats+virtual memory/PE; T3(06–07) Windows Internals+intro static/dynamic; T4(08–09) tools bổ sung+Static S1; T5(10–11) Dynamic Workflow+Lab Dynamic S1; T6(12–13) Procdot+Network S1; T7(14–16) Sample 2 static (unpack/strings/PE hash); T8(17–18) Sample 2 dynamic+Assembly; T9(19–20) Sample 3 (AutoIT/Ghidra/x64dbg)+Sample 4 Ransomware (TeslaCrypt unpack); T10(21–22) Sample 5 Simda+Resources. Mỗi bài: mục tiêu, công cụ/dữ liệu, lý thuyết, lab từng bước, link Drive từng video/slide/mẫu, thảo luận, bài tập 3 mức, nộp bài, rubric.
4. Viết `code/` — các script Python/Bash minh hoạ an toàn: PE header parser (tự parse struct), file-type identifier (magic bytes, thay thế trid), strings extractor, hashing, entropy tính toán, disassembler đơn giản không, procmon-like log parser mô phỏng; mỗi tuần 1-3 ví dụ có README, chạy độc lập, dữ liệu giả, không đọc mẫu malware thật. Mỗi file code có header chú thích Việt (mục tiêu/đầu vào/đầu ra/an toàn).
5. Viết `exercises/week01..10.md` (3 mức mỗi tuần) và `projects/final_project.md` (bài toán, phạm vi, yêu cầu, milestone, deliverables, rubric 100đ, tiêu chí thất bại).
6. Viết `references/components.md`, `references/safety.md`, `references/software.md` (bảng công cụ, nguồn học, cảnh báo pháp lý về malware).
7. Kiểm tra chất lượng: đủ 10 bài, code biên dịch/chạy (python -m py_compile + chạy thử), đủ link Drive, không có link hỏng định dạng, kiểm tra cấu trúc theo gate trong blueprint.

## Cách kiểm chứng

- `find reverse-engineering-malware-10weeks/lessons -name 'week*.md' | wc -l` = 10.
- `find reverse-engineering-malware-10weeks/code -path '*/week*/README.md' | wc -l` ≥ 10.
- `python3 -m py_compile` mọi script Python trong code/ và chạy thử từng script với dữ liệu giả.
- `grep -c 'drive.google.com' lessons/*.md` > 0 ở mọi tuần (đảm bảo link Drive đủ).
- `git diff --check` (nếu có git) và rà secret/an toàn: không chứa mật khẩu mẫu malware thật ngoài link Drive, không code tải malware về.

## Phương án đã cân nhắc và loại bỏ

- Tải toàn bộ tài liệu (video/slide) về nhúng: bị loại vì nặng và rủi ro bản quyền/không cần thiết.
- Giữ 22 module thành 22 bài: bị loại vì trái chuẩn CyberLearn 10 tuần.
- Tạo trong Science/courses/: người dùng chọn thư mục làm việc hiện tại nên không dùng.
- Không viết code minh hoạ: bị loại vì skill yêu cầu khoá học chạy được, không chỉ tài liệu.
