/******************************************************************************
 * @file: log_buffer.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-14
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_LOGGING_LOG_BUFFER_H
#define SRC_LOGGING_LOG_BUFFER_H

#include <string>
#include <vector>

#include "utils/macros.h"
#include "utils/printable_enum.h"

namespace picasso {
namespace logging {

PRINTABLE_ENUM(LogLevel, LOG_FATAL, LOG_ERROR, LOG_WARN,
                         LOG_INFO, LOG_DEBUG);

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#define PRINTF_FORMAT_ATTRIBUTE(fmt_one_index, varargs_one_index) \
  __attribute__ ((format (printf, fmt_one_index, varargs_one_index)))
#else
#define PRINTF_FORMAT_ATTRIBUTE(ignore1, ignore2)
#endif

/* void Log(FILE *output, LogLevel, const char *file, int line, */
/*          int indent, const char *fmt, ...) PRINTF_FORMAT_ATTRIBUTE(6, 7); */

/**
 * Singleton in charge of keeping the log entries
 */
class LogBuffer {
 public:
  using LogContainer = std::vector<std::string>;

 public:
  static void Clear();
  static void Log(int indent, LogLevel, const char *file, int line, 
                  const char *fmt, ...);
  static void LogStderr(int indent, LogLevel, const char *file, int line, 
                        const char *fmt, ...); 

 public:
  static const LogBuffer& Instance() { return PrivateInstance(); }

 private:
  static LogBuffer& PrivateInstance() {
    static LogBuffer instance;
    return instance;
  }

 private:
  LogBuffer() = default;
  DISABLE_COPY(LogBuffer);
  DISABLE_MOVE(LogBuffer);

 public:
  LogContainer::iterator begin() { return container_.begin(); }
  LogContainer::iterator end() { return container_.end(); }
  LogContainer::reverse_iterator rbegin() { return container_.rbegin(); }
  LogContainer::reverse_iterator rend() { return container_.rend(); }
 
 public:
  const LogContainer& Logs = container_;

 private:
  LogContainer container_;

};  // class LogBuffer



}   // namespace logging
}   // namespace picasso

#endif  // SRC_LOGGING_LOG_BUFFER_H
