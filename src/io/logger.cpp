#include "../../headers/io/logger.hpp"
#include "../../headers/core/error.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

InerLogger *__logger = NULL;
i8 logger_buffer[4096] = {0};

void loggerInit(Logger_Type type, Logger_Opt opt, const i8 *f_name, i32 fd) {
  if (__logger != NULL) {
    errorPrintError("Logger already initialized, if you need one more logger,  create obj Logger");
    return;
  }
  __logger = new InerLogger;
  __logger->type = type;
  __logger->opt = opt;
  switch(type) {
    case LOGGER_FILE: {
      if (access(f_name, F_OK) == 0) {
        __logger->fd = open(f_name, O_WRONLY | O_APPEND); 
        if (__logger->fd < 0) {
          errorPrintError("Can't open file for initialize logger: %s", f_name);
          delete __logger;
          return;
        }
      } else {
        __logger->fd = open(f_name, O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); 
        if (__logger->fd < 0) {
          errorPrintError("Can't open file for initialize logger: %s", f_name);
          delete __logger;
          return;
        }
      }
    } break;
    case LOGGER_CONSOLE: {
      __logger->fd = STDERR_FILENO;
    } break;
    case LOGGER_FD: {
      __logger->fd = fd;
    } break;
    case LOGGER_SOCKET: {
      //TODO(Maxim) write this later
    } break;
  }
}

void loggerCleanup() {
  delete __logger;
}

void __inerLog(InerLogger *log, const i8 *level, const i8 *file, const i8 *func, i32 line, const char *fmt, ...) {
  va_list li;
  va_start(li, fmt);
  memset(logger_buffer, 0, 4096);
  u32 log_size = 0;
  log_size += sprintf(logger_buffer, "[%s] => ", level);
  if (__logger->opt.file) {
    log_size += sprintf(&logger_buffer[log_size], "File-%s ", file);
  }
  if (__logger->opt.func) {
    log_size += sprintf(&logger_buffer[log_size], "Func-%s ", func);
  }
  if (__logger->opt.line) {
    log_size += sprintf(&logger_buffer[log_size], "Line %d ", line);
  }
  if (__logger->opt.time) {

    i64 t = time(NULL);
    struct tm *loc = localtime(&t);
    log_size += sprintf(&logger_buffer[log_size], "%04d:%02d:%02d %02d:%02d:%02d", loc->tm_year + 1900, loc->tm_mon + 1, loc->tm_mday, loc->tm_hour, loc->tm_min, loc->tm_sec);
  }
  log_size += sprintf(&logger_buffer[log_size], " => ");
  log_size += vsprintf(&logger_buffer[log_size], fmt, li);
  log_size += sprintf(&logger_buffer[log_size], "\n");
  i32 write_bytes = write(__logger->fd, logger_buffer, log_size);
  if (write_bytes != log_size) {
    errorPrintError("Log error, logger_buffer size not equal write bytes -> %d %d\n", write_bytes, log_size);
  }
}
