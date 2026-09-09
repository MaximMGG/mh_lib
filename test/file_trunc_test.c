#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main() {
  int fd = open("for_test_file.txt", O_CREAT | O_TRUNC, 666);
  printf("%d\n", fd);

  return 0;
}
