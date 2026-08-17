#!/usr/bin/env python3
# ============================================================
# report_builder.py — Sinh khung báo cáo RE tổng hợp
# Mục tiêu: tạo template báo cáo reverse engineering.
# Đầu vào: không.
# Đầu ra: khung báo cáo RE (in ra).
# An toàn: dữ liệu do bạn điền; không chứa secret/mẫu thật.
# ============================================================

def main():
    print("""# Báo cáo Reverse Engineering — macOS

## 1. Tổng quan
- Mục tiêu: <app/mẫu>
- Phạm vi hợp pháp: <app của mình / được cấp phép / mã nguồn mở>

## 2. Static analysis
- Nhận dạng: <hash, file, otool>
- Strings / symbol: <kết quả>
- Decompile (Ghidra): <hàm chính + luồng>

## 3. Dynamic analysis
- LLDB/Frida: <breakpoint, hook, tham số>
- Hành vi: <file, network, process>

## 4. Runtime / hooking
- Swizzle / dylib / Frida: <kỹ thuật dùng>

## 5. Kết luận
- Cơ chế hoạt động: <tóm tắt>
- Điểm yếu/phòng thủ: <nếu là app của bạn>

## 6. Bằng chứng
- <ảnh, log, output script>
""")


if __name__ == "__main__":
    main()
