// 02_pe_triage.cpp — PE static triage (DIE-like): định dạng + gợi ý compiler
// Mục tiêu: bước đầu phân loại binary trước khi đưa vào debugger.
// An toàn: chỉ ĐỌC header + quét chuỗi.  g++ -std=c++17 -O2 pe_triage.cpp -o demo
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

uint32_t rd32(const std::vector<uint8_t>& d, size_t o){ return (uint32_t)d[o]|(d[o+1]<<8)|(d[o+2]<<16)|(d[o+3]<<24); }
uint16_t rd16(const std::vector<uint8_t>& d, size_t o){ return d[o]|(d[o+1]<<8); }

int main(int argc, char** argv) {
    if (argc < 2) { std::cout << "Dùng: ./demo <file>\n"; return 1; }
    std::ifstream f(argv[1], std::ios::binary);
    std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), {});
    if (d.size() < 2) return 1;

    if (d[0]=='M' && d[1]=='Z') {
        std::cout << "PE (Windows)\n";
        if (d.size() >= 0x40) {
            uint32_t e = rd32(d, 0x3C);
            if (e+6 <= d.size() && d[e]=='P' && d[e+1]=='E') {
                size_t opt = e + 4 + 20;
                uint16_t magic = rd16(d, opt);
                std::cout << "  " << (magic==0x20B ? "PE32+ (x64)" : "PE32 (x86)") << "\n";
                std::cout << "  Số section: " << rd16(d, opt+2) << "\n";
                std::cout << "  EntryPoint RVA: 0x" << std::hex << rd32(d, opt+16) << std::dec << "\n";
            }
        }
    } else { std::cout << "Không phải PE.\n"; return 0; }

    std::string s(d.begin(), d.begin() + std::min<size_t>(d.size(), 1<<20));
    std::cout << "Compiler: ";
    if (s.find("MinGW") != std::string::npos)       std::cout << "MinGW (GCC)";
    else if (s.find("GCC:") != std::string::npos)   std::cout << "GCC";
    else if (s.find("MSVC") != std::string::npos)   std::cout << "MSVC";
    else if (s.find("UPX") != std::string::npos)    std::cout << "có thể bị pack UPX";
    else                                            std::cout << "không rõ";
    std::cout << "\n";
    return 0;
}
