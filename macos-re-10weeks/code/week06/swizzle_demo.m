// ============================================================
// swizzle_demo.m — Demo method swizzling trong Objective-C
// Mục tiêu: đổi implementation của 2 method lúc runtime.
// Biên dịch: clang -framework Foundation swizzle_demo.m -o /tmp/swizzle
// An toàn: chạy trên app của chính bạn; không can thiệp app khác.
// ============================================================
#import <Foundation/Foundation.h>
#import <objc/runtime.h>

@interface MyClass : NSObject
- (NSString *)greet;
- (NSString *)secretGreet;
@end

@implementation MyClass
- (NSString *)greet {
    return @"Hello";
}
- (NSString *)secretGreet {
    return @"Swizzled!";
}
@end

int main(void) {
    @autoreleasepool {
        MyClass *obj = [[MyClass alloc] init];
        NSLog(@"Before: %@", [obj greet]);

        Method a = class_getInstanceMethod([MyClass class], @selector(greet));
        Method b = class_getInstanceMethod([MyClass class], @selector(secretGreet));
        method_exchangeImplementations(a, b);

        NSLog(@"After : %@", [obj greet]);
    }
    return 0;
}
