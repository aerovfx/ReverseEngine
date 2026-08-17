// 03_pe_entry_point.cpp — Đọc ImageBase + AddressOfEntryPoint từ PE (thật)
// Mục tiêu: tính EntryPoint = ImageBase + AddressOfEntryPoint, không cần debugger.
// An toàn: chỉ ĐỌC header, không thực thi.  g++ -std=c++17 -O2 pe_entry_point.cpp -o demo
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

uint32_t rd32(const std::vector<uint8_t>& d, size_t off) {
    return uint32_t(d[off]) | (uint32_t(d[off+1])<<8) | (uint32_t(d[off+2])<<16) | (uint32_t(d[off+3])<<24);
}
uint16_t rd16(const std::vector<uint8_t>& d, size_t off) { return uint16_t(d[off]) | (uint16_t(d[off+1])<<8); }

int main(int argc, char** argv) {
    if (argc < 2) { std::cout << "Cách dùng: ./demo <file.exe>\n"; return 1; }
    std::ifstream f(argv[1], std::ios::binary);
    std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), {});
    if (d.size() < 64 || !(d[0]=='M' && d[1]=='Z')) { std::cout << "Không phải PE.\n"; return 1; }

    uint32_t e_lfanew = rd32(d, 0x3C);
    if (d[e_lfanew]!='P' || d[e_lfanew+1]!='E') { std::cout << "Không tìm thấy chữ ký PE.\n"; return 1; }

    size_t opt = e_lfanew + 4 + 20;
    uint16_t magic = rd16(d, opt);
    if (magic == 0x10B) {                    // PE32
        uint32_t image_base = rd32(d, opt+28);
        uint32_t entry_rva  = rd32(d, opt+16);
        uint16_t nsect      = rd16(d, opt+2);
        std::cout << "Kiến trúc: PE32 (x86)\n";
        std::cout << "ImageBase: 0x" << std::hex << image_base << "\n";
        std::cout << "AddressOfEntryPoint: 0x" << entry_rva << " (RVA)\n";
        std::cout << "EntryPoint(VA): 0x" << (image_base + entry_rva) << std::dec << "\n";
        std::cout << "Số section: " << nsect << "\n";
    } else if (magic == 0x20B) {             // PE32+
        uint64_t image_base = rd32(d, opt+24);   // đọc 32-bit thấp (đơn giản hoá)
        uint32_t entry_rva  = rd32(d, opt+16);
        std::cout << "Kiến trúc: PE32+ (x64)\n";
        std::cout << "ImageBase: 0x" << std::hex << image_base << "\n";
        std::cout << "EntryPoint(VA): 0x" << (image_base + entry_rva) << std::dec << "\n";
    } else {
        std::cout << "Magic optional header lạ: 0x" << std::hex << magic << std::dec << "\n";
    }
    std::cout << "Công thức: EntryPoint = ImageBase + AddressOfEntryPoint\n";
    return 0;
}
