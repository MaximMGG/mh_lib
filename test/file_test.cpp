#include <mh/testing.hpp>
#include <mh/io/file.hpp>
#include <stdio.h>

TEST(file_write_test) {
  File f("for_test_file.txt");
  f.open(FILE_WRITE);

  String hello("Hello world!");
  f.write(hello);

  u64 file_size = f.size();

  assertTrue(hello.len == file_size);
}

TEST(file_read_line_test) {
  File f("log_test.cpp");
  f.open(FILE_READ);

  String line = f.readLine();

  assertTrue(line.len > 0);
  printf("%s\n", line.data);

}

TEST(file_read_until_del_test) {
  File f("log_test.cpp");
  f.open(FILE_READ);

  String line = f.readUntilDelimeter('\n');

  assertTrue(line.len > 0);
  printf("%s\n", line.data);
}


TEST(file_trunc_test) {
  File f("for_test_file.txt");
  f.trunc();

  String s = f.readEntyreFile();
  assertTrue(s.len == 0);
}


TEST(read_entyre_file_test) {
  File f("log_test.cpp");
  assertTrue(f.ok);
  f.open(FILE_READ);

  String content = f.readEntyreFile();
  printf("%s\nLen: %d", content.data, content.len);
}


TEST(file_read_test) {
  File f("log_test.cpp");
  assertTrue(f.ok);
  f.open(FILE_READ);

  String content = f.read(100);
  assertTrue(content.len == 100);
}

TEST(file_open_test) {
  File f("log_test.cpp");

  f.open(FILE_READ);
  assertTrue(f.ok);
}


TEST_LIST(file_open_test, 
    file_read_test, 
    read_entyre_file_test, 
    file_trunc_test, 
    file_read_until_del_test, 
    file_read_line_test,
    file_write_test
    );

