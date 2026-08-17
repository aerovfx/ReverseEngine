# -*- coding: utf-8 -*-
"""
06_network_policy.py — Mô phỏng network policy allowlist/denylist.

Mục tiêu: minh hoạ khớp policy theo CIDR (LPM trie) cho sandbox.
Đầu vào : không (demo); policy mặc định default-deny.
Đầu ra : kết quả cho phép/chặn cho từng địa chỉ đích.
An toàn : chỉ mô phỏng khớp địa chỉ bằng Python.
"""

# Policy: danh sách (prefix, action). Thứ tự ưu tiên theo prefix dài hơn.
POLICY = [
    ("10.0.0.0/8", "allow"),   # mạng nội bộ riêng
    ("0.0.0.0/0", "deny"),     # default-deny mọi thứ còn lại
]

# (đích, kỳ vọng)
TESTS = [
    ("10.1.2.3", "allow (nội bộ)"),
    ("192.168.1.1", "deny (default)"),
    ("8.8.8.8", "deny (internet)"),
]


def ip_to_int(ip):
    parts = [int(x) for x in ip.split(".")]
    return (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3]


def match(ip):
    """Khớp policy: chọn prefix dài nhất (longest-prefix-match)."""
    ip_int = ip_to_int(ip)
    best = None
    for prefix, action in POLICY:
        net, bits = prefix.split("/")
        mask = (0xFFFFFFFF << (32 - int(bits))) & 0xFFFFFFFF
        if (ip_int & mask) == (ip_to_int(net) & mask):
            if best is None or int(bits) > int(best[0]):
                best = (bits, action)
    return best[1] if best else "deny"


def main():
    print("Network policy (default-deny):\n")
    for prefix, action in POLICY:
        print(f"  {prefix:<14} -> {action}")
    print()
    for dst, expect in TESTS:
        print(f"  đích {dst:<14} -> {match(dst):<6} (kỳ vọng: {expect})")
    print("\nLPM trie giúp khớp prefix dài nhất ở tốc độ cao, không duyệt từng luật.")


if __name__ == "__main__":
    main()
