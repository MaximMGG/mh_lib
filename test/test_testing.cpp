#include "../headers/testing.hpp"


TEST(one_test) {
  assertTrue(2 == 2);
  assertEql(3, 8);
}

int main() {
  int test_list_len = 0;
  TestCase *list = prepareTestCases("one_test", &test_list_len, one_test, nullptr);
  runTest(list, test_list_len, "/home/mhrun/cppprogramming/mh_lib/test/test_testing.cpp");
  cleanup(list, test_list_len);
};
