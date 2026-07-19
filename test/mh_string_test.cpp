#include <mh/core/string.hpp>
#include <mh/testing.hpp>
#include <string.h>
#include <stdio.h>

TEST(replace_test) {
  String a{"Complitly this disaster, becouse this string for this replace test this"};
  a.replace("this", "BUZZUZ");
  printf("'%s'\n", a.data);
}

TEST(index_of_test) {
  String a{"Complitly disaster @ string with symbols &(#)"};
  i32 i = a.indexOf('#');
  printf("Len of string %d, index of '#' is %d\n", a.len, i);
  assertTrue(i == a.len - 2);

  i32 string_i = a.indexOf("string");
  printf("String index: %d\n", string_i);
  assertTrue(string_i == 21);
}


TEST(trim_test) {
  String a{"  Bugaga\n"};
  a.trim();
  assertTrue(a == "Bugaga");
}

TEST(substring_test) {
  String a{"String for fubstring!"};
  String b = a.substring(7, 10);
  assertTrue(b == "for");
}


TEST(start_with_test) {
  String a{"What's going on?"};
  assertTrue(a.startsWith("What"));
  assertTrue(!a.startsWith("MY"));
}

TEST(ends_with_test) {

  String a{"What's going on, buah"};

  assertTrue(a.endsWith("buah"));
  assertTrue(!a.endsWith("ijij"));
}


TEST(new_add_string_test) {
  String a{"Hello"};

  String b = a + " world";

  assertTrue(b == "Hello world");
}


TEST(multiple_add_test) {
  String a("Readdy");
  for(i32 i = 0; i < 10000; i++) {
    a += " a";
  }
  assertTrue(a.len > 10000);
  //printf("%s\n", a.data);
}


TEST(add_string_test) {
  String a("Hello");
  a += " world";

  assertTrue(strcmp(a.data, "Hello world") == 0);
}


TEST(create_string_test) {
  String a("Hello world");

  assertTrue(strcmp(a.data, "Hello world") == 0);
}


TEST_LIST(create_string_test, add_string_test, multiple_add_test, new_add_string_test, ends_with_test, start_with_test, substring_test, trim_test, index_of_test, replace_test);
