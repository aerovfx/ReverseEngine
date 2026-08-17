# Tích hợp khóa macOS RE vào CyberLearn (giao diện Jekyll)

## Bối cảnh

Khóa macOS RE (10 tuần) đã xây xong trong workspace tại macos-re-10weeks/. Người dùng muốn nó hiển thị với giao diện CyberLearn (trang aerovfx.github.io/cybersercurity), có mã nguồn tại /Users/dangvietchung/cybersercurity — đây là repo Jekyll/GitHub Pages. Người dùng chốt: đưa TOÀN BỘ khóa vào repo CyberLearn, theo cấu trúc chuẩn để render đúng giao diện và build được bằng Jekyll. course.js yêu cầu cấu trúc cố định: lessons/weekNN.html, code/weekNN/README.html, exercises/weekNN/README.html, projects/final_project.html, và mọi trang Markdown phải có front matter (layout: course + permalink). Khóa hiện tại có exercises/weekNN.md (sai cấu trúc), chưa có front matter, và các lesson/code chưa nhúng code bằng include_relative.

## File sẽ đụng tới

- `/Users/dangvietchung/cybersercurity/1_System_App_Security/macos-reverse-engineering-10weeks/INDEX.md`
- `.../schedule.md`
- `.../lessons/week01.md ... week10.md`
- `.../code/README.md`
- `.../code/WEEKLY_EXAMPLES.md`
- `.../code/week01/...week10/README.md + scripts`
- `.../exercises/week01/README.md ... week10/README.md`
- `.../projects/final_project.md`
- `.../references/components.md`
- `.../references/safety.md`
- `.../references/software.md`

## Ràng buộc không được phá

Theo skill cyberlearn-course-creator: tên thư mục kebab-case kết thúc -10weeks; mọi trang Markdown có front matter layout: course + permalink; cấu trúc lessons/code/exercises/projects/references; code nhúng bằng {% include_relative %}; giữ nguyên giao diện dùng chung (không sửa _layouts/course.html, course.js, course.css); không tự commit/push/deploy; nội dung tiếng Việt, thuật ngữ Anh giữ nguyên; ràng buộc pháp lý RE app thương mại (chỉ app của mình/được phép/mã nguồn mở, cấm phát tán crack). Không sửa các khóa khác trong repo.

## Các bước

1. Tạo thư mục 1_System_App_Security/macos-reverse-engineering-10weeks/ trong repo và copy toàn bộ nội dung từ workspace macos-re-10weeks/ (lessons, code, exercises, projects, references). 2. Chuyển cấu trúc exercises/weekNN.md -> exercises/weekNN/README.md (nội dung giữ, đổi đường dẫn). 3. Thêm front matter (layout: course + title + permalink) vào MỌI trang: INDEX, schedule, lessons/week01..10, code/week01..10/README, exercises/week01..10/README, projects/final_project, references/*. Permalink đúng đường dẫn tương ứng với .html. 4. Trong lessons/weekNN.md và code/weekNN/README.md, nhúng code bằng {% include_relative <file> %} thay vì hiển thị thô, để course.js render code viewer. 5. Cập nhật INDEX.md để trỏ tới các trang bằng link tương đối (schedule.md, lessons, code, exercises, projects, references). 6. Build Jekyll (JEKYLL_NO_BUNDLER_REQUIRE=true bundle exec jekyll build --destination /tmp/...) và kiểm tra không lỗi, đủ 10 trang lesson + 10 code README + 10 exercise README + project + refs. 7. Chạy quality gates: git diff --check, py_compile code Python, bash -n script Bash, rà secret/pháp lý.

## Cách kiểm chứng

- Jekyll build thành công không lỗi, đầu ra /tmp có đủ các trang .html (10 lessons, 10 code README, 10 exercises README, project, refs, INDEX, schedule). - grep front matter: mọi .md trong khóa có 'layout: course' và 'permalink:'. - find exercises -name 'weekNN/README.md' = 10. - python3 -m py_compile mọi .py; bash -n mọi .sh. - git status chỉ thấy file mới trong macos-reverse-engineering-10weeks/, không sửa file khác. - Rà pháp lý: mọi lesson có cảnh báo pháp lý.

## Phương án đã cân nhắc và loại bỏ

- Tạo giao diện HTML tĩnh riêng trong workspace: bị loại vì người dùng chọn đưa vào repo CyberLearn để dùng chung giao diện + build Jekyll. - Giữ nguyên exercises/weekNN.md: bị loại vì course.js chỉ render exercises/weekNN/README.html. - Đặt khóa ở category khác: chọn 1_System_App_Security vì khóa RE Windows cùng category và cùng chủ đề reverse engineering.
