<div align="center">

# 🔍 ReverseEngine

### Học reverse engineering thực chiến — từ nền tảng binary đến thành thạo dịch ngược

**Nền tảng học tập mở · Tiếng Việt · Chạy trên GitHub Pages**

<p>
  <a href="https://aerovfx.github.io/ReverseEngine/"><img src="https://img.shields.io/badge/🚀_Truy_cập_trang_học-aerovfx.github.io%2FReverseEngine-b455ff?style=for-the-badge&logo=github" alt="Truy cập trang học"></a>
  <a href="#-lộ-trình"><img src="https://img.shields.io/badge/Lộ_trình-4_khóa·40_tuần-ffb000?style=for-the-badge" alt="Lộ trình"></a>
  <a href="#-nguyên-tắc-an-toàn"><img src="https://img.shields.io/badge/Lab-An_toàn·Cô_lập-b455ff?style=for-the-badge" alt="An toàn"></a>
</p>

<p>
  <img src="https://img.shields.io/badge/Binary-PE·MachO·ARM64-0a0616" alt="binary">
  <img src="https://img.shields.io/badge/Công_cụ-x64dbg·LLDB·Frida·Ghidra-0a0616" alt="tools">
  <img src="https://img.shields.io/badge/Ngôn_ngữ-Tiếng_Việt-0a0616" alt="lang">
  <img src="https://img.shields.io/badge/License-Giáo_dục_·_Hợp_pháp-0a0616" alt="license">
</p>

---

<h3>🎯 <a href="https://aerovfx.github.io/ReverseEngine/">Mở ReverseEngine trên GitHub Pages →</a></h3>

</div>

---

## ✨ ReverseEngine là gì?

ReverseEngine là bộ giáo trình **reverse engineering 10 tuần** bằng tiếng Việt, xây dựng theo triết lý *"đọc mã máy, làm chủ dịch ngược"*. Mỗi lộ trình kết hợp lý thuyết cô đọng, lab có hướng dẫn, code minh hoạ chạy được và dự án cuối khóa — tất cả trong phạm vi hợp pháp và môi trường lab cô lập.

- 🧭 **4 lộ trình · 40 tuần** — từ nền tảng đến chuyên sâu
- 💻 **Code minh hoạ chạy được** — Python/Bash/C với chú thích tiếng Việt
- 🧪 **Bài tập 3 mức** — cơ bản, nâng cao, thử thách
- 📋 **Rubric 100 điểm** — đánh giá rõ ràng, có dự án cuối khóa
- 🛡️ **An toàn & hợp pháp** — chỉ lab trên hệ thống bạn sở hữu/được phép

---

## 🗺️ Lộ trình

| # | Khóa học | Cấp độ | Nội dung chính |
|---|----------|--------|----------------|
| 01 | [Reverse Engineering & Malware Analysis](https://aerovfx.github.io/ReverseEngine/reverse-engineering-malware-10weeks/INDEX.html) | 🟢 Nền tảng | PE, phân tích tĩnh/động, unpacking, IOC |
| 02 | [Software Reverse Engineering](https://aerovfx.github.io/ReverseEngine/software-reverse-engineering-10weeks/INDEX.html) | 🔴 Chuyên sâu | x64dbg, DIE, secure patching, license |
| 03 | [Software Cracking & Ethical Hacking](https://aerovfx.github.io/ReverseEngine/software-cracking-10weeks/INDEX.html) | 🔴 Chuyên sâu | DIE → EP → breakpoint → reversing jump → patch |
| 04 | [macOS Reverse Engineering](https://aerovfx.github.io/ReverseEngine/macos-reverse-engineering-10weeks/INDEX.html) | 🔴 Chuyên sâu | Mach-O, ARM64, Objective-C/Swift, Frida |

> Mỗi khóa gồm **10 tuần**, có `INDEX.html` (tổng quan), `schedule.md` (lịch học), `lessons/` (bài học), `code/` (code mẫu), `exercises/` (bài tập) và `projects/` (dự án cuối khóa).

---

## 🚀 Bắt đầu học

**1. Truy cập trang học (không cần cài đặt):**

👉 **[https://aerovfx.github.io/ReverseEngine/](https://aerovfx.github.io/ReverseEngine/)**

**2. Chạy local:**

```bash
git clone https://github.com/aerovfx/ReverseEngine.git
cd ReverseEngine
bundle install
bundle exec jekyll build
# hoặc chỉ xem trang chủ tĩnh:
open index.html
```

**3. Triển khai GitHub Pages:**

Repository → **Settings → Pages → Deploy from a branch** → chọn nhánh `main` và thư mục `/ (root)`.

---

## 🎓 Công cụ lớp học (Classroom)

Bộ công cụ chạy trực tiếp trên trang, lưu dữ liệu local-first (không rời trình duyệt):

| Công cụ | Link |
|---------|------|
| ◎ Cổng lớp học | [`/tools/khao-sat/portal.html`](https://aerovfx.github.io/ReverseEngine/tools/khao-sat/portal.html) |
| ▤ Khảo sát học viên | [`/tools/khao-sat/index.html`](https://aerovfx.github.io/ReverseEngine/tools/khao-sat/index.html) |
| ◇ Đánh giá đồng đẳng | [`/tools/khao-sat/danh-gia.html`](https://aerovfx.github.io/ReverseEngine/tools/khao-sat/danh-gia.html) |
| ⚙ Chấm điểm giáo viên | [`/tools/khao-sat/admin.html`](https://aerovfx.github.io/ReverseEngine/tools/khao-sat/admin.html) |
| ▥ Dashboard kết quả | [`/tools/khao-sat/ket-qua.html`](https://aerovfx.github.io/ReverseEngine/tools/khao-sat/ket-qua.html) |

---

## 🛡️ Nguyên tắc an toàn

> ⚠️ **Reverse engineering chỉ hợp pháp khi** bạn sở hữu phần mềm, được ủy quyền bằng văn bản, hoặc phân tích crackme công khai.

- ✅ Chỉ phân tích **binary của bạn / crackme / được phép**
- 🚫 **Không** phát tán crack, patch, keygen cho phần mềm thương mại
- 🚫 **Không** né DRM/license để dùng lậu
- 🧪 Malware chỉ chạy trong **VM cô lập**, có snapshot & hash SHA-256

Đọc chi tiết: [`references/safety.md`](https://aerovfx.github.io/ReverseEngine/macos-reverse-engineering-10weeks/references/safety.html)

---

## 🤝 Đóng góp

PR và góp ý đều được hoan nghênh. Vui lòng giữ nội dung trong **phạm vi hợp pháp** và tuân thủ nguyên tắc an toàn ở trên.

---

<div align="center">

**Built with ❤️ để học · Chia sẻ để cùng tiến bộ**

© ReverseEngine — [aerovfx.github.io/ReverseEngine](https://aerovfx.github.io/ReverseEngine/)

</div>
