// 02_macho_parser.cpp — Đọc header Mach-O + nhận diện fat/universal binary
// Mục tiêu: magic bytes, cputype (arm64/x86_64), filetype, số load commands.
// An toàn: chỉ ĐỌC header, không thực thi.  g++ -std=c++17 -O2 macho_parser.cpp -o demo
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

uint32_t rd32(const std::vector<uint8_t>& d, size_t o){ return (uint32_t)d[o]|(d[o+1]<<8)|(d[o+2]<<16)|(d[o+3]<<24); }

int main(int argc, char** argv) {
    if (argc < 2) { std::cout << "Dùng: ./demo <file>\n"; return 1; }
    std::ifstream f(argv[1], std::ios::binary);
    std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), {});
    if (d.size() < 4) return 1;

    uint32_t magic = rd32(d, 0);
    if (magic == 0xCAFEBABE) {
        std::cout << "FAT / UNIVERSAL binary (" << rd32(d, 4) << " kiến trúc)\n";
    } else if (magic == 0xFEEDFACF) {
        std::cout << "Mach-O 64-bit (little-endian)\n";
    } else if (magic == 0xFEEDFACE) {
        std::cout << "Mach-O 32-bit (little-endian)\n";
    } else if (magic == 0xCFFAEDFE) {
        std::cout << "Mach-O 64-bit (big-endian)\n";
    } else {
        std::cout << "Không phải Mach-O (magic 0x" << std::hex << magic << std::dec << ")\n";
        return 0;
    }

    if (d.size() >= 32) {
        uint32_t cpu = rd32(d, 4);
        uint32_t filetype = rd32(d, 12);
        uint32_t ncmds = rd32(d, 16);
        std::cout << "  cputype: " << (cpu == 0x0100000C ? "ARM64" : cpu == 0x01000007 ? "x86_64" : "khác") << "\n";
        std::cout << "  filetype: " << filetype << "\n";
        std::cout << "  load commands: " << ncmds << "\n";
    }
    std::cout << "Kích thước: " << d.size() << " bytes\n";
    return 0;
}
