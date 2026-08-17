# Xây khoá học "macOS Reverse Engineering" (10 tuần) — thiên về RE ứng dụng trên ARM64

## Bối cảnh

Đã có khóa Windows "reverse-engineering-malware-10weeks" hoàn chỉnh theo chuẩn CyberLearn. Người dùng muốn thêm một khóa mới chuyên về reverse engineering ứng dụng trên macOS, thiên về crack app thương mại, trên máy Apple Silicon (arm64), và GIỮ NGUYÊN khóa Windows hiện tại. Vì mục tiêu chính là crack app thương mại, plan phải đặt khung pháp lý lên hàng đầu: chỉ dạy kỹ thuật trong phạm vi hợp pháp (phân tích app của chính mình, app được cấp phép, nghiên cứu bảo mật), không phát tán bản crack, không né kiểm soát DRM/license của bên thứ ba cho mục đích lậu. Tái sử dụng cấu trúc CyberLearn của khóa Windows (10 tuần, lessons/code/exercises/projects/references, rubric 100đ).

## File sẽ đụng tới

- `macos-re-10weeks/INDEX.md`
- `macos-re-10weeks/schedule.md`
- `macos-re-10weeks/lessons/week01.md ... week10.md`
- `macos-re-10weeks/code/README.md`
- `macos-re-10weeks/code/WEEKLY_EXAMPLES.md`
- `macos-re-10weeks/code/week01/...week10/ (README + scripts)`
- `macos-re-10weeks/exercises/week01.md ... week10.md`
- `macos-re-10weeks/projects/final_project.md`
- `macos-re-10weeks/references/components.md`
- `macos-re-10weeks/references/safety.md`
- `macos-re-10weeks/references/software.md`

## Ràng buộc không được phá

Theo skill cyberlearn-course-creator: tên thư mục kebab-case kết thúc -10weeks; 10 bài lessons/week01..10; code minh hoạ chạy được, dữ liệu lab local an toàn, không secret/mục tiêu thật; bài tập 3 mức; rubric 100 điểm (35/25/20/20); final project có rubric + tiêu chí thất bại bắt buộc; references gồm components/safety/software; nội dung tiếng Việt, thuật ngữ chuyên môn giữ tiếng Anh. PHÁP LÝ BẮT BUỘC: mọi nội dung crack/license chỉ trong phạm vi hợp pháp — app của chính mình, app được cấp phép cho mục đích nghiên cứu, hoặc app mã nguồn mở; cấm hướng dẫn phát tán bản crack, né DRM để lậu, hoặc dùng kỹ thuật để vi phạm bản quyền; mọi bài đều có cảnh báo pháp lý rõ ràng. Không đụng tới thư mục reverse-engineering-malware-10weeks hiện có.

## Các bước

1. Tạo cấu trúc thư mục macos-re-10weeks/ (lessons, code, exercises, projects, references). 2. Viết INDEX.md (mô tả, kết quả đầu ra, cấu trúc, quy tắc an toàn & pháp lý) và schedule.md (ma trận 10 tuần). 3. Viết 10 bài lessons/week01..10.md theo chủ đề macOS RE trên arm64, thiên về RE ứng dụng: T1 môi trường lab macOS (VM Apple Silicon, SIP/AMFI, snapshot, cài công cụ thủ công qua brew); T2 Mach-O & universal binaries (otool, nm, lipo); T3 macOS internals (dyld, Objective-C/Swift runtime, TCC, sandbox, code signing); T4 static analysis (Ghidra/Hopper, class-dump, strings, codesign/entitlements); T5 dynamic analysis (LLDB, Frida, fs_usage, unified logging); T6 runtime & hooking (method swizzling, dylib injection, Frida script); T7 network & persistence (LaunchAgents, Wireshark, Little Snitch); T8 RE malware macOS mẫu 1 (XCSSET/OSX.Dok) trong lab cô lập; T9 RE app thương mại hợp pháp — phân tích cơ chế license, patch để nghiên cứu, pháp lý; T10 RE malware macOS mẫu 2 (EvilQuest) + final project. Mỗi bài: mục tiêu, công cụ/dữ liệu, lý thuyết, lab từng bước, thảo luận, bài tập 3 mức, nộp bài, rubric, và khối cảnh báo pháp lý. 4. Viết code/ — script Python/Bash/AppleScript minh hoạ an toàn: Mach-O header parser, fat-binary detector, strings extractor, class-dump mô phỏng, entitlements/codesign inspector, dylib-injection demo trên app do chính người học viết, Frida hook template, entropy, license-check stub mô phỏng; mỗi tuần 1-3 ví dụ có README, chạy độc lập, dữ liệu giả. 5. Viết exercises/week01..10.md (3 mức) và projects/final_project.md (phân tích app của chính mình hoặc app mã nguồn mở, viết báo cáo; rubric 100đ + tiêu chí thất bại). 6. Viết references/components.md, references/safety.md (gồm cảnh báo pháp lý RE app thương mại + quy tắc macOS SIP/AMFI/TCC), references/software.md (bảng công cụ macOS arm64). 7. Kiểm tra chất lượng: đủ 10 bài, code biên dịch/chạy, đủ link tài liệu, rà pháp lý (mọi bài có cảnh báo, không hướng dẫn phát tán crack/DRM lậu).

## Cách kiểm chứng

- find macos-re-10weeks/lessons -name 'week*.md' | wc -l = 10. - find macos-re-10weeks/code -path '*/week*/README.md' | wc -l ≥ 10. - python3 -m py_compile mọi script Python; bash -n mọi script Bash; chạy thử với dữ liệu giả. - grep -iE 'pháp lý|legal|license|copyright|DMCA' lessons/*.md ở MỌI tuần > 0 (đảm bảo cảnh báo pháp lý hiện diện). - git diff --check (nếu có git); rà secret/an toàn. - git status đảm bảo không sửa file trong reverse-engineering-malware-10weeks/.

## Phương án đã cân nhắc và loại bỏ

- Đan xen macOS vào khóa Windows hiện tại: bị loại vì người dùng chọn phương án A (khóa riêng) và muốn giữ khóa Windows nguyên vẹn. - Thiên về malware macOS thay vì RE app: bị loại vì người dùng chọn mục tiêu crack app thương mại làm trọng tâm; malware macOS vẫn giữ 2 tuần (T8, T10) nhưng không là trọng tâm. - Dùng Hopper thay Ghidra làm công cụ chính: cân nhắc, nhưng Ghidra miễn phí + đã hỗ trợ Mach-O/arm64 nên chọn Ghidra làm chính, Hopper giới thiệu thêm. - Không viết code minh hoạ: bị loại vì chuẩn CyberLearn yêu cầu code chạy được.
