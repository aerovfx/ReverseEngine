# -*- coding: utf-8 -*-
"""
02_isolation_levels.py — So sánh container vs VM vs MicroVM.

Mục tiêu: minh hoạ trade-off tốc độ / cô lập giữa các mức ảo hoá.
Đầu vào : không (demo).
Đầu ra : bảng so sánh + danh sách syscall nguy hiểm nên chặn.
An toàn : chỉ in thông tin giáo dục, không đụng hệ thống.
"""

LEVELS = [
    ("Container", "Có (cùng kernel)", "Thấp (~ms)", "Lớn — syscall, kernel vuln"),
    ("VM (QEMU/KVM)", "Không (kernel riêng)", "Cao (giây)", "Nhỏ — hypervisor"),
    ("MicroVM (RustVMM)", "Không (kernel riêng)", "Rất thấp (<100ms)", "Nhỏ — VMM tối giản"),
]

# syscall nguy hiểm mà sandbox thường chặn bằng seccomp.
DANGEROUS_SYSCALLS = [
    ("ptrace", "gắn vào tiến trình khác (đọc/ghi bộ nhớ)"),
    ("mount", "gắn filesystem, leo thang đặc quyền"),
    ("kexec_load", "nạp kernel mới"),
    ("bpf", "nạp chương trình BPF tuỳ ý"),
    ("perf_event_open", "rò rỉ thông tin kernel"),
]


def main():
    print("So sánh các mức cô lập\n")
    print(f"{'Mức':<18} {'Kernel':<20} {'Overhead':<16} {'Escape surface'}")
    print("-" * 78)
    for name, kernel, ovh, esc in LEVELS:
        print(f"{name:<18} {kernel:<20} {ovh:<16} {esc}")

    print("\nSyscall nguy hiểm (nên chặn bằng seccomp):")
    for name, why in DANGEROUS_SYSCALLS:
        print(f"  - {name:<16} {why}")

    print("\nKết luận: MicroVM = cô lập phần cứng + overhead thấp (lựa chọn của CubeSandbox).")


if __name__ == "__main__":
    main()
