#include "../headers/testing.hpp"
#include "../headers/core/types.hpp"
#include <stdarg.h>
#include <time.h>
#include <stdio.h>

#define 

u32 passed_test = 0;
u32 succesed_test = 0;
u32 failed_test = 0;
TestCase *cur_test_case;

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

void runTest(TestCase *test_cases, i32 test_list_len, i8 *test_file_name) {
  printf("Run %s tests", test_file_name);
  u64 start_time = clock();
  for (i32 i = 0; i < test_list_len; i++) {
    cur_test_case = &test_cases[i];
    cur_test_case->start_time = clock();
    cur_test_case->test_func();
    cur_test_case->end_time = clock();
    passed_test++;
    if (cur_test_case->success) {
      succesed_test++;
    }
  }
  u64 end_time = clock();
}

TestCase *prepareTestCases(i8 *tests, int *test_list_len, ...) {
  u32 tests_len = testsLen(tests);
  for(i32 i = 0; i < tests_len; i++) {
    if (tests[i] == ',') {
      *test_list_len += 1;
    }
  }
  TestCase *test_list = new TestCase [*test_list_len];
  u32 test_i = 0;

  i8 buf[128]{0};
  i32 buf_i = 0;

  for(i32 i = 0; i < tests_len; i++) {
    if (tests[i] == ',') {
      test_list[test_i].test_name = new char [buf_i + 1];
      __strcpy(test_list[test_i].test_name, buf, buf_i);
      ZERO(buf, 128);
      buf_i = 0;
      test_i++;
    } else {
      buf[buf_i++] = tests[i];
    }
  }
  test_list[test_i].test_name = new char[buf_i + 1];
  __strcpy(test_list[test_i].test_name, buf, buf_i);

  test_i = 0;
  va_list li;
  va_start(li, test_list_len);

  ptr p;

  while (true) {
    p = va_arg(li, ptr);
    if (p == nullptr) {
      break;
    }
    test_list[test_i++].test_func = (void (*)())p;
  }
  test_list[0].success = true;
  va_end(li);
  return test_list;
}
