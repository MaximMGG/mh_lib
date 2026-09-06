#ifndef MH_LOGGER_HPP
#define MH_LOGGER_HPP
#include "../core.hpp"



enum Logger_Type {
  CONSOLE, FILE, SOCKET
};

struct Logger_Opt {
  bool line;
  bool file;
  bool func;
  bool time;
};

class Logger {
public:
  Logger();
  Logger(const char *file_name);
  Logger(i32 fd);
  ~Logger();
  void setOpt();
  void TRACE_log(const char *fmt, ...);
  void INFO_log(const char *fmt, ...);
  void DEBUG_log(const char *fmt, ...);
  void ERROR_log(const char *fmt, ...);
  void FATAL_log(const char *fmt, ...);
private:
  Logger_Opt opt = (Logger_Opt){.line = true, .file = true, .func = true, .time = true};
  Logger_Type type;
  i32 fd;
};

extern Logger __logger;

#define TRACE 
#define INFO 
#define DEBUG 
#define ERROR 
#define FATAL 

#define LOG(level, fmt, ...) level ## _log(fmt, __VA_ARGS__)


#endif //MH_LOGGER_HPP
