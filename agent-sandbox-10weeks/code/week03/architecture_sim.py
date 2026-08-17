# -*- coding: utf-8 -*-
"""
03_architecture_sim.py — Mô phỏng vòng đời request Sandbox.create().

Mục tiêu: minh hoạ luồng control plane -> data plane của CubeSandbox.
Đầu vào : không (demo).
Đầu ra : vết từng bước của một request tạo sandbox.
An toàn : chỉ mô phỏng chuỗi bước bằng dữ liệu.
"""

# Mỗi bước: (thành phần, hành động)
PIPELINE = [
    ("Client/SDK", "POST /sandboxes (E2B-compatible)"),
    ("CubeAPI", "nhận REST -> gửi gRPC CreateSandbox"),
    ("CubeMaster", "chọn node theo tài nguyên"),
    ("CubeMaster", "gửi gRPC RunCubeSandbox tới Cubelet"),
    ("Cubelet", "chuẩn bị rootfs (CubeCoW clone từ template)"),
    ("Cubelet", "gọi containerd Shim v2 -> CubeShim"),
    ("CubeShim", "khởi động CubeHypervisor (RustVMM)"),
    ("CubeHypervisor", "tạo microVM: vCPU, memory, virtio devices"),
    ("CubeHypervisor", "restore từ snapshot -> VM ready (vsock)"),
    ("Cubelet", "gắn TAP + network policy (CubeVS)"),
    ("CubeMaster", "publish lifecycle event tới Redis"),
    ("Client/SDK", "nhận 201 { sandbox_id, ... }"),
]


def main():
    print("Vòng đời request Sandbox.create()\n")
    for i, (comp, action) in enumerate(PIPELINE, 1):
        print(f"{i:>2}. [{comp:<14}] {action}")
    print("\nControl plane: CubeAPI, CubeMaster, Redis (stateless).")
    print("Data plane: Cubelet, CubeShim, CubeHypervisor, CubeCoW, CubeVS.")


if __name__ == "__main__":
    main()
