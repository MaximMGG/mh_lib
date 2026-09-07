#include "../../headers/core/error.hpp"
#include "../../headers/core/types.hpp"

#include <stdio.h>
#include <stdarg.h>

void errorPrintError(const char *fmt, ...) {
  va_list li;
  va_start(li, fmt);
  i8 buf[1024]{0};
  vsprintf(buf, fmt, li);
  fprintf(stderr, "LIB MH ERROR: %s\n", buf);
  va_end(li);
}
