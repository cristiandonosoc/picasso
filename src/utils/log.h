#ifndef SRC_UITLS_LOG_H
#define SRC_UITLS_LOG_H

#include <cstdio>
#include <cstdarg>

namespace picasso {
namespace utils {

#define DEFINE_LOG_FUNC(func_name, prefix, file)                  \
  void func_name(const char *fmt, ...) {                   \
    char buf[1024];                                                       \
    snprintf(buf, sizeof(buf), prefix " %s\n", fmt);   \
    va_list arglist;                                                      \
    va_start(arglist, fmt);                                               \
    vfprintf(file, buf, arglist);                                         \
    fflush(file);                                                         \
    va_end(arglist);                                                      \
  }



#define DEFINE_INDENT_LOG_FUNC(func_name, prefix, file)                  \
  void func_name(int indent, const char *fmt, ...) {                   \
    char buf[1024];                                                       \
    snprintf(buf, sizeof(buf), prefix " %*s%s\n", indent, " ", fmt);   \
    va_list arglist;                                                      \
    va_start(arglist, fmt);                                               \
    vfprintf(file, buf, arglist);                                         \
    fflush(file);                                                         \
    va_end(arglist);                                                      \
  }

namespace logout {

DEFINE_LOG_FUNC(Info, "[INFO]", stdout);
DEFINE_LOG_FUNC(Warn, "[WARN]", stdout);
DEFINE_LOG_FUNC(Error, "[ERROR]", stdout);

DEFINE_INDENT_LOG_FUNC(IndentInfo, "[INFO]", stdout);
DEFINE_INDENT_LOG_FUNC(IndentWarn, "[WARN]", stdout);
DEFINE_INDENT_LOG_FUNC(IndentError, "[ERROR]", stdout);

}   // namespace logout

namespace logerr {

DEFINE_LOG_FUNC(Info, "[INFO]", stderr);
DEFINE_LOG_FUNC(Warn, "[WARN]", stderr);
DEFINE_LOG_FUNC(Error, "[ERROR]", stderr);

DEFINE_INDENT_LOG_FUNC(IndentInfo, "[INFO]", stderr);
DEFINE_INDENT_LOG_FUNC(IndentWarn, "[WARN]", stderr);
DEFINE_INDENT_LOG_FUNC(IndentError, "[ERROR]", stderr);

}   // namespace logerr

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_LOG_H
