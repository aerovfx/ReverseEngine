// ============================================================================
// 05_cow.cpp — Copy-on-Write (chia sẻ extent + tách bản khi ghi)
//
// Mục tiêu: minh hoạ cách CubeCoW dùng reflink (FICLONE trên XFS) để clone
//           O(1): nhiều sandbox chia sẻ cùng một khối dữ liệu, chỉ khi ai đó
//           GHI thì khối đó mới được tách thành bản sao riêng (copy-on-write).
// Đầu vào : không.
// Đầu ra : bảng trạng thái chia sẻ/tách của từng khối trước và sau khi ghi.
// An toàn : THUẦN TÍNH TOÁN — mô phỏng extent bằng struct + refcount,
//           không thao tác filesystem/XFS thật.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 cow.cpp -o cow
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

// Một "extent" (khối dữ liệu) với đếm tham chiếu (refcount).
// refcount > 1 nghĩa là nhiều sandbox đang chia sẻ khối này.
struct Extent {
    std::string data;      // nội dung khối
    int         refs = 1;  // số sandbox đang tham chiếu
};

// Một sandbox giữ con trỏ tới các extent (chia sẻ với template/sandbox khác).
class SandboxDisk {
public:
    // Clone từ template: tăng refcount của từng extent (O(1), không copy byte).
    SandboxDisk(const SandboxDisk& other) : extents_(other.extents_) {
        for (auto& e : extents_) e->refs++;
    }

    SandboxDisk(std::vector<std::shared_ptr<Extent>> extents)
        : extents_(std::move(extents)) {}

    // Ghi vào extent idx: nếu đang bị chia sẻ (refs>1) thì tách bản riêng (CoW).
    void write(size_t idx, const std::string& new_data) {
        if (idx >= extents_.size()) return;
        auto& e = extents_[idx];
        if (e->refs > 1) {
            e->refs--;                       // bỏ tham chiếu khỏi bản chung
            e = std::make_shared<Extent>();  // tạo bản sao riêng (copy-on-write)
            e->data = new_data;
            e->refs = 1;
        } else {
            e->data = new_data;              // đã là bản riêng -> ghi trực tiếp
        }
    }

    // In trạng thái: khối nào đang "chia sẻ" hay "riêng".
    void dump(const std::string& label) const {
        std::cout << label << ": ";
        for (size_t i = 0; i < extents_.size(); ++i) {
            std::cout << "[" << i << "]"
                      << (extents_[i]->refs > 1 ? "shared" : "own")
                      << "(\"" << extents_[i]->data << "\") ";
        }
        std::cout << "\n";
    }

private:
    std::vector<std::shared_ptr<Extent>> extents_;
};

int main() {
    std::cout << "COPY-ON-WRITE — clone O(1) + tách bản khi ghi\n";
    std::cout << std::string(70, '=') << "\n\n";

    // Template: 4 extent.
    std::vector<std::shared_ptr<Extent>> tpl;
    for (const char* s : {"rootfs", "libc", "config", "cache"}) {
        tpl.push_back(std::make_shared<Extent>());
        tpl.back()->data = s;
    }
    SandboxDisk template_disk(tpl);

    // Sandbox A và B clone từ template (chỉ tăng refcount, không copy byte).
    SandboxDisk a = template_disk;
    SandboxDisk b = template_disk;

    template_disk.dump("Template");
    a.dump("Sandbox A");
    b.dump("Sandbox B");

    std::cout << "\nSandbox B ghi vào extent[1] (copy-on-write):\n";
    b.write(1, "libc-patched");

    a.dump("Sandbox A");
    b.dump("Sandbox B");

    std::cout << "\n" << std::string(70, '-') << "\n";
    std::cout << "KẾT LUẬN: chỉ extent bị GHI mới được copy riêng (CoW);\n";
    std::cout << "các extent còn lại vẫn chia sẻ -> clone/snapshot O(1) về dung lượng.\n";
    return 0;
}
