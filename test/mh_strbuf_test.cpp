#include <mh/testing.hpp>
#include <mh/core/string.hpp>
#include <stdio.h>
#include <string.h>


TEST(strbuf_append_test) {
  StrBuf a{};
  a.append("Hello");
  a.append(' ');
  a.append("world");

  printf("%s\n", a.data);
  assertTrue(strcmp(a.data, "Hello world") == 0);
}

TEST(strbuf_create_test) {
  StrBuf a{};
}


TEST_LIST(strbuf_create_test, strbuf_append_test);
