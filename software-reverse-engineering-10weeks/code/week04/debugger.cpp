// 04_debugger.cpp — Mô phỏng debugger: stepping + breakpoint (INT3)
// Mục tiêu: hiểu F7/F8 stepping, software BP, và cách debugger dừng tại địa chỉ.
// An toàn: thuần tính toán trên danh sách lệnh giả.  g++ -std=c++17 -O2 debugger.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

struct Ins { std::string addr, code; };

int main(int argc, char** argv) {
    std::string bp = (argc > 1) ? argv[1] : "00401005";
    std::vector<Ins> prog = {
        {"00401000", "MOV EAX, [input]"},
        {"00401005", "CMP EAX, serial"},
        {"0040100A", "JE  success"},
        {"0040100F", "CALL MessageBoxA"},
    };
    std::cout << "Step (F8) tới breakpoint " << bp << ":\n\n";
    for (auto& in : prog) {
        if (in.addr == bp) { std::cout << "  [BP hit] " << in.addr << ": " << in.code << "\n"; break; }
        std::cout << "  " << in.addr << "  " << in.code << "\n";
    }
    std::cout << "\nNhắc: F2 đặt software BP (INT3); F7 step into; F8 step over; F9 run.\n";
    return 0;
}
