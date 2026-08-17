# Tuần 01 — Môi trường lab macOS và chuẩn bị công cụ

## Mục tiêu

- Giải thích vì sao cần môi trường lab cô lập khi phân tích (malware hoặc app chưa rõ nguồn).
- Dựng máy ảo macOS trên Apple Silicon và hiểu vai trò của SIP/AMFI.
- Cài bộ công cụ RE macOS qua Homebrew và kiểm tra môi trường.

## Công cụ và dữ liệu

- Apple Silicon Mac, macOS Sonoma/Sequoia; trình ảo hoá (UTM, Parallels, hoặc macOS VM qua `SoftwareUpdate`/IPSW).
- Homebrew, Xcode Command Line Tools.
- Script `code/week01/macos_env_check.sh` (kiểm tra môi trường).

## Lý thuyết và ví dụ

Phân tích macOS cần một môi trường **cô lập và khôi phục nhanh**. Ba nguyên tắc:

1. **Cô lập** — chạy mẫu/app lạ trong VM, không có quyền truy cập máy chính.
2. **Khôi phục nhanh** — snapshot trước mỗi lần chạy, revert sau khi xong.
3. **Mạng giả lập** — chặn hoặc giả lập kết nối ra ngoài.

**SIP (System Integrity Protection)** và **AMFI (Apple Mobile File Integrity)** là các lớp bảo vệ của macOS. Trong lab, đôi khi cần tắt/vô hiệu một phần (qua Recovery) để chạy công cụ hook — việc này **chỉ làm trong VM lab**, không bao giờ trên máy chính.

Khác với Windows có Flare VM, macOS **không có bộ cài sẵn** — cần cài công cụ thủ công qua `brew`. Đây là kỹ năng bắt buộc của người làm RE macOS.

## Lab từng bước

1. Cài Xcode Command Line Tools: `xcode-select --install`.
2. Cài Homebrew: `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`.
3. Cài công cụ RE: `brew install --cask ghidra` và `brew install lldb radare2`.
4. Dựng VM macOS (UTM/Parallels) và **tắt SIP/AMFI chỉ trong VM** qua Recovery.
5. Chạy `bash code/week01/macos_env_check.sh` để sinh checklist môi trường và ghi kết quả.
6. Tạo snapshot "Base" sau khi cài xong; ghi chú cấu hình vào file mô tả lab.

## Liên kết code mẫu

Xem [code/week01/README.md](../code/week01/README.md) — script `macos_env_check.sh`.

## Thảo luận

- Vì sao cần tách môi trường lab khỏi máy chính, ngay cả khi chỉ phân tích app "sạch"?
- Sự khác nhau giữa SIP và AMFI; điều gì xảy ra nếu tắt cả hai trên máy thật?

## Bài tập

- **Cơ bản:** Liệt kê 5 công cụ RE macOS và mục đích của từng công cụ.
- **Nâng cao:** Dựng VM macOS, tắt SIP/AMFI trong VM, và giải thích rủi ro nếu làm trên máy chính.
- **Thử thách:** Viết quy trình khôi phục lab từ snapshot trong 5 phút, kèm checklist trước khi chạy mẫu.

## Nộp bài và rubric

Nộp mô tả cấu hình VM, ảnh chụp màn hình công cụ cài xong, output của `macos_env_check.sh`. Chấm theo rubric 100 điểm: đúng quy trình 35; an toàn/cô lập 25; tài liệu rõ ràng 20; bằng chứng và giải thích 20.

> **Cảnh báo pháp lý:** chỉ dựng lab trên máy bạn sở hữu. Không tải mẫu malware về máy thật; không tắt SIP/AMFI ngoài VM lab. Tuần 02 bắt đầu đọc cấu trúc Mach-O.
