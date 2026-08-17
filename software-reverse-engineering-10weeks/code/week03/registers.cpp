// 03_registers.cpp — Mô phỏng thanh ghi CPU x86/x64 + phân vùng bộ nhớ
// Mục tiêu: hiểu vai trò thanh ghi (EAX/EBX/EIP/ESP...) và layout bộ nhớ tiến trình.
// An toàn: thuần tính toán — mô phỏng CPU bằng struct.  g++ -std=c++17 -O2 registers.cpp -o demo
#include <iostream>
#include <string>
#include <cstdint>

struct Cpu {
    uint32_t eax=0, ebx=0, ecx=0, edx=0;   // thanh ghi đa dụng
    uint32_t eip=0x401000;                  // instruction pointer
    uint32_t esp=0x7fff0000;                // stack pointer
    bool zf=false;
};
void mov_imm(Cpu& c, uint32_t& reg, uint32_t v) { reg = v; }      // mov reg, imm
void cmp(Cpu& c, uint32_t a, uint32_t b)      { c.zf = (a==b); }  // cmp a,b

int main() {
    Cpu c;
    mov_imm(c, c.eax, 5);
    mov_imm(c, c.ebx, 5);
    cmp(c, c.eax, c.ebx);
    std::cout << "EAX=" << c.eax << " EBX=" << c.ebx << " -> ZF=" << c.zf << "\n";

    std::cout << "\nPhân vùng bộ nhớ tiến trình:\n";
    std::cout << "  0x00400000  .text (code, R-X)\n";
    std::cout << "  0x00600000  .data (RW-)\n";
    std::cout << "  0x7fff0000  stack  (RW-)\n";
    std::cout << "  0x10000000  heap   (RW-)\n";
    std::cout << "\nKết luận: hiểu thanh ghi + layout = đọc assembly có mục đích.\n";
    return 0;
}
