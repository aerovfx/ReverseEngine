// 08_keygen.cpp — Thuật toán keygen (sinh serial hợp lệ) — minh hoạ
// Mục tiêu: hiểu thuật toán sinh serial từ tên; vì sao client-side dễ bị reverse.
// An toàn: thuần tính toán; chỉ cho app của bạn/crackme.  g++ -std=c++17 -O2 keygen.cpp -o demo
#include <iostream>
#include <string>
#include <cctype>

unsigned hash(const std::string& s) {
    unsigned h = 5381;
    for (char c : s) h = h*33 ^ (unsigned char)std::toupper(c);
    return h;
}
std::string keygen(const std::string& name) {
    char b[16]; std::snprintf(b, sizeof(b), "KEY-%08X", hash("APP-"+name));
    return b;
}
int main(int argc, char** argv) {
    if (argc < 2) { std::cout << "Dùng: ./demo <name>\n"; return 1; }
    std::cout << "Name: " << argv[1] << "  Serial: " << keygen(argv[1]) << "\n";
    std::cout << "\nBài học: keygen cho thấy thuật toán client-side dễ reverse;\n";
    std::cout << "bảo vệ bằng kiểm tra server + chữ ký số (Ed25519).\n";
    return 0;
}
