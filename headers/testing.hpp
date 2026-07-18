#ifndef MH_TESTING_HPP
#define MH_TESTING_HPP



#endif //MH_TESTING_HPP

#define TEST(test_name) void test_name()

#define TEST_LIST(tests)                                                       \
  int main() {                                                                 \
    int test_list_len = 0;                                                     \
    char **list = prepareTestCases(#tests, &test_list_len);                    \
    runTest(list, test_list_len);                                              \
    cleanup(list, test_list_len);                                              \
}


#define assertNotNull(p) __assertNotNull(p, __LINE__)
