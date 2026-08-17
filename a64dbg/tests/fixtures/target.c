// Fixture: tiến trình đích ARM64 để test MachBackend.
// In địa chỉ của bp_here (READY <addr>) rồi gọi bp_here liên tục,
// in "tick" định kỳ để test xác minh luồng vẫn chạy sau khi remove breakpoint.
#include <stdio.h>
#include <unistd.h>

volatile int g_counter = 0;

__attribute__((noinline)) void bp_here(void) {
  g_counter++;
}

int main(void) {
  printf("READY %p\n", (void*)&bp_here);
  fflush(stdout);

  for (int i = 0; i < 200000; ++i) {
    bp_here();
    if (i % 2000 == 0) {
      printf("tick %d\n", i);
      fflush(stdout);
    }
    usleep(500);  // 500us * 200000 = tối đa ~100s; test sẽ kill sớm.
  }

  printf("done %d\n", g_counter);
  return 0;
}
