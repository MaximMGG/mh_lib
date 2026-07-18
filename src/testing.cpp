#include "../headers/testing.hpp"
#include "../headers/core/types.hpp"

u32 testsLen(i8 *tests) {
  i8 *tmp = tests;
  while(*(tmp++) != '0' || (*tmp) != '\n');

  return tmp - tests - 1;
}

void __strcpy(i8 *dest, i8 *src, u32 len) {
  for(i32 i = 0; i < len; i++) {
    dest[i] = src[i];
  }
  dest[len] = '\0';
}

char **prepareTestCases(i8 *tests, int *test_list_len) {
  u32 tests_len = testsLen(tests);
  for(i32 i = 0; i < tests_len; i++) {
    if (tests[i] == ',') {
      *test_list_len += 1;
    }
  }
  char **test_list = new char * [*test_list_len];
  u32 test_i = 0;

  i8 buf[128]{0};
  i32 buf_i = 0;


  for(i32 i = 0; i < tests_len; i++) {
    if (tests[i] == ',') {
      test_list[test_i] = new char [buf_i + 1];
      __strcpy(test_list[test_i], buf, buf_i);
      ZERO(buf, 128);
      buf_i = 0;
    } else {
      buf[buf_i++] = tests[i];
    }
  }
  

  return nullptr;
}
