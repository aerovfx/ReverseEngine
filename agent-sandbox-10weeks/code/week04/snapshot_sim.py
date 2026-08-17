# -*- coding: utf-8 -*-
"""
04_snapshot_sim.py — Mô phỏng snapshot/restore bộ nhớ để boot nhanh.

Mục tiêu: minh hoạ vì sao restore nhanh hơn boot lạnh.
Đầu vào : không (demo).
Đầu ra : so sánh cold boot vs restore, và dirty-page tracking.
An toàn : chỉ mô phỏng trạng thái bằng dict.
"""

import time


def cold_boot():
    """Mô phỏng boot lạnh: kernel boot + init (tốn thời gian)."""
    time.sleep(0.6)  # giả lập vài giây
    return {"vCPU": "ready", "ram": "initialized", "init": "done"}


def snapshot(state):
    """Snapshot chỉ lưu trang bộ nhớ 'dirty' (đã thay đổi)."""
    return dict(state)


def restore(snap):
    """Restore nạp lại trạng thái đã snapshot (không boot lại)."""
    time.sleep(0.02)  # giả lập ms
    return dict(snap)


def main():
    print("1) Cold boot (lần đầu):")
    t0 = time.time()
    state = cold_boot()
    print(f"   trạng thái: {state}  (mất {time.time()-t0:.2f}s mô phỏng)")

    print("\n2) Snapshot trạng thái 'nóng' (kernel đã boot):")
    snap = snapshot(state)
    print(f"   snapshot lưu {len(snap)} mục trạng thái (chỉ dirty pages trong thực tế)")

    print("\n3) Restore (lần sau):")
    t0 = time.time()
    state2 = restore(snap)
    print(f"   trạng thái: {state2}  (mất {time.time()-t0:.2f}s mô phỏng)")

    print("\nKết luận: restore = nạp snapshot, nhanh hơn nhiều so với boot lại từ đầu.")


if __name__ == "__main__":
    main()
