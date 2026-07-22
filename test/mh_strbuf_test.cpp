#include <mh/testing.hpp>
#include <mh/core/string.hpp>
#include <stdio.h>
#include <string.h>


TEST(strbuf_revers_test) {
  StrBuf a{};

  a << "qwertyuiop[]";

  a.reverse();
  printf("%s\n", a.data);
  assertEql(a.data, "][poiuytrewq");
}

TEST(strbuf_append_format_test) {
  StrBuf a{};

  a.appendFmt("%s %s!", "hello", "world");

  assertEql(a.data, "hello world!");
  a.appendFmt("%s %.2f %-10s", "ww", 0.01231, "qweryu");
  printf("%s\n", a.data);
}


TEST(strbuf_append_operator_test) {
  StrBuf a{};
  a << "hello";
  a << ' ';
  a << " world";
  assertEql(a.data, "hello  world");
}


TEST(strbuf_append_test) {
  StrBuf a{};
  a.append("Hello");
  a.append(' ');
  a.append("world");

  printf("%s\n", a.data);
  //assertTrue(strcmp(a.data, "Hello world") == 0);
  assertEql(a.data, "Hello world");
}

TEST(strbuf_create_test) {
  StrBuf a{};
}


TEST_LIST(strbuf_create_test, strbuf_append_test, strbuf_append_operator_test, strbuf_append_format_test, strbuf_revers_test);
