// 06_patcher.cpp — Secure patching: vá byte trên bản sao + hash + rollback
// Mục tiêu: minh hoạ quy trình vá an toàn (NOP/byte patch, hash, bản sao).
// An toàn: KHÔNG sửa file gốc; chỉ tạo bản sao.  g++ -std=c++17 -O2 patcher.cpp -o demo
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>
#include <sstream>

uint64_t fnv1a(const std::vector<uint8_t>& d) {
    uint64_t h = 0xcbf29ce484222325ULL; for (uint8_t b : d) { h ^= b; h *= 0x100000001b3ULL; } return h;
}
int main(int argc, char** argv) {
    if (argc < 4) { std::cout << "Dùng: ./demo <file> <offset_hex> <bytes_hex>\n"; return 1; }
    size_t off = std::stoul(argv[2], nullptr, 16);
    std::string hex = argv[3];
    std::vector<uint8_t> patch;
    for (size_t i = 0; i+1 < hex.size(); i += 2) patch.push_back((uint8_t)std::stoul(hex.substr(i,2), nullptr, 16));

    std::ifstream in(argv[1], std::ios::binary);
    std::vector<uint8_t> d((std::istreambuf_iterator<char>(in)), {});
    std::cout << "Hash trước: " << std::hex << fnv1a(d) << std::dec << "\n";
    if (off + patch.size() > d.size()) { std::cout << "Offset vượt kích thước.\n"; return 1; }
    for (size_t i = 0; i < patch.size(); ++i) d[off+i] = patch[i];
    std::string out = std::string(argv[1]) + ".patched";
    std::ofstream of(out, std::ios::binary); of.write((char*)d.data(), d.size());
    std::cout << "Đã vá " << patch.size() << " byte -> " << out << "\n";
    std::cout << "Hash sau : " << std::hex << fnv1a(d) << std::dec << "\n";
    std::cout << "\nNhắc: luôn vá bản sao; giữ hash + manifest để rollback.\n";
    return 0;
}
