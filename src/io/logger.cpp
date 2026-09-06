#include "../../headers/io/logger.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

Logger __logger;

Logger::Logger() {
  __logger = *this;
}

Logger::Logger(const char *file_name) {
  if (access(file_naem) == F_OK) {

  }

}

Logger::Logger(i32 fd) {

}
