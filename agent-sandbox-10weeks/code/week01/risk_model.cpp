// ============================================================================
// 01_risk_model.cpp — Phân loại rủi ro khi AI agent gọi tool (mô hình đe doạ)
//
// Mục tiêu: minh hoạ cách xây mô hình đe doạ dựa trên NĂNG LỰC (capability)
//           mà một tool cần. Tool cần càng nhiều năng lực nguy hiểm -> rủi ro
//           càng cao -> càng bắt buộc phải chạy trong sandbox.
// Đầu vào : không (dữ liệu demo có sẵn trong bảng TOOLS).
// Đầu ra : bảng phân loại rủi ro theo 4 mức + tổng kết.
// An toàn : chương trình THUẦN TÍNH TOÁN — không chạy tool/lệnh thật,
//           không đụng filesystem/mạng. Có thể biên dịch và chạy ở mọi nơi.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 risk_model.cpp -o risk_model
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

// Các "năng lực" nguy hiểm mà một tool có thể yêu cầu.
// Đây chính là các trục trong mô hình đe doạ của sandbox.
enum class Capability {
    ReadOnly,   // chỉ đọc dữ liệu
    WriteFile,  // ghi / xoá file
    Exec,       // chạy tiến trình con
    Network,    // gửi dữ liệu ra ngoài
    Install,    // cài package / code mới
};

// Một tool: tên + danh sách năng lực nó cần.
struct Tool {
    std::string name;
    std::vector<Capability> caps;
};

// Trả về mức rủi ro: 0=thấp, 1=trung bình, 2=cao, 3=nghiêm trọng.
int risk_level(const Tool& t) {
    // Mọi tool có Exec hoặc Install đều là "nghiêm trọng" —
    // vì nó có thể chạy code tuỳ ý nếu không bị cô lập.
    for (Capability c : t.caps) {
        if (c == Capability::Install) return 3;   // cài mã mới = kiểm soát hoàn toàn
        if (c == Capability::Exec)     return 2;   // chạy lệnh = nguy hiểm cao
    }
    bool writes  = false, nets = false;
    for (Capability c : t.caps) {
        if (c == Capability::WriteFile) writes = true;
        if (c == Capability::Network)   nets   = true;
    }
    if (writes || nets) return 1;                 // ghi file / gọi mạng = trung bình
    return 0;                                     // chỉ đọc = thấp
}

// In tên năng lực ra để hiển thị bảng.
std::string cap_name(Capability c) {
    switch (c) {
        case Capability::ReadOnly:  return "read";
        case Capability::WriteFile: return "write";
        case Capability::Exec:      return "exec";
        case Capability::Network:   return "network";
        case Capability::Install:   return "install";
    }
    return "?";
}

int main() {
    // Dữ liệu demo: các tool điển hình một coding agent hay gọi.
    const std::vector<Tool> tools = {
        {"read_file",   {Capability::ReadOnly}},
        {"list_dir",    {Capability::ReadOnly}},
        {"write_file",  {Capability::WriteFile}},
        {"http_request",{Capability::Network}},
        {"run_shell",   {Capability::Exec}},
        {"install_pkg", {Capability::Install, Capability::Network}},
    };

    const char* levels[] = {"THẤP", "TRUNG BÌNH", "CAO", "NGHIÊM TRỌNG"};

    std::cout << "MÔ HÌNH ĐE DOẠ — phân loại rủi ro theo năng lực của tool\n";
    std::cout << std::string(70, '=') << "\n\n";

    // Sắp xếp tool theo mức rủi ro tăng dần để dễ đọc.
    std::vector<Tool> sorted = tools;
    std::sort(sorted.begin(), sorted.end(),
              [](const Tool& a, const Tool& b) { return risk_level(a) < risk_level(b); });

    std::cout << "Tool            Rủi ro        Năng lực cần\n";
    std::cout << std::string(70, '-') << "\n";
    for (const Tool& t : sorted) {
        std::cout << t.name;
        std::cout << std::string(16 - t.name.size(), ' ');
        std::cout << levels[risk_level(t)];
        std::cout << std::string(15 - std::string(levels[risk_level(t)]).size(), ' ');
        for (Capability c : t.caps) std::cout << cap_name(c) << " ";
        std::cout << "\n";
    }

    // Kết luận cốt lõi của bài học.
    std::cout << "\n" << std::string(70, '-') << "\n";
    std::cout << "KẾT LUẬN: mọi tool cần 'exec' hoặc 'install' đều phải chạy trong\n";
    std::cout << "sandbox cô lập. Đây chính là lý do tồn tại của CubeSandbox.\n";
    return 0;
}
