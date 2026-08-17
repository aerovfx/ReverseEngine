// 01_env_check.cpp — Checklist môi trường lab macOS RE (ARM64)
// Mục tiêu: sinh checklist lab cô lập (VM, SIP/AMFI, công cụ) trước khi phân tích.
// An toàn: thuần tính toán, chỉ in văn bản.  g++ -std=c++17 -O2 env_check.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::pair<std::string,std::string>> ck = {
        {"Cô lập", "macOS VM Apple Silicon + snapshot"},
        {"SIP/AMFI", "chỉ tắt trong VM lab, không trên máy chính"},
        {"Công cụ", "Homebrew: lldb, Ghidra, otool, class-dump, Frida"},
        {"Mạng", "tắt mạng hoặc giả lập"},
        {"Hash", "ghi hash Mach-O trước khi phân tích"},
    };
    std::cout << "LAB CHECKLIST — macOS RE\n" << std::string(56,'=') << "\n\n";
    int i = 1;
    for (auto& [muc, ghi] : ck) std::cout << "[ ] " << i++ << ". " << muc << ": " << ghi << "\n";
    std::cout << "\nChỉ dựng lab trên máy bạn sở hữu; không tắt SIP ngoài VM.\n";
    return 0;
}
