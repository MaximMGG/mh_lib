#include <mh/core.hpp>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

struct TestFile {
  String name;
  String test_dir;
  String compiler;
  String compile_string;
  DArr<String> test_cases;
};



void doTesting(const i8 *name, i32 argc, i8 **argv) {
  i32 fd = open(name, O_RDONLY);
  if (fd <= 0) {
    fprintf(stderr, "Can't open file %s\n", name);
    return;
  }
  u64 file_size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  i8 *test_buf = new i8 [file_size + 1];
  ZERO(test_buf, file_size + 1);
  u64 read_bytes = read(fd, test_buf, file_size);
  String content{test_buf};
  delete [] test_buf;
  if (read_bytes != file_size) {
    fprintf(stderr, "read syscall error, try to read %s file\n", name);
    return;
  }
}



i32 main(i32 argc, i8 **argv) {
  DIR *d = opendir(".");
  if (d == nullptr) {
    fprintf(stderr, "Can't open current dir\n");
    return 1;
  }
  bool find_test_file = false;

  struct dirent *dt;

  while((dt = readdir(d)) != nullptr) {
    i8 *dot_test = strstr(dt->d_name, ".test");
    if (dot_test != nullptr) {
      find_test_file = true;
      doTesting(dt->d_name, argc, argv);
    }
  }
  closedir(d);
  if (!find_test_file) {
    fprintf(stderr, "Do not found *.test file!\nIf you what to create default one, type: cpptest -create-default\n");
    return 1;
  }
}
