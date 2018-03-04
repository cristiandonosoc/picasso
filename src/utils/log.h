#ifndef SRC_UITLS_LOG_H
#define SRC_UITLS_LOG_H

#include <cstdio>
#include <cstdarg>

namespace picasso {
namespace utils {

namespace logout {

void Info(const char *fmt, ...);
void Warn(const char *fmt, ...);
void Error(const char *fmt, ...);
void Debug(const char *fmt, ...);

void IndentInfo(size_t indent, const char *fmt, ...);
void IndentWarn(size_t indent, const char *fmt, ...);
void IndentError(size_t indent, const char *fmt, ...);
void IndentDebug(size_t indent, const char *fmt, ...);

}   // namespace logout

namespace logerr {

void Info(const char *fmt, ...);
void Warn(const char *fmt, ...);
void Error(const char *fmt, ...);
void Debug(const char *fmt, ...);

void IndentInfo(size_t indent, const char *fmt, ...);
void IndentWarn(size_t indent, const char *fmt, ...);
void IndentError(size_t indent, const char *fmt, ...);
void IndentDebug(size_t indent, const char *fmt, ...);

}   // namespace logerr

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_LOG_H
