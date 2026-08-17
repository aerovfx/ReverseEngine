#!/usr/bin/env python3
# ============================================================
# dynamic_log_parser.py — Phân tích log dynamic giả
# Mục tiêu: đọc log (fs_usage/dynamic) dạng text và tóm tắt.
# Đầu vào: file log (mặc định dữ liệu giả).
# Đầu ra: tóm tắt hành vi (file, network, process).
# An toàn: dữ liệu giả, không chạy mẫu thật.
# ============================================================
import re
import sys

FAKE_LOG = """\
open /Users/lab/Library/LaunchAgents/com.example.app.plist
connect 93.184.216.34:443
read  /tmp/session.tmp
exec  /bin/sh -c 'echo hi'
close 5
"""


def summarize(lines):
    files = []
    nets = []
    procs = []
    for ln in lines:
        ln = ln.strip()
        if re.match(r"open|read|write|unlink", ln):
            files.append(ln)
        elif re.match(r"connect|sendto|recvfrom", ln):
            nets.append(ln)
        elif re.match(r"exec|fork|posix_spawn", ln):
            procs.append(ln)
    return files, nets, procs


def main():
    if len(sys.argv) > 1:
        content = open(sys.argv[1]).read()
    else:
        content = FAKE_LOG
        print("(dùng log giả)\n")
    files, nets, procs = summarize(content.splitlines())
    print("=== File ops ===")
    for f in files:
        print(" ", f)
    print("=== Network ops ===")
    for n in nets:
        print(" ", n)
    print("=== Process ops ===")
    for p in procs:
        print(" ", p)


if __name__ == "__main__":
    main()
