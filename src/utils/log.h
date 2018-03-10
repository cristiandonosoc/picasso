#ifndef SRC_UITLS_LOG_H
#define SRC_UITLS_LOG_H

#include <cstdio>
#include <cstdarg>

namespace picasso {
namespace utils {
namespace log {

/* enum LogLevel { */
/*   DEBUG, */
/*   INFO, */
/*   WARN, */
/*   ERROR, */
/* }; */

enum LogLevel {
  LOG_FATAL = 0,
  LOG_ERROR = 100,
  LOG_WARN  = 200,
  LOG_INFO  = 300,
  LOG_DEBUG = 400,
};

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#define PRINTF_FORMAT_ATTRIBUTE(fmt_one_index, varargs_one_index) \
  __attribute__ ((format (printf, fmt_one_index, varargs_one_index)))
#else
#define PRINTF_FORMAT_ATTRIBUTE(ignore1, ignore2)
#endif

void Log(FILE *output, LogLevel, const char *file, int line,
         int indent, const char *fmt, ...) PRINTF_FORMAT_ATTRIBUTE(6, 7);
void Separator(FILE *output, int length = 30, const char *c = "-");

#define LOGOUT_FATAL(fmt, ...) ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_FATAL, __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGOUT_ERROR(fmt, ...) ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_ERROR, __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGOUT_WARN(fmt, ...)  ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_WARN,  __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGOUT_INFO(fmt, ...)  ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_INFO,  __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGOUT_DEBUG(fmt, ...) ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_DEBUG, __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGOUT_INDENT_FATAL(indent, fmt, ...) ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_FATAL,  __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGOUT_INDENT_ERROR(indent, fmt, ...) ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_ERROR,  __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGOUT_INDENT_WARN(indent, fmt, ...)  ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_WARN,   __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGOUT_INDENT_INFO(indent, fmt, ...)  ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_INFO,   __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGOUT_INDENT_DEBUG(indent, fmt, ...) ::picasso::utils::log::Log(stdout, ::picasso::utils::log::LogLevel::LOG_DEBUG,  __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGOUT_SEPARATOR ::picasso::utils::log::Separator(stdout);

#define LOGERR_FATAL(fmt, ...) ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_FATAL, __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGERR_ERROR(fmt, ...) ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_ERROR, __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGERR_WARN(fmt, ...)  ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_WARN,  __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGERR_INFO(fmt, ...)  ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_INFO,  __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGERR_DEBUG(fmt, ...) ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_DEBUG, __FILE__, __LINE__, 0, fmt, __VA_ARGS__); 
#define LOGERR_INDENT_FATAL(indent, fmt, ...) ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_FATAL,  __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGERR_INDENT_ERROR(indent, fmt, ...) ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_ERROR,  __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGERR_INDENT_WARN(indent, fmt, ...)  ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_WARN,   __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGERR_INDENT_INFO(indent, fmt, ...)  ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_INFO,   __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGERR_INDENT_DEBUG(indent, fmt, ...) ::picasso::utils::log::Log(stderr, ::picasso::utils::log::LogLevel::LOG_DEBUG,  __FILE__, __LINE__, indent, fmt, __VA_ARGS__); 
#define LOGERR_SEPARATOR ::picasso::utils::log::Separator(stderr);

};

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_LOG_H
