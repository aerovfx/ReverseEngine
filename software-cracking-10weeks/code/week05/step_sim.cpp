// 05_step_sim.cpp — Mô phỏng step over (F8) vs step into (F7)
// Mục tiêu: phân biệt hai thao tác stepping trong debugger.
// An toàn: thuần tính toán (danh sách lệnh giả).  g++ -std=c++17 -O2 step_sim.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

struct Ins { std::string addr, code, func; };

int main() {
    std::vector<Ins> prog = {
        {"00401000", "MOV EAX, 5", "main"},
        {"00401005", "CALL 00402000", "main"},
        {"0040100A", "CMP EAX, 0", "main"},
        {"00402000", "PUSH EBP", "check"},
        {"00402004", "MOV EAX, 1", "check"},
        {"00402008", "RET", "check"},
    };
    std::cout << "== STEP OVER (F8) — lướt qua CALL, không vào thân hàm ==\n";
    for (size_t i = 0; i < prog.size();) {
        const auto& in = prog[i];
        std::cout << "  " << in.addr << "  " << in.code << "  [" << in.func << "]\n";
        if (in.code.rfind("CALL", 0) == 0) {          // gặp CALL
            ++i;
            while (i < prog.size() && prog[i].func != "main") ++i;  // nhảy qua thân hàm
        } else ++i;
    }
    std::cout << "\n== STEP INTO (F7) — đi sâu vào bên trong CALL ==\n";
    for (const auto& in : prog)
        std::cout << "  " << in.addr << "  " << in.code << "  [" << in.func << "]\n";
    std::cout << "\nNhận xét: step into thấy cả thân hàm check(); step over lướt qua.\n";
    return 0;
}
