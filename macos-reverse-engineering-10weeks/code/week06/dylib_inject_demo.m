// ============================================================
// dylib_inject_demo.m — Demo dylib "injection" bằng DYLD_INSERT_LIBRARIES
// Mục tiêu: thấy constructor của dylib chạy khi app nạp dylib.
// Biên dịch:
//   clang -dynamiclib dylib_inject_demo.m -o /tmp/inject.dylib
//   clang -framework Foundation -o /tmp/hello -x objective-c - <<< 'int main(void){NSLog(@"app chạy");return 0;}'
//   DYLD_INSERT_LIBRARIES=/tmp/inject.dylib /tmp/hello
// An toàn: chỉ dùng trên app do chính bạn viết, trong VM lab.
// ============================================================
#import <Foundation/Foundation.h>

__attribute__((constructor))
static void injected(void) {
    NSLog(@"[inject] dylib đã được nạp vào tiến trình");
}
