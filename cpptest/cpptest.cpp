#include <mh/core.hpp>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

struct TestCase {
  String test_dir;
  String compiler;
  String generic_compile_string;
  DArr<String> test_cases;
};


const i8 *default_test_json = "{\n"
"\"test_directory\": \"./test\",\n"
"\"compiler\": \"g++\",\n"
"\"save_exe\": false,\n"
"\"log_file_name\": \"\",\n"
"\"test_cases\": []\n"
"}\n";

void printHelp() {
  printf("-c -> Just compile, without running tests\n");
  printf("-g -> Comile with debug symbols\n");
  printf("-create_default -> Create default test.json with default content\n");
  printf("-show_default_json -> Print default content of test.json\n");
  printf("-v -> Run tests with 'valgrind'\n");
  printf("-save_exe -> Will not delete executable after test run. \n\t\t"
         "If you want this behaviour be default, specify in test.json (\"save_exe\": true)\n");
}

void printDefaultJson() {
  printf("%s\n", default_test_json);
}

void createDefaultTestJson() {
  i32 fd = open("test.json", O_CREAT | O_TRUNC | O_RDWR, 777);
  if (fd <= 0) {
    fprintf(stderr, "Can't create test.json file\n");
    return;
  }

  i64 write_bytes = write(fd, default_test_json, strlen(default_test_json));
  if (write_bytes < 0) {
    fprintf(stderr, "Failed to write default json content to test.json\n");
    close(fd);
    return;
  }
  
  close(fd);
}

bool checkTestFileExists() {
  if (access("test.json", F_OK) != 0) {
    return false;
  } return true;
}


void doTesting(const i8 *name, i32 argc, i8 **argv) {

}

i32 main(i32 argc, i8 **argv) {
  if (!checkTestFileExists()) {
    fprintf(stderr, "Can't find test.json file, if you want to create default enter [cpptest -create_default]\n");
    return 1;
  }

  for(i32 i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-create_default") == 0) {
      createDefaultTestJson();
      return 0;
    }
    if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
      printHelp();
      return 0;
    }
    if (strcmp(argv[i], "-show_default_json") == 0) {
      printDefaultJson();
      return 0;
    }
    
  }
}


