// 06_swizzle_demo.cpp — Mô phỏng method swizzling (ObjC runtime hook)
// Mục tiêu: hiểu cách hoán đổi implementation của một method để quan sát/hook.
// An toàn: thuần tính toán — mô phỏng swizzle bằng map, không hook process thật.
// Biên dịch: g++ -std=c++17 -O2 swizzle_demo.cpp -o demo
#include <iostream>
#include <string>
#include <map>

class Runtime {
public:
    std::string& imp(const std::string& sel) { return methods_[sel]; }
    // Swizzle: hoán đổi implementation của hai selector.
    void swizzle(const std::string& a, const std::string& b) { std::swap(methods_[a], methods_[b]); }
    void dump(const std::string& sel) const {
        auto it = methods_.find(sel);
        std::cout << "  " << sel << " -> " << (it != methods_.end() ? it->second : "?") << "\n";
    }
private:
    std::map<std::string, std::string> methods_;
};

int main() {
    Runtime rt;
    rt.imp("checkSerial:") = "original_check (kiểm tra license)";
    rt.imp("fakeCheck:")   = "fake (luôn trả đúng)";

    std::cout << "Trước swizzle:\n"; rt.dump("checkSerial:"); rt.dump("fakeCheck:");
    rt.swizzle("checkSerial:", "fakeCheck:");
    std::cout << "\nSau swizzle:\n";  rt.dump("checkSerial:"); rt.dump("fakeCheck:");

    std::cout << "\nKết luận: swizzle hoán đổi imp để hook — dùng hợp pháp cho app của bạn,\n";
    std::cout << "quan sát/hiểu cơ chế; không dùng để né license của phần mềm người khác.\n";
    return 0;
}
