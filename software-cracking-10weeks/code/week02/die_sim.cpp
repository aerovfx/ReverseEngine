// 02_die_sim.cpp — Mô phỏng Detect It Easy: nhận diện định dạng + gợi ý compiler
// Mục tiêu: đọc magic bytes và đặc trưng header để đoán PE/ELF/Mach-O + compiler.
// An toàn: chỉ ĐỌC đầu file, không thực thi binary.  g++ -std=c++17 -O2 die_sim.cpp -o demo
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

int main(int argc, char** argv) {
    if (argc < 2) { std::cout << "Cách dùng: ./demo <file>\n"; return 1; }
    std::ifstream f(argv[1], std::ios::binary);
    if (!f) { std::cout << "Không mở được file.\n"; return 1; }
    std::vector<char> data((std::istreambuf_iterator<char>(f)), {});
    std::cout << "File: " << argv[1] << " (" << data.size() << " bytes)\n";

    // 1. Magic bytes.
    auto starts = [&](const char* s){ return data.size() >= 4 && std::string(data.begin(), data.begin()+4) == s; };
    if (starts("MZ"))                    std::cout << "Định dạng: PE (Windows)\n";
    else if (data.size() >= 4 && (unsigned char)data[0] == 0x7F && data[1]=='E' && data[2]=='L' && data[3]=='F')
                                          std::cout << "Định dạng: ELF (Linux)\n";
    else if (starts("\xCF\xFA\xED\xFE"))  std::cout << "Định dạng: Mach-O 32-bit (little-endian)\n";
    else if (starts("\xFE\xED\xFA\xCF"))  std::cout << "Định dạng: Mach-O 32-bit (big-endian)\n";
    else                                 std::cout << "Định dạng: không nhận diện (text/script?)\n";

    // 2. Gợi ý compiler từ chuỗi trong phần đầu.
    std::string s(data.begin(), data.begin() + std::min<size_t>(data.size(), 1 << 20));
    auto has = [&](const char* p){ return s.find(p) != std::string::npos; };
    std::cout << "Compiler : ";
    if (has("MinGW"))          std::cout << "MinGW (GCC)";
    else if (has("GCC:"))      std::cout << "GCC";
    else if (has("MSVC"))      std::cout << "Microsoft Visual C++";
    else if (has(".NETFramework")) std::cout << ".NET (C#/VB.NET)";
    else if (has("UPX"))       std::cout << "có thể bị pack UPX";
    else                       std::cout << "không rõ (dùng DIE thật để xác nhận)";
    std::cout << "\n";
    return 0;
}
