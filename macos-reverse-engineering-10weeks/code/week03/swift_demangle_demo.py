#!/usr/bin/env python3
# ============================================================
# swift_demangle_demo.py — Minh hoạ giải mã symbol Swift mangled
# Mục tiêu: cho thấy symbol Swift bị "mangle" và cách đọc.
# Đầu vào: không.
# Đầu ra: vài symbol mangled + dạng đã giải mã (mô phỏng).
# An toàn: dữ liệu giả.
# ============================================================

# Cặp mangled -> demangled (mô phỏng, chỉ minh hoạ)
SAMPLES = [
    ("$s4demo5greetyySSF", "_$s4demo5greetyySSF".lstrip("_")),  # demo.greet() -> String
    ("$s4demo5MyClassC6doThingyys6Int32VF", "demo.MyClass.doThing(_: Swift.Int32)"),
]


def main():
    print("=== Swift symbol mangling (mô phỏng) ===")
    print("Symbol Swift chứa kiểu và module trong tên, cần demangle để đọc.\n")
    for mangled, demangled in SAMPLES:
        print(f"mangled  : {mangled}")
        print(f"demangled: {demangled}")
        print()
    print("Thực tế dùng: swift-demangle <symbol>")


if __name__ == "__main__":
    main()
