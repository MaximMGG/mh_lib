#include "../headers/testing.hpp"
#include "../headers/core/types.hpp"
#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define COLOR_RESET "\x1b[0m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED   "\x1b[31m"


#define TEST_TOTAL_PASSED_FMT "TOTAL " COLOR_BLUE "PASSED" COLOR_RESET " TESTS: %d\n"
#define TEST_TOTAL_SUCCESS_FMT "TOTAL " COLOR_GREEN "SUCCESS" COLOR_RESET " TESTS: %d\n"
#define TEST_TOTAL_FAILED_FMT "TOTAL " COLOR_BLUE "FAILED" COLOR_RESET " TESTS: %d\n"
#define TEST_TOTAL_TIME_FMT   "TOTAL TIME : %ds %dms\n"

#define TEST_SUCCES_FMT     "Test Case %s " COLOR_GREEN "SUCCESS" COLOR_RESET " : %ds %dms\n"
#define TEST_FAILED_FMT     "Test Case %s " COLOR_RED "FAILED" COLOR_RESET " %s : %ds %dms\n"


i8 failure_msg[256]{0};

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

void setFailureMsg(const i8 *msg, u32 line) {
  memset(failure_msg, 0, 256);
  sprintf(failure_msg, "%s line: %d", msg, line);
}

void printTestCaseSuccessMsg() {
  i8 buf[512]{0};
  u32 seconds = (cur_test_case->end_time - cur_test_case->start_time) / 1000;
  u32 ms = (cur_test_case->end_time - cur_test_case->start_time) % 1000;
  sprintf(buf, TEST_SUCCES_FMT, cur_test_case->test_name, seconds, ms);
  printf("%s", buf);
}

void printTestCaseFailMsg() {
  i8 buf[512]{0};
  u32 seconds = (cur_test_case->end_time - cur_test_case->start_time) / 1000;
  u32 ms = (cur_test_case->end_time - cur_test_case->start_time) % 1000;
  sprintf(buf, TEST_FAILED_FMT, cur_test_case->test_name, failure_msg, seconds, ms);
  printf("%s", buf);
}

void printFinalmsg(u64 start_time, u64 end_time) {
  u32 seconds = (end_time - start_time) / 1000;
  u32 ms = (end_time - start_time) % 1000;
  i8 buf[512]{0};
  sprintf(buf, TEST_TOTAL_PASSED_FMT, passed_test);
  memset(buf, 0, 512);
  sprintf(buf, TEST_TOTAL_SUCCESS_FMT, succesed_test);
  memset(buf, 0, 512);
  sprintf(buf, TEST_TOTAL_FAILED_FMT, failed_test);
  memset(buf, 0, 512);
  sprintf(buf, TEST_TOTAL_TIME_FMT, seconds, ms);
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
      printTestCaseSuccessMsg();
    } else {
      failed_test++;
      printTestCaseFailMsg();
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

void __assertNotNull(ptr p, u32 line) {
  if (p == nullptr) {
    setFailureMsg("Pointer is NULL", line);
    cur_test_case->success = false;
  }
}

void __assertNull(ptr p, u32 line) {
  if (p != nullptr) {
    setFailureMsg("Pointer not NULL", line);
    cur_test_case->success = false;
  }
}

void __assertTrue(bool expression, u32 line) {
  if (!expression) {
    setFailureMsg("expression should be TRUE", line);
    cur_test_case->success = false;
  }
}

template <typename T, typename D>
void __assertEql(T a, D d, u32 line) {
  if (a != d) {
    setFailureMsg("Not Eql", line);
    cur_test_case->success = false;
  }
}
