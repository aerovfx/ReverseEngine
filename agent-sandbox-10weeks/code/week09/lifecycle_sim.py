# -*- coding: utf-8 -*-
"""
09_lifecycle_sim.py — Mô phỏng vòng đời sandbox + auto-pause.

Mục tiêu: minh hoạ create/run/pause/resume/snapshot/destroy.
Đầu vào : không (demo).
Đầu ra : vết chuyển trạng thái của một sandbox.
An toàn : chỉ mô phỏng state machine bằng Python.
"""

TRANSITIONS = {
    "created": "running",
    "running": "paused",      # auto-pause khi rảnh
    "paused": "running",      # resume khi có request
    "running": "destroyed",
}


def next_state(current, event):
    return {
        ("created", "start"): "running",
        ("running", "idle"): "paused",
        ("paused", "request"): "running",
        ("running", "kill"): "destroyed",
        ("paused", "kill"): "destroyed",
    }.get((current, event), current)


def main():
    state = "created"
    print(f"  trạng thái ban đầu: {state}")
    for event in ["start", "idle", "request", "idle", "kill"]:
        state = next_state(state, event)
        print(f"  --{event}--> {state}")
    print("\nAuto-pause: sandbox rảnh tự tạm dừng (giải phóng CPU), có request thì resume.")


if __name__ == "__main__":
    main()
