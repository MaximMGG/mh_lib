#include "../headers/testing.hpp"


TEST(one_test) {
  assertTrue(2 == 2);
  //  assertEql(3, 8);
  __assertEql(3, 3, __LINE__);
 }


TEST(two_test) {
  i32 a = 3;
  i32 *b = &a;
  assertNotNull(b);
}

TEST_LIST(one_test, two_test);
