#!/usr/bin/env python3
"""Script tự động thêm bình luận tiếng Việt vào các file .py trong thư mục code."""
import os
import re
import ast

BASE_DIR = "1_System_App_Security/cybersec-ai-10weeks/code"

# Ánh xạ từ khóa/pattern sang comment tiếng Việt
COMMENT_MAP = [
    (r'^import (\w+)', r'# Import thư viện \1'),
    (r'^from (\w+) import', r'# Import hàm/class từ thư viện \1'),
    (r'^records\s*=\s*\[', '# Khởi tạo danh sách dữ liệu mẫu'),
    (r'^result\s*=\s*\[', '# Tạo danh sách kết quả (list comprehension)'),
    (r'^result\s*=\s*\{', '# Tạo từ điển kết quả'),
    (r'^result\s*=\s*\w+\(', '# Gọi hàm để tính toán kết quả'),
    (r'^assert', '# Kiểm tra điều kiện (assertion)'),
    (r'^print\(', '# In kết quả ra màn hình'),
    (r'^@dataclass', '# Khai báo dataclass'),
    (r'^class \w+', '# Định nghĩa class'),
    (r'^def \w+', '# Định nghĩa hàm'),
    (r'^if ', '# Kiểm tra điều kiện'),
    (r'^for ', '# Vòng lặp for'),
    (r'^while ', '# Vòng lặp while'),
    (r'^return ', '# Trả về giá trị'),
    (r'^with ', '# Mở context manager'),
    (r'^try:', '# Bắt đầu khối try-except'),
    (r'^except', '# Xử lý ngoại lệ'),
    (r'^finally:', '# Khối finally luôn chạy'),
    (r'^elif ', '# Kiểm tra điều kiện khác'),
    (r'^else:', '# Trường hợp còn lại'),
    (r'^data\s*=\s*\[', '# Dữ liệu đầu vào'),
    (r'^items\s*=\s*\[', '# Danh sách các phần tử'),
    (r'^payload\s*=\s*', '# Tạo payload dữ liệu'),
    (r'^response\s*=\s*', '# Lưu phản hồi'),
    (r'^request\s*=\s*', '# Tạo request'),
    (r'^config\s*=\s*', '# Cấu hình'),
    (r'^data\s*=\s*\{', '# Dữ liệu dạng từ điển'),
    (r'^\w+\s*=\s*\[', '# Khởi tạo danh sách'),
    (r'^\w+\s*=\s*\{', '# Khởi tạo từ điển'),
    (r'^\w+\s*=\s*\w+\(', '# Gán kết quả từ hàm'),
    (r'^\w+\s*=\s*\d+', '# Gán giá trị số'),
    (r'^\w+\s*=\s*["\']', '# Gán giá trị chuỗi'),
    (r'^\w+\s*=\s*True', '# Gán giá trị True'),
    (r'^\w+\s*=\s*False', '# Gán giá trị False'),
    (r'^\w+\s*=\s*None', '# Gán giá trị None'),
    (r'^\w+\s*=\s*\w+', '# Gán giá trị từ biến khác'),
]


def add_comments_to_file(filepath):
    """Thêm comment tiếng Việt vào file Python."""
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    new_lines = []
    in_docstring = False
    
    for i, line in enumerate(lines):
        stripped = line.strip()
        
        # Xử lý docstring (""" hoặc ''')
        if stripped.startswith('"""') or stripped.startswith("'''"):
            if in_docstring:
                in_docstring = False
            else:
                in_docstring = True
            new_lines.append(line)
            continue
        
        # Bỏ qua dòng trống, comment đã có, hoặc trong docstring
        if in_docstring or not stripped or stripped.startswith('#') or stripped.startswith('//'):
            new_lines.append(line)
            continue
        
        # Kiểm tra nếu dòng đã có comment ở cuối
        if '#' in line and not stripped.startswith('#'):
            # Đã có comment, thêm comment mới trước dòng này
            pass
        
        # Tìm comment phù hợp
        comment = None
        for pattern, comment_text in COMMENT_MAP:
            match = re.match(pattern, stripped)
            if match:
                # Thay thế \1 nếu có
                comment = comment_text
                for j in range(1, len(match.groups()) + 1):
                    comment = comment.replace(f'\\{j}', match.group(j))
                break
        
        if comment:
            # Kiểm tra xem dòng trước đã có comment tương tự chưa
            if new_lines and new_lines[-1].strip().startswith('#'):
                # Bỏ qua nếu comment trùng lặp
                prev_comment = new_lines[-1].strip()
                if prev_comment == comment:
                    new_lines.append(line)
                    continue
            # Thêm comment trước dòng code
            indent = re.match(r'^(\s*)', line).group(1)
            new_lines.append(f'{indent}{comment}\n')
        
        new_lines.append(line)
    
    # Ghi lại file
    with open(filepath, 'w', encoding='utf-8') as f:
        f.writelines(new_lines)
    
    print(f'Đã xử lý: {filepath}')


def main():
    """Duyệt tất cả file .py trong thư mục code."""
    count = 0
    for root, dirs, files in os.walk(BASE_DIR):
        for filename in files:
            if filename.endswith('.py'):
                filepath = os.path.join(root, filename)
                add_comments_to_file(filepath)
                count += 1
    print(f'\nĐã xử lý {count} file Python.')


if __name__ == '__main__':
    main()
