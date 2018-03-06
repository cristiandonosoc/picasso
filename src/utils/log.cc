#include "log.h"

#include <cstdio>
#include <cstdarg>

namespace picasso {
namespace utils {

#define DEFINE_LOG_FUNC(func_name, prefix, file)                              \
  void func_name(const char *fmt, ...) {                                      \
    char buf[1024];                                                           \
    snprintf(buf, sizeof(buf), prefix " %s\n", fmt);                          \
    va_list arglist;                                                          \
    va_start(arglist, fmt);                                                   \
    vfprintf(file, buf, arglist);                                             \
    fflush(file);                                                             \
    va_end(arglist);                                                          \
  }

#define DEFINE_INDENT_LOG_FUNC(func_name, prefix, file)                       \
  void func_name(size_t indent, const char *fmt, ...) {                       \
    char buf[1024];                                                           \
    snprintf(buf, sizeof(buf), prefix " %*s%s\n", (int)indent, " ", fmt);     \
    va_list arglist;                                                          \
    va_start(arglist, fmt);                                                   \
    vfprintf(file, buf, arglist);                                             \
    fflush(file);                                                             \
    va_end(arglist);                                                          \
  }

#define SEPARATOR(file)                                                       \
  void Separator(int length, const char *c) {                                 \
    if (length > 128) { length = 128; }                                       \
    char buf[128 + 1];                                                        \
    char *p = (char*)&buf;                                                    \
    for (int i = 0; i < length; i++) { *p++ = *c; }                           \
    *p = 0;                                                                   \
    fprintf(file, "%s\n", buf);                                               \
    fflush(file);                                                             \
  }

namespace logout {

DEFINE_LOG_FUNC(Info, "[INFO]", stdout);
DEFINE_LOG_FUNC(Warn, "[WARN]", stdout);
DEFINE_LOG_FUNC(Error, "[ERROR]", stdout);
DEFINE_LOG_FUNC(Debug, "[DEBUG]", stdout);

DEFINE_INDENT_LOG_FUNC(IndentInfo, "[INFO]", stdout);
DEFINE_INDENT_LOG_FUNC(IndentWarn, "[WARN]", stdout);
DEFINE_INDENT_LOG_FUNC(IndentError, "[ERROR]", stdout);
DEFINE_INDENT_LOG_FUNC(IndentDebug, "[DEBUG]", stdout);

SEPARATOR(stdout);

}   // namespace logout

namespace logerr {

DEFINE_LOG_FUNC(Info, "[INFO]", stderr);
DEFINE_LOG_FUNC(Warn, "[WARN]", stderr);
DEFINE_LOG_FUNC(Error, "[ERROR]", stderr);
DEFINE_LOG_FUNC(Debug, "[DEBUG]", stderr);

DEFINE_INDENT_LOG_FUNC(IndentInfo, "[INFO]", stderr);
DEFINE_INDENT_LOG_FUNC(IndentWarn, "[WARN]", stderr);
DEFINE_INDENT_LOG_FUNC(IndentError, "[ERROR]", stderr);
DEFINE_INDENT_LOG_FUNC(IndentDebug, "[DEBUG]", stderr);

SEPARATOR(stdout);

}   // namespace logerr

}   // namespace utils
}   // namespace picasso


