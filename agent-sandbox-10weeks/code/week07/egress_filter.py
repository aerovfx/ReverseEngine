# -*- coding: utf-8 -*-
"""
07_egress_filter.py — Mô phỏng egress zero-trust (L7 MITM proxy).

Mục tiêu: minh hoạ domain allowlist, quyết định TLS inspection,
          và credential injection của CubeEgress.
Đầu vào : không (demo).
Đầu ra : quyết định cho phép/chặn + header được tiêm.
An toàn : chỉ mô phỏng bằng dữ liệu, không MITM lưu lượng thật.
"""

# Chỉ cho phép các domain này (default-deny).
ALLOWLIST = {"api.openai.com", "pypi.org"}

# Secret không bao giờ đưa vào sandbox; proxy sẽ tiêm khi cần.
SECRETS = {"api.openai.com": "Bearer sk-***proxy-injected***"}


def egress_decision(host):
    """Quyết định cho phép/chặn dựa trên allowlist."""
    if host in ALLOWLIST:
        return "allow"
    return "deny"


def inject(host):
    """Credential injection: proxy thêm Authorization, sandbox không thấy secret."""
    return SECRETS.get(host, None)


def main():
    print("CubeEgress — zero-trust egress (default-deny)\n")
    for host in ["api.openai.com", "pypi.org", "evil-c2.example", "raw.githubusercontent.com"]:
        decision = egress_decision(host)
        status = "✅ CHO PHÉP" if decision == "allow" else "⛔ CHẶN"
        header = inject(host)
        extra = f" | inject Authorization: {header}" if header else ""
        print(f"  {host:<28} -> {status}{extra}")
    print("\nLưu ý: TLS inspection nhờ root CA do proxy cấp; secret không nằm trong sandbox.")


if __name__ == "__main__":
    main()
