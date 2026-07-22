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

class TestList {
public:
  TestCase *cases;
  u32 len = 0;
  u32 cap;

  TestList();
  ~TestList();
  void add(const char *func_name, u32 name_len);
};
  
TestList *prepareTestCases(const i8 *tests, ...);
void runTest(TestList *list, const i8 *test_file_name);
void cleanup(TestCase *test_list, i32 test_list_len);
//void registerTestCase(const i8 *test_name, void (*test_func)());


#define TEST(test_name) void test_name()
#define TEST_LIST(...)                                                \
  int main() {                                                                 \
    TestList *list = prepareTestCases(#__VA_ARGS__,  __VA_ARGS__, nullptr);          \
    runTest(list, __FILE__);                                    \
    return 0;\
  }

void __assertNotNull(ptr p, u32 line);
void __assertNull(ptr p, u32 line);
void __assertTrue(bool expression, u32 line);

// template <typename T, typename D>
// void __assertEql(T a, D d, u32 line);


void setFailureMsg(const i8 *msg, u32 line);
void setCurTestCaseStatus(bool stat);
bool getCurTestCaseStatus();

template <typename T, typename B>
void __assertEql(T a, B b, u32 line) {
  if (a != b) {
    if (getCurTestCaseStatus()) {
      setFailureMsg("Not eql, line %d", line);
      setCurTestCaseStatus(false);
    }
  }
}

template <typename T>
void __assertEql(T a, T b, u32 line) {
  if (a != b) {
    if (getCurTestCaseStatus()) {
      setFailureMsg("Not eql, line %d", line);
      setCurTestCaseStatus(false);
    }
  }
}

void __assertEql(const i8 *a, const i8 *b, u32 line);
void __assertEql(i8 *a, const i8 *b, u32 line);
void __assertEql(i8 *a, i8 *b, u32 line);
void __assertEql(const i8 *a, i8 *b, u32 line);


#define assertNotNull(p) __assertNotNull(p, __LINE__)
#define assertNull(p) __assertNull(p, __LINE__)
#define assertTrue(exp) __assertTrue(exp, __LINE__)
#define assertEql(a, b) __assertEql(a, b, __LINE__)


#endif //MH_TESTING_HPP
