// ============================================================================
// 09_lifecycle.cpp — Vòng đời sandbox + auto-pause/resume (state machine)
//
// Mục tiêu: minh hoạ vòng đời microVM: created → running → paused → resumed →
//           destroyed, và cơ chế AUTO-PAUSE (sandbox rảnh tự tạm dừng để giải
//           phóng CPU, có request thì resume).
// Đầu vào : không.
// Đầu ra : vết chuyển trạng thái theo chuỗi sự kiện.
// An toàn : THUẦN TÍNH TOÁN — state machine bằng enum + bảng chuyển trạng thái.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 lifecycle.cpp -o lifecycle
// ============================================================================

#include <iostream>
#include <string>
#include <map>

// Các trạng thái của một sandbox.
enum class State { Created, Running, Paused, Destroyed };

// Sự kiện kích hoạt chuyển trạng thái.
enum class Event { Start, Idle, Request, Kill };

// Tên trạng thái để in ra trace.
const char* state_name(State s) {
    switch (s) {
        case State::Created:   return "created";
        case State::Running:   return "running";
        case State::Paused:    return "paused";
        case State::Destroyed: return "destroyed";
    }
    return "?";
}

// State machine: chuyển trạng thái theo (state, event).
class Lifecycle {
public:
    Lifecycle() : state_(State::Created) {}

    State state() const { return state_; }

    // Áp dụng một sự kiện, trả về trạng thái mới (giữ nguyên nếu chuyển không hợp lệ).
    State apply(Event e) {
        static const std::map<std::pair<State, Event>, State> table = {
            {{State::Created,   Event::Start},   State::Running},
            {{State::Running,   Event::Idle},    State::Paused},     // auto-pause
            {{State::Paused,    Event::Request}, State::Running},    // resume
            {{State::Running,   Event::Kill},    State::Destroyed},
            {{State::Paused,    Event::Kill},    State::Destroyed},
        };
        auto it = table.find({state_, e});
        if (it != table.end()) state_ = it->second;
        return state_;
    }

private:
    State state_;
};

int main() {
    Lifecycle lc;
    std::cout << "VÒNG ĐỜI SANDBOX + AUTO-PAUSE\n" << std::string(70, '=') << "\n\n";

    std::cout << "  trạng thái ban đầu: " << state_name(lc.state()) << "\n";

    // Chuỗi sự kiện điển hình: start -> idle (auto-pause) -> request (resume) -> kill.
    const std::pair<Event, const char*> sequence[] = {
        {Event::Start,   "start"},
        {Event::Idle,    "idle (auto-pause)"},
        {Event::Request, "request (resume)"},
        {Event::Idle,    "idle (auto-pause)"},
        {Event::Kill,    "kill"},
    };

    for (const auto& [ev, label] : sequence) {
        State s = lc.apply(ev);
        std::cout << "  --" << label << "--> " << state_name(s) << "\n";
    }

    std::cout << "\n" << std::string(70, '-') << "\n";
    std::cout << "KẾT LUẬN: auto-pause giải phóng CPU khi sandbox rảnh; resume nạp\n";
    std::cout << "snapshot khi có request — tiết kiệm tài nguyên cho hàng nghìn sandbox.\n";
    return 0;
}
