# Xây dựng trang GitHub.io cho khóa macOS Reverse Engineering

## Bối cảnh

Bạn có khóa học macOS Reverse Engineering 10 tuần với cấu trúc: lessons (week01-10.md), code (week01-10 folders với README.md và scripts), exercises, projects, references, và INDEX.md. Cần tạo trang GitHub.io tương tự index.html hiện tại, với cấu trúc thư mục và các file HTML để hiển thị nội dung Markdown dưới dạng web.

## File sẽ đụng tới

- `index.html`
- `macos-re-10weeks/INDEX.md`
- `macos-re-10weeks/lessons/week01.md`
- `macos-re-10weeks/code/week01/README.md`

## Ràng buộc không được phá

1. Sử dụng cấu trúc HTML/CSS hiện tại trong index.html (styles.css, course.css, script.js)
2. Tạo file HTML cho INDEX (trang tổng quan khóa học)
3. Tạo file HTML cho từng week (lessons, code, exercises)
4. Không thay đổi nội dung Markdown, chỉ chuyển đổi sang HTML
5. Giữ điều hướng nhất quán với trang chủ
6. Hỗ trợ breadcrumb navigation để quay lại trang chủ

## Các bước

1. Tạo thư mục macos-reverse-engineering-10weeks/ trong root
2. Tạo file INDEX.html từ macos-re-10weeks/INDEX.md
3. Tạo folder lessons/ với week01.html - week10.html
4. Tạo folder code/ với week01/README.html - week10/README.html
5. Tạo folder exercises/ và projects/ với cấu trúc tương tự
6. Tạo file references/safety.html
7. Cập nhật index.html để thêm link đến khóa macOS RE (course card #06)

## Cách kiểm chứng

1. Kiểm tra tất cả file HTML được tạo
2. Xác nhận breadcrumb navigation hoạt động
3. Kiểm tra link từ index.html đến course page
4. Kiểm tra link từ course page đến lessons/code/exercises
5. Xác nhận styling nhất quán với các khóa học khác

## Phương án đã cân nhắc và loại bỏ

- Dùng markdown-to-html converter thay vì tạo HTML thủ công
- Tạo template HTML chung rồi inject nội dung Markdown
- Sử dụng Jekyll/static site generator (nhưng khóa học hiện tại dùng HTML tĩnh)
