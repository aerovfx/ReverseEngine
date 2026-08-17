// 07_analyzer.cpp — Static analyzer: trích chuỗi + liệt kê import (mô phỏng)
// Mục tiêu: hiểu cách static analyzer (Ghidra/IDA) đọc chuỗi + import để vẽ luồng.
// An toàn: chỉ ĐỌC file, quét chuỗi ASCII.  g++ -std=c++17 -O2 analyzer.cpp -o demo
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>

int main(int argc, char** argv) {
    if (argc < 2) { std::cout << "Dùng: ./demo <file>\n"; return 1; }
    std::ifstream f(argv[1], std::ios::binary);
    std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), {});

    std::cout << "Chuỗi khả nghi (URL/lệnh/serial):\n" << std::string(56,'-') << "\n";
    std::string cur;
    for (uint8_t b : d) {
        char c = (char)b;
        if (std::isprint((unsigned char)c)) cur += c;
        else {
            if (cur.size() >= 6) {
                std::string l = cur;
                if (l.find("http") != std::string::npos || l.find("Serial") != std::string::npos ||
                    l.find("cmd") != std::string::npos || l.find("Reg") != std::string::npos)
                    std::cout << "  " << l << "\n";
            }
            cur.clear();
        }
    }
    std::cout << "\nNhắc: import table cho biết API; kết hợp chuỗi + import = đoán hành vi.\n";
    return 0;
}
