// 09_license_check.cpp — Thuật toán kiểm tra license + keygen (minh hoạ)
// Mục tiêu: hiểu vì sao kiểm tra license client-side dễ bị reverse; cách bảo vệ.
// An toàn: thuần tính toán trên chuỗi giả; chỉ cho app của bạn/crackme.
// Biên dịch: g++ -std=c++17 -O2 license_check.cpp -o demo
#include <iostream>
#include <string>
#include <cctype>

unsigned hash_name(const std::string& n) {
    unsigned h = 2166136261u;
    for (char c : n) { h ^= (unsigned char)std::toupper(c); h *= 16777619u; }
    return h;
}
std::string derive(const std::string& name) {
    char b[16]; std::snprintf(b, sizeof(b), "LIC-%08X", hash_name("APP-" + name));
    return b;
}
bool check(const std::string& n, const std::string& l) {
    std::string s = l; for (auto& c : s) c = std::toupper(c);
    return s == derive(n);
}

int main(int argc, char** argv) {
    if (argc == 3 && std::string(argv[1]) == "--keygen") {
        std::cout << "Name: " << argv[2] << "  License: " << derive(argv[2]) << "\n";
    } else if (argc == 4 && std::string(argv[1]) == "--check") {
        std::cout << (check(argv[2], argv[3]) ? "HỢP LỆ" : "KHÔNG hợp lệ") << "\n";
    } else {
        std::cout << "Dùng: ./demo --keygen <name> | ./demo --check <name> <license>\n";
        return 0;
    }
    std::cout << "\nBài học: license client-side dễ reverse; bảo vệ bằng server-side + signing.\n";
    return 0;
}
