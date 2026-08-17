# Tuần 10 — RE malware macOS mẫu 2 (EvilQuest) + tổng kết + final project

## Mục tiêu

- Phân tích mẫu **EvilQuest/ThiefQuest** trong VM cô lập.
- Tổng hợp toàn bộ kỹ năng: static, dynamic, network, persistence, RE.
- Hoàn thiện **final project** theo [projects/final_project.md](../projects/final_project.md).

## Công cụ và dữ liệu

- VM macOS cô lập, snapshot, mạng giả lập.
- Ghidra, LLDB, Frida, `fs_usage`, Wireshark, `launchctl`.
- Mẫu malware: **chỉ dùng mẫu được cấp phép** từ giảng viên/người sở hữu.

## Lý thuyết và ví dụ

**EvilQuest/ThiefQuest** — ransomware + stealer macOS: mã hoá file, đánh cắp dữ liệu và cài persistence. Đây là case study tốt để tổng hợp mọi kỹ thuật đã học: Mach-O, Objective-C/Swift runtime, dylib injection, LaunchAgent, code signing, network.

Quy trình tổng hợp:
1. **Nhận dạng** — hash, `file`, `otool`, strings.
2. **Static** — Ghidra decompile, class-dump, entitlements.
3. **Dynamic** — LLDB/Frida, `fs_usage`, Wireshark, `launchctl list`.
4. **IOC** — file, network, plist, hash.
5. **Phòng thủ** — biện pháp giảm thiểu.

## Lab từng bước

1. Nhận mẫu EvilQuest được cấp phép; snapshot trước khi chạy.
2. Phân tích static (Ghidra, strings, class-dump) — tìm hàm mã hoá, hàm đánh cắp.
3. Chạy trong VM (mạng tắt), theo dõi bằng `fs_usage` + Wireshark + `launchctl`.
4. **Revert snapshot**; viết báo cáo IOC.
5. Hoàn thiện final project: báo cáo phân tích + demo 5 phút + retrospective.

## Liên kết code mẫu

Xem [code/week10/README.md](../code/week10/README.md) — `report_builder.py`, `rechecklist.sh`.

## Thảo luận

- EvilQuest kết hợp ransomware + stealer như thế nào? Vì sao hiệu quả?
- Kỹ năng nào trong 10 tuần là quan trọng nhất để phát hiện malware tương tự?

## Bài tập

- **Cơ bản:** Tóm tắt chuỗi lây nhiễm và persistence của EvilQuest.
- **Nâng cao:** Phân tích static hàm mã hoá của EvilQuest (mẫu cấp phép) trong Ghidra.
- **Thử thách:** Hoàn thiện final project theo rubric 100 điểm.

## Nộp bài và rubric

Nộp báo cáo RE mẫu 2 + final project. Rubric chuẩn 100 điểm.

> **Cảnh báo pháp lý:** chỉ phân tích mẫu được cấp phép trong VM cô lập. Không chạy trên máy chính, không phát tán. Final project phải dùng app/mẫu hợp pháp theo [safety.md](../references/safety.md).
