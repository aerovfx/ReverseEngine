// ============================================================
// license_check.c — App kiểm tra license (app của CHÍNH BẠN)
// Mục tiêu: minh hoạ một hàm kiểm tra serial đơn giản để học RE.
// Biên dịch: clang license_check.c -o /tmp/license
// Chạy:     /tmp/license SECRETKEY123
// An toàn: CHỈ dùng trên app do chính bạn viết, trong VM lab.
//          KHÔNG dùng kỹ thuật này để patch app của người khác.
// ============================================================
#include <stdio.h>
#include <string.h>

// Hàm kiểm tra license — đây là "điểm quyết định" để học RE.
int check_license(const char *key) {
    if (key == NULL) return 0;
    return strcmp(key, "SECRETKEY123") == 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Cách dùng: %s <serial>\n", argv[0]);
        return 1;
    }
    if (check_license(argv[1])) {
        printf("License HỢP LỆ\n");
        return 0;
    }
    printf("License KHÔNG hợp lệ\n");
    return 1;
}
