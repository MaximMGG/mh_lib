#ifndef MY_LIB_ERROR_HPP
#define MY_LIB_ERROR_HPP

void errorPrintError(const char *fmt, ...);

#define LIB_ERROR(fmt, ...) errorPrintError("MH LIB ERROR:" ##fmt, ##__VA_ARGS__)

#endif//MY_LIB_ERROR_HPP
