// 06_breakpoint_sim.cpp — Mô phỏng breakpoint phần mềm (INT3 / 0xCC)
// Mục tiêu: minh hoạ debugger dừng khi CPU chạm lệnh INT3 tại địa chỉ đặt BP.
// An toàn: thuần tính toán, không ghi INT3 vào binary thật.  g++ -std=c++17 -O2 breakpoint_sim.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

struct Ins { std::string addr, code; };

int main(int argc, char** argv) {
    std::string bp = (argc > 1) ? argv[1] : "0040100A";
    std::vector<Ins> prog = {
        {"00401000", "MOV EAX, [serial]"},
        {"00401005", "CMP EAX, 0x1337"},
        {"0040100A", "JE  00401020"},
        {"0040100F", "PUSH 'try harder'"},
        {"00401014", "CALL MessageBox"},
        {"00401020", "PUSH 'correct'"},
        {"00401025", "CALL MessageBox"},
    };
    std::cout << "Đặt software BP (INT3) tại " << bp << "\n\n";
    for (const auto& in : prog) {
        if (in.addr == bp) { std::cout << "  [BP hit] dừng tại " << in.addr << ": " << in.code << "\n"; return 0; }
        std::cout << "  " << in.addr << "  " << in.code << "\n";
    }
    std::cout << "  (kết thúc, không chạm BP)\n";
    return 0;
}
