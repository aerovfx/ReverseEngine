// ============================================================================
// 03_pipeline.cpp — Mô phỏng vòng đời request Sandbox.create() (control/data plane)
//
// Mục tiêu: minh hoạ kiến trúc phân tầng của CubeSandbox bằng OOP:
//   Client -> CubeAPI -> CubeMaster -> Cubelet -> CubeShim -> CubeHypervisor
//   Mỗi thành phần là một lớp có handle(), ghi log rồi chuyển tiếp bước kế.
// Đầu vào : không.
// Đầu ra : vết (trace) từng bước của một request tạo sandbox.
// An toàn : THUẦN TÍNH TOÁN — chỉ in log, không gọi mạng/gRPC thật.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 pipeline.cpp -o pipeline
// ============================================================================

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// Lớp cơ sở cho mọi thành phần trong pipeline.
// handle() trả về mô tả kết quả để in ra trace.
class Component {
public:
    explicit Component(std::string name) : name_(std::move(name)) {}
    virtual ~Component() = default;

    const std::string& name() const { return name_; }
    // Thực hiện công việc của thành phần này (mô phỏng).
    virtual std::string handle() = 0;

protected:
    std::string name_;
};

// Thành phần đơn giản chỉ in một thông điệp (dùng cho các node ở biên).
class Step : public Component {
public:
    Step(std::string name, std::string action)
        : Component(std::move(name)), action_(std::move(action)) {}
    std::string handle() override { return action_; }
private:
    std::string action_;
};

// CubeHypervisor: bước cuối — tạo microVM (KVM/RustVMM).
class CubeHypervisor : public Component {
public:
    CubeHypervisor() : Component("CubeHypervisor") {}
    std::string handle() override {
        return "tạo microVM (vCPU + memory + virtio) rồi restore từ snapshot";
    }
};

// CubeShim: chuẩn bị rootfs/kernel rồi khởi động hypervisor.
class CubeShim : public Component {
public:
    CubeShim(std::shared_ptr<CubeHypervisor> next)
        : Component("CubeShim"), next_(std::move(next)) {}
    std::string handle() override {
        std::string r = "chuẩn bị rootfs + memory file + kernel; khởi động ";
        r += next_->name() + " → " + next_->handle();
        return r;
    }
private:
    std::shared_ptr<CubeHypervisor> next_;
};

// Cubelet: agent node-local, gọi containerd Shim v2 + gắn mạng.
class Cubelet : public Component {
public:
    Cubelet(std::shared_ptr<CubeShim> next)
        : Component("Cubelet"), next_(std::move(next)) {}
    std::string handle() override {
        std::string r = "clone rootfs (CubeCoW) → containerd Shim v2 → ";
        r += next_->name() + " → " + next_->handle();
        return r;
    }
private:
    std::shared_ptr<CubeShim> next_;
};

// CubeMaster: lập lịch, chọn node theo tài nguyên.
class CubeMaster : public Component {
public:
    CubeMaster(std::shared_ptr<Cubelet> next)
        : Component("CubeMaster"), next_(std::move(next)) {}
    std::string handle() override {
        std::string r = "chọn node theo tài nguyên → gửi gRPC tới ";
        r += next_->name() + " → " + next_->handle();
        return r;
    }
private:
    std::shared_ptr<Cubelet> next_;
};

// CubeAPI: REST gateway tương thích E2B.
class CubeAPI : public Component {
public:
    CubeAPI(std::shared_ptr<CubeMaster> next)
        : Component("CubeAPI"), next_(std::move(next)) {}
    std::string handle() override {
        std::string r = "nhận POST /sandboxes (E2B) → gRPC tới ";
        r += next_->name() + " → " + next_->handle();
        return r;
    }
private:
    std::shared_ptr<CubeMaster> next_;
};

int main() {
    // Dựng pipeline theo chiều ngược (dưới lên) để gắn con trỏ next.
    auto hypervisor = std::make_shared<CubeHypervisor>();
    auto shim       = std::make_shared<CubeShim>(hypervisor);
    auto cubelet    = std::make_shared<Cubelet>(shim);
    auto master     = std::make_shared<CubeMaster>(cubelet);
    auto api        = std::make_shared<CubeAPI>(master);

    std::cout << "VÒNG ĐỜI REQUEST Sandbox.create()\n" << std::string(72, '=') << "\n\n";

    // In trace từng bước: mỗi thành phần ghi tên + kết quả handle().
    std::cout << "[Client/SDK]   gửi POST /sandboxes\n";
    std::cout << "[" << api->name() << "]  " << api->handle() << "\n";

    std::cout << "\n" << std::string(72, '-') << "\n";
    std::cout << "Control plane: CubeAPI, CubeMaster, Redis (stateless).\n";
    std::cout << "Data plane   : Cubelet, CubeShim, CubeHypervisor, CubeCoW, CubeVS.\n";
    return 0;
}
