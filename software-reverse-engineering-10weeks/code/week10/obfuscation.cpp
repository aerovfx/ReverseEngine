// 10_obfuscation.cpp — Mô phỏng obfuscation/deobfuscation + phân tích DLL export
// Mục tiêu: hiểu kỹ thuật làm rối (đổi tên, mã hoá chuỗi) và cách khôi phục.
// An toàn: thuần tính toán trên chuỗi.  g++ -std=c++17 -O2 obfuscation.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

// "Deobfuscate" chuỗi bị XOR đơn giản (minh hoạ kỹ thuật làm rối).
std::string xor_dec(const std::string& s, char key) {
    std::string r = s; for (auto& c : r) c ^= key; return r;
}

int main() {
    std::cout << "OBFUSCATION / DEOBFUSCATION\n" << std::string(56,'=') << "\n\n";

    // Chuỗi bị obfuscate (XOR 0x2A) rồi giải mã lại.
    std::string obf = xor_dec("c2.example", 0x2A);
    std::cout << "Chuỗi obfuscated (hex): ";
    for (unsigned char c : obf) std::cout << std::hex << (int)c << " ";
    std::cout << std::dec << "\n";
    std::cout << "Deobfuscate: " << xor_dec(obf, 0x2A) << "\n";

    std::cout << "\nDLL exports (mô phỏng):\n";
    for (const std::string& e : {"DllMain", "Install", "GetKey"})
        std::cout << "  export: " << e << "\n";

    std::cout << "\nKết luận: obfuscation làm chậm RE; deobfuscation + phân tích export\n";
    std::cout << "giúp khôi phục luồng. Bảo vệ app bằng anti-debug + anti-tamper.\n";
    return 0;
}
