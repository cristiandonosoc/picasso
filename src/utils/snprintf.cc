#include <cstdarg>
#include <cstdio>

#include "utils/snprintf.h"


namespace picasso {
namespace utils {

int picasso_snprintf(char *buf, size_t size, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
#ifndef _WIN32
  int res = vsnprintf(buf, size, fmt, args);
#else
  int res = vsnprintf_s(buf, size, size - 1, fmt, args);
#endif
  va_end(args);
  return res;
}

}   // namespace utils
}   // namespace picasso
