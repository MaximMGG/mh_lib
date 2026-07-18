#ifndef MH_TESTING_HPP
#define MH_TESTING_HPP
#include "core/types.hpp"


struct TestCase {
  i8 *test_name;
  void (*test_func)();
  u64 start_time;
  u64 end_time;
  bool success;
};

TestCase *prepareTestCases(i8 *tests, i32 test_list_len, ...);


#define TEST(test_name) void test_name()
#define TEST_LIST(tests)                                                       \
  int main() {                                                                 \
    int test_list_len = 0;                                                     \
    TestCase **list = prepareTestCases(#tests, &test_list_len, tests, nullptr);	\
    runTest(list, test_list_len, __FILE__);					\
    cleanup(list, test_list_len);                                              \
}


#define assertNotNull(p) __assertNotNull(p, __LINE__)


#endif //MH_TESTING_HPP
