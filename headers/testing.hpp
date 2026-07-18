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

TestCase *prepareTestCases(const i8 *tests, i32 *test_list_len, ...);
void runTest(TestCase *test_list, i32 test_list_len, const i8 *test_file_name);
void cleanup(TestCase *test_list, i32 test_list_len);


#define TEST(test_name) void test_name()
#define TEST_LIST(tests)                                                       \
  int main() {                                                                 \
    int test_list_len = 0;                                                     \
    TestCase *list = prepareTestCases(#tests, &test_list_len, tests, nullptr);	\
    runTest(list, test_list_len, __FILE__);					\
    cleanup(list, test_list_len);                                              \
}

void __assertNotNull(ptr p, u32 line);
void __assertNull(ptr p, u32 line);
void __assertTrue(bool expression, u32 line);

template <typename T, typename D>
void __assertEql(T a, D d, u32 line);

#define assertNotNull(p) __assertNotNull(p, __LINE__)
#define assertNull(p) __assertNull(p, __LINE__)
#define assertTrue(exp) __assertTrue(exp, __LINE__)
#define assertEql(a, b) __assertEql(a, b, __LINE__)


#endif //MH_TESTING_HPP
