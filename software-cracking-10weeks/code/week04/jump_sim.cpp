// 04_jump_sim.cpp — Mô phỏng CMP + cờ ZF + nhảy JE/JNE
// Mục tiêu: hiểu vì sao JE/JNE nhảy hay không dựa vào cờ ZF.
// An toàn: thuần tính toán.  g++ -std=c++17 -O2 jump_sim.cpp -o demo
#include <iostream>
#include <cstdint>

// Mô phỏng một CPU tối giản với thanh ghi + cờ.
struct Cpu { uint32_t eax = 0, ebx = 0; bool zf = false; };

void cmp(Cpu& c, uint32_t a, uint32_t b) { c.zf = (a == b); }   // CMP a,b -> ZF=1 nếu bằng
bool je(Cpu& c)  { return c.zf; }                                // JE nhảy khi ZF==1
bool jne(Cpu& c) { return !c.zf; }                               // JNE nhảy khi ZF==0

int main() {
    const uint32_t serial = 0x1337;
    for (uint32_t input : {0x1337u, 0x1234u}) {
        Cpu c; cmp(c, input, serial);
        std::cout << "input=0x" << std::hex << input
                  << "  correct=0x" << serial << std::dec
                  << "  -> ZF=" << c.zf << "\n";
        std::cout << "   JE  (jump if equal)     : " << (je(c) ? "NHẢY (nhánh đúng)" : "không nhảy") << "\n";
        std::cout << "   JNE (jump if not equal) : " << (jne(c) ? "NHẢY (nhánh sai)" : "không nhảy") << "\n";
        std::cout << std::string(50, '-') << "\n";
    }
    std::cout << "Kết luận: 'đảo nhảy' = đổi JE<->JNE hoặc lật cờ ZF (1<->0).\n";
    return 0;
}
