# -*- coding: utf-8 -*-
"""
01_risk_model.py — Phân loại mức rủi ro khi agent gọi tool.

Mục tiêu: minh hoạ cách xây mô hình đe doạ cho agent chạy tool.
Đầu vào : không (demo); có thể mở rộng danh sách TOOLS.
Đầu ra : bảng phân loại rủi ro theo mức (thấp/trung bình/cao/nghiêm trọng).
An toàn : chỉ dùng dữ liệu, không chạy tool thật.
"""

# (tên tool, hành động, mức rủi ro, lý do)
TOOLS = [
    ("read_file", "đọc file văn bản", "thấp", "chỉ đọc, không thay đổi trạng thái"),
    ("list_dir", "liệt kê thư mục", "thấp", "chỉ đọc metadata"),
    ("write_file", "ghi file", "trung bình", "có thể ghi đè dữ liệu"),
    ("run_shell", "chạy lệnh shell", "cao", "toàn quyền trên máy nếu không sandbox"),
    ("http_request", "gửi HTTP ra ngoài", "cao", "có thể rò rỉ dữ liệu ra C2"),
    ("install_pkg", "cài package", "nghiêm trọng", "có thể cài mã độc hại"),
]


def classify():
    order = {"thấp": 0, "trung bình": 1, "cao": 2, "nghiêm trọng": 3}
    return sorted(TOOLS, key=lambda t: order[t[2]])


def main():
    print("Mô hình đe doạ: phân loại rủi ro tool của agent\n")
    print(f"{'Tool':<14} {'Rủi ro':<12} Lý do")
    print("-" * 60)
    for name, action, level, why in classify():
        print(f"{name:<14} {level:<12} {why} ({action})")
    print("\nKết luận: mọi tool 'thay đổi trạng thái' đều nên chạy trong sandbox.")


if __name__ == "__main__":
    main()
