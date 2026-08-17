// 04_strings_codesign.cpp — Trích xuất chuỗi ASCII + ghi chú kiểm tra codesign
// Mục tiêu: tìm chuỗi khả nghi (URL, license, lệnh) trong Mach-O trước khi RE.
// An toàn: chỉ ĐỌC file, không thực thi.  g++ -std=c++17 -O2 strings_codesign.cpp -o demo
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>

int main(int argc, char** argv) {
    if (argc < 2) { std::cout << "Dùng: ./demo <file>\n"; return 1; }
    std::ifstream f(argv[1], std::ios::binary);
    std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), {});

    std::cout << "Chuỗi ASCII (>= 6 ký tự):\n" << std::string(56,'-') << "\n";
    std::string cur;
    for (uint8_t b : d) {
        char c = (char)b;
        if (std::isprint((unsigned char)c)) cur += c; else { if (cur.size() >= 6) std::cout << "  " << cur << "\n"; cur.clear(); }
    }
    if (cur.size() >= 6) std::cout << "  " << cur << "\n";

    std::cout << "\nNhắc nhở: chạy 'codesign -dv' để kiểm tra signature/entitlements\n";
    std::cout << "của binary — dấu hiệu app đã bị sửa là signature invalid.\n";
    return 0;
}
