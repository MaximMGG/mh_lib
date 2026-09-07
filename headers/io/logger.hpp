#ifndef MH_LOGGER_HPP
#define MH_LOGGER_HPP
#include "../core.hpp"



enum Logger_Type {
  LOGGER_CONSOLE, LOGGER_FILE, LOGGER_SOCKET, LOGGER_FD
};

struct Logger_Opt {
  bool line;
  bool file;
  bool func;
  bool time;
};


typedef struct {
  Logger_Opt opt;
  Logger_Type type;
  i32 fd;
} InerLogger;

void loggerInit(Logger_Type type = LOGGER_CONSOLE, 
    Logger_Opt opt = (Logger_Opt){.line = true, .file = true, .func = true, .time = true}, const i8 *f_name = "", i32 fd = -1);
void loggerCleanup();
void __inerLog(InerLogger *log, const i8 *level, const i8 *file, const i8 *func, i32 line, const char *fmt, ...);

extern InerLogger *__logger;

#define TRACE "TRACE" 
#define INFO  "INFO"
#define DEBUG "DEBUG"
#define ERROR "ERROR"
#define FATAL "FATAL"

#define LOG(level, fmt, ...) __inerLog(__logger, level, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)


#endif //MH_LOGGER_HPP
