# -*- coding: utf-8 -*-
"""
04_jump_sim.py — Mô phỏng CMP + cờ ZF + lệnh nhảy có/không điều kiện.

Mục tiêu: giúp hiểu vì sao JE/JNE nhảy hay không nhảy dựa vào cờ ZF.
Đầu vào : không (chạy demo có sẵn).
Đầu ra : bảng trạng thái thanh ghi/cờ và quyết định nhảy.
An toàn : chỉ mô phỏng bằng dữ liệu, không đụng CPU thật.
"""


def cpu_step(reg_a: int, reg_b: int) -> dict:
    """Mô phỏng lệnh CMP a, b: đặt ZF=1 nếu a == b."""
    zf = 1 if reg_a == reg_b else 0
    return {"ZF": zf}


def je_taken(zf: int) -> bool:
    """JE (jump if equal) nhảy khi ZF == 1."""
    return zf == 1


def jne_taken(zf: int) -> bool:
    """JNE (jump if not equal) nhảy khi ZF == 0."""
    return zf == 0


def main() -> None:
    print("Mô phỏng kiểm tra serial:  CMP input, correct_serial")
    print("=" * 55)

    correct_serial = 0x1337
    for user_input in (0x1337, 0x1234):
        # CMP user_input, correct_serial
        flags = cpu_step(user_input, correct_serial)
        print(f"input=0x{user_input:04X}  correct=0x{correct_serial:04X}  -> ZF={flags['ZF']}")
        print(f"   JE  (jump if equal)      : {'NHẢY (nhánh đúng)' if je_taken(flags['ZF']) else 'không nhảy'}")
        print(f"   JNE (jump if not equal)  : {'NHẢY (nhánh sai)' if jne_taken(flags['ZF']) else 'không nhảy'}")
        print("-" * 55)

    print("Kết luận: để 'đảo nhảy', ta đổi JE<->JNE hoặc lật cờ ZF (1<->0).")


if __name__ == "__main__":
    main()
