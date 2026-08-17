// 10_serial_check.cpp — Thuật toán kiểm tra serial + keygen (minh hoạ)
// Mục tiêu: cho thấy vì sao kiểm tra serial client-side dễ bị reverse; từ đó
//           hiểu cách bảo vệ (server-side, chữ ký số).
// An toàn: thuần tính toán trên chuỗi giả, không liên quan phần mềm thật.  g++ -std=c++17 -O2 serial_check.cpp -o demo
#include <iostream>
#include <string>
#include <cctype>

// "Hash" đơn giản: tổng trọng số các ký tự (minh hoạ, không phải hàm băm an toàn).
unsigned hash_name(const std::string& name) {
    unsigned h = 0x811C9DC5u;
    for (char c : name) { h ^= (unsigned char)std::toupper(c); h *= 0x01000193u; }
    return h;
}
std::string derive_serial(const std::string& name) {
    char buf[16]; std::snprintf(buf, sizeof(buf), "RE-%08X", hash_name("RE-" + name));
    return buf;
}
bool check(const std::string& name, const std::string& serial) {
    std::string s = serial; for (auto& c : s) c = std::toupper(c);
    return s == derive_serial(name);
}

int main(int argc, char** argv) {
    if (argc == 3 && std::string(argv[1]) == "--check") {
        std::cout << "Name: " << argv[2] << "\nSerial: " << derive_serial(argv[2]) << "\n";
    } else if (argc == 4 && std::string(argv[1]) == "--verify") {
        bool ok = check(argv[2], argv[3]);
        std::cout << "Name: " << argv[2] << "\nSerial: " << argv[3] << "\nKết quả: " << (ok ? "HỢP LỆ" : "KHÔNG hợp lệ") << "\n";
    } else {
        std::cout << "Dùng: ./demo --check <name>  |  ./demo --verify <name> <serial>\n";
        return 0;
    }
    std::cout << "\nBài học: thuật toán nằm trong client thì ai cũng reverse được.\n";
    std::cout << "Phòng thủ: kiểm tra phía server, ký số serial, chống debug.\n";
    return 0;
}
