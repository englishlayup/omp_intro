#include "lib.c"
#include <assert.h>
#include <stdio.h>

// Very small test helpers
int failed = 0;
#define TEST(name) void name()
#define RUN_TEST(name)                                                         \
  printf("\n\033[1m%s\n\033[0m", #name);                                       \
  name()
#define ASSERT(expr)                                                           \
  if (!(expr)) {                                                               \
    failed = 1;                                                                \
    printf("\033[0;31mFAIL: %s\n\033[0m", #expr);                              \
  } else {                                                                     \
    printf("\033[0;32mPASS: %s\n\033[0m", #expr);                              \
  }
#define ASSERT_STR_EQ(str1, str2)                                              \
  if (!(strcmp(str1, str2) == 0)) {                                            \
    failed = 1;                                                                \
    printf("\033[0;31mFAIL: %s != %s\n\033[0m", str1, str2);                   \
  } else {                                                                     \
    printf("\033[0;32mPASS: %s == %s\n\033[0m", str1, str2);                   \
  }
// End of test helpers

#define PI 3.141593

TEST(test_round_to_decimal) {
  ASSERT(round_to_decimal(5.43432, 4) == 5.4343);
  ASSERT(round_to_decimal(5.43435, 4) == 5.4344);
  ASSERT(round_to_decimal(5.43435, 0) == 5);
}

TEST(test_parallel) { ASSERT(round_to_decimal(parallel_pi(), 6) == PI); }

TEST(test_sequential) { ASSERT(round_to_decimal(sequential_pi(), 6) == PI); }

int main() {
  // Add a `RUN_TEST` line for each test function
  RUN_TEST(test_round_to_decimal);
  RUN_TEST(test_parallel);
  RUN_TEST(test_sequential);
  return failed;
}
