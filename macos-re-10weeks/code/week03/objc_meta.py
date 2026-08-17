#!/usr/bin/env python3
# ============================================================
# objc_meta.py — Mô phỏng metadata Objective-C
# Mục tiêu: minh hoạ cấu trúc class ObjC (isa, method list).
# Đầu vào: không.
# Đầu ra: in metadata của một class giả.
# An toàn: dữ liệu giả, không đụng app thật.
# ============================================================

class ObjCMethod:
    def __init__(self, name, imp):
        self.name = name
        self.imp = imp

    def __repr__(self):
        return f"<Method {self.name} -> {self.imp}>"


class ObjCClass:
    def __init__(self, name, superclass, methods):
        self.name = name
        self.superclass = superclass
        self.methods = methods  # method list

    def lookup(self, sel):
        """Mô phỏng objc_msgSend: tìm method theo selector."""
        for m in self.methods:
            if m.name == sel:
                return m
        if self.superclass:
            return self.superclass.lookup(sel)
        return None


def main():
    base = ObjCClass("NSObject", None, [])
    my = ObjCClass(
        "MyClass",
        base,
        [
            ObjCMethod("doThing:", "0x1000"),
            ObjCMethod("init", "0x1010"),
        ],
    )

    print("=== Objective-C class metadata (mô phỏng) ===")
    print(f"class      : {my.name}")
    print(f"superclass : {my.superclass.name}")
    print("method list:")
    for m in my.methods:
        print(f"  -[{my.name} {m.name}] imp={m.imp}")

    sel = "doThing:"
    found = my.lookup(sel)
    print(f"\nobjc_msgSend(my, {sel!r}) -> {found!r}")


if __name__ == "__main__":
    main()
