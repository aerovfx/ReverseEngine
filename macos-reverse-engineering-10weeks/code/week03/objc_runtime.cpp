// 03_objc_runtime.cpp — Mô phỏng Objective-C runtime: class, isa, method list
// Mục tiêu: hiểu cấu trúc ObjC (isa pointer, method list) — nền tảng RE macOS.
// An toàn: thuần tính toán — mô phỏng runtime, không đụng ObjC thật.  g++ -std=c++17 -O2 objc_runtime.cpp -o demo
#include <iostream>
#include <string>
#include <vector>
#include <map>

// Mô phỏng một class ObjC với danh sách method (SEL -> implementation).
struct ObjCClass {
    std::string name;
    std::map<std::string, std::string> methods;   // selector -> imp (mô phỏng)
    void add(const std::string& sel, const std::string& imp) { methods[sel] = imp; }
    void dump() const {
        std::cout << "Class: " << name << "\n";
        for (auto& [sel, imp] : methods)
            std::cout << "  -[" << name << " " << sel << "] -> " << imp << "\n";
    }
};

int main() {
    ObjCClass License {
        "LicenseManager",
        {{"checkSerial:", "0x100003e00"}, {"isRegistered", "0x100003f20"}, {"activate:", "0x100004010"}}
    };
    std::cout << "OBJC RUNTIME — isa + method list\n" << std::string(56,'=') << "\n\n";
    License.dump();
    std::cout << "\nKết luận: class-dump liệt kê method; RE tìm method kiểm tra license\n";
    std::cout << "rồi hook (swizzle) hoặc phân tích implementation của nó.\n";
    return 0;
}
