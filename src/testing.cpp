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
#define TEST_TOTAL_FAILED_FMT "TOTAL " COLOR_RED "FAILED" COLOR_RESET " TESTS: %d\n"
#define TEST_TOTAL_TIME_FMT   "TOTAL TIME : %.3lfs\n"

#define TEST_SUCCES_FMT     " ==> Test Case '%s' " COLOR_GREEN "SUCCESS" COLOR_RESET " : %.3lfs\n"
#define TEST_FAILED_FMT     " ==> Test Case '%s' " COLOR_RED "FAILED" COLOR_RESET " %s : %.3lfs\n"


i8 failure_msg[256]{0};

u32 passed_test = 0;
u32 succesed_test = 0;
u32 failed_test = 0;
TestCase *cur_test_case;

u32 testsLen(const i8 *tests) {
  i8 *tmp = cast(i8 *, tests);
  while(*(tmp++) != '\0' && (*tmp) != '\n');

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
  f64 sec = (f64(cur_test_case->end_time) - f64(cur_test_case->start_time)) / 1000000.0;

  sprintf(buf, TEST_SUCCES_FMT, cur_test_case->test_name, sec);
  printf("%s", buf);
}

void printTestCaseFailMsg() {
  i8 buf[512]{0};
  f64 sec = (f64(cur_test_case->end_time) - f64(cur_test_case->start_time)) / 1000000.0;
  sprintf(buf, TEST_FAILED_FMT, cur_test_case->test_name, failure_msg, sec);
  printf("%s", buf);
}

void printFinalMsg(u64 start_time, u64 end_time) {
  i8 buf[512]{0};
  sprintf(buf, TEST_TOTAL_PASSED_FMT, passed_test);
  printf("%s", buf);
  memset(buf, 0, 512);
  sprintf(buf, TEST_TOTAL_SUCCESS_FMT, succesed_test);
  printf("%s", buf);
  memset(buf, 0, 512);
  sprintf(buf, TEST_TOTAL_FAILED_FMT, failed_test);
  printf("%s", buf);
  memset(buf, 0, 512);
  f64 sec = (f64(end_time) - f64(start_time)) / 1000000.0;
  sprintf(buf, TEST_TOTAL_TIME_FMT, sec);
  printf("%s", buf);
}

void runTest(TestList *list, const i8 *test_file_name) {
  printf("==================RUN \x1b[3;7;32m%s\x1b[0m TEST FILE=================\n", test_file_name);
  u64 start_time = clock();
  for (i32 i = 0; i < list->len; i++) {
    cur_test_case = &list->cases[i];
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
  printFinalMsg(start_time, end_time);
  delete(list);
}

TestList::TestList() {
  this->len = 0;
  this->cap = 8;
  this->cases = new TestCase [this->cap];
}

TestList::~TestList() {
  for(i32 i = 0; i < this->len; i++) {
    delete [] this->cases[i].test_name;
  }
  delete [] this->cases;
}

void TestList::add(const char *func_name, u32 name_len) {
  if (this->len == this->cap) {
    this->cap <<= 1;
    TestCase *new_arr = new TestCase [this->cap];
    memmove(new_arr, this->cases, sizeof(TestCase) * this->len);
    delete [] this->cases;
    this->cases = new_arr;
  }

  this->cases[this->len].test_name = new i8 [name_len + 1];
  __strcpy(this->cases[this->len].test_name, (i8 *)func_name, name_len);
  this->len++;
}


TestList *prepareTestCases(const i8 *tests, ...) {
  
  TestList *list = new TestList{};

  u32 test_i = 0;
  u32 tests_len = strlen(tests);

  i8 buf[128]{0};
  i32 buf_i = 0;

  for(i32 i = 0; i < tests_len; i++) {
    if (tests[i] == ',') {
      list->add(buf, buf_i);
      memset(buf, 0, 128);
      buf_i = 0;
      test_i++;
      i++;
    } else {
      buf[buf_i++] = tests[i];
    }
  }
  list->add(buf, buf_i);

  test_i = 0;
  va_list li;
  va_start(li, tests);

  ptr p;

  while (true) {
    p = va_arg(li, ptr);
    if (p == nullptr) {
      break;
    }
    list->cases[test_i].success = true;
    list->cases[test_i++].test_func = (void (*)())p;
  }

  va_end(li);
  return list;
}

void __assertNotNull(ptr p, u32 line) {
  if (p == nullptr) {
    if (cur_test_case->success) {
      setFailureMsg("Pointer is NULL", line);
      cur_test_case->success = false;
    }
  }
}

void __assertNull(ptr p, u32 line) {
  if (p != nullptr) {
    if (cur_test_case->success) {
      setFailureMsg("Pointer not NULL", line);
      cur_test_case->success = false;
    }
  }
}

void __assertTrue(bool expression, u32 line) {
  if (!expression) {
    if (cur_test_case->success) {
      setFailureMsg("expression should be TRUE", line);
      cur_test_case->success = false;
    }
  }
}

void setCurTestCaseStatus(bool stat) {
  cur_test_case->success = stat;
}

bool getCurTestCaseStatus() {
  return cur_test_case->success;
}
