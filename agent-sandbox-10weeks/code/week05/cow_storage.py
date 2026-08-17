# -*- coding: utf-8 -*-
"""
05_cow_storage.py — Mô phỏng Copy-on-Write (shared extent + reflink).

Mục tiêu: minh hoạ vì sao clone/snapshot O(1) và tiết kiệm dung lượng.
Đầu vào : không (demo).
Đầu ra : bảng extent của template và 2 sandbox trước/sau khi ghi.
An toàn : chỉ mô phỏng bằng dict, không thao tác filesystem.
"""

# Template có 4 extent (khối dữ liệu). Sandbox clone tham chiếu (không copy).
TEMPLATE = ["extent1", "extent2", "extent3", "extent4"]


def clone():
    """Clone O(1): mỗi sandbox tham chiếu tới cùng các extent."""
    return {e: "shared" for e in TEMPLATE}


def write(sb, extent):
    """Copy-on-Write: khi ghi, extent trở thành bản riêng của sandbox đó."""
    sb[extent] = "dirty (riêng)"


def main():
    a, b = clone(), clone()
    print("Ban đầu (clone O(1), không copy byte):")
    print(f"  Template: {TEMPLATE}")
    print(f"  Sandbox A: {a}")
    print(f"  Sandbox B: {b}")

    print("\nSandbox B ghi vào extent2 (copy-on-write):")
    write(b, "extent2")
    print(f"  Sandbox A: {a}")
    print(f"  Sandbox B: {b}")

    shared_a = sum(1 for v in a.values() if v == "shared")
    print(f"\nSố extent Sandbox A còn chia sẻ với template: {shared_a}/{len(TEMPLATE)}")
    print("Kết luận: chỉ extent bị ghi mới được copy riêng (CoW), còn lại chia sẻ.")


if __name__ == "__main__":
    main()
