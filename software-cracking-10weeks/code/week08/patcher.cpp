// 08_patcher.cpp — Vá byte trên BẢN SAO của file + hash trước/sau (thật)
// Mục tiêu: minh hoạ quy trình patch an toàn (backup + hash + vá + xác minh).
// An toàn: KHÔNG sửa file gốc; chỉ tạo bản sao đã vá.
// Biên dịch: g++ -std=c++17 -O2 patcher.cpp -o demo
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>
#include <sstream>

// FNV-1a 64-bit — hash đơn giản để so sánh trước/sau (minh hoạ; thực tế nên dùng SHA-256).
uint64_t fnv1a(const std::vector<uint8_t>& d) {
    uint64_t h = 0xcbf29ce484222325ULL;
    for (uint8_t b : d) { h ^= b; h *= 0x100000001b3ULL; }
    return h;
}
std::string hex64(uint64_t v) {
    std::ostringstream os; os << std::hex << std::setw(16) << std::setfill('0') << v; return os.str();
}

int main(int argc, char** argv) {
    if (argc < 4) { std::cout << "Cách dùng: ./demo <file> <offset_hex> <bytes_hex>\n"; return 1; }
    std::string src = argv[1];
    size_t offset = std::stoul(argv[2], nullptr, 16);
    std::string hex = argv[3];
    std::vector<uint8_t> patch;
    for (size_t i = 0; i + 1 < hex.size(); i += 2)
        patch.push_back((uint8_t)std::stoul(hex.substr(i, 2), nullptr, 16));

    std::ifstream in(src, std::ios::binary);
    std::vector<uint8_t> d((std::istreambuf_iterator<char>(in)), {});
    std::cout << "Hash TRƯỚC (" << src << "): " << hex64(fnv1a(d)) << "\n";

    if (offset + patch.size() > d.size()) { std::cout << "Offset vượt kích thước file.\n"; return 1; }
    for (size_t i = 0; i < patch.size(); ++i) d[offset + i] = patch[i];

    std::string out = src + ".patched";
    std::ofstream of(out, std::ios::binary);
    of.write((char*)d.data(), d.size());
    std::cout << "Đã vá " << patch.size() << " byte tại offset 0x" << std::hex << offset << std::dec << " -> " << out << "\n";
    std::cout << "Hash SAU  (" << out << "): " << hex64(fnv1a(d)) << "\n";
    std::cout << "\nLưu ý: chỉ vá bản sao; giữ nguyên file gốc để rollback.\n";
    return 0;
}
