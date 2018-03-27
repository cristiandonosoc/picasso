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
#include <ctime>

#include "utils/macros.h"
#include "utils/printable_enum.h"
#include "utils/status.h"

namespace picasso {
namespace logging {

using ::picasso::utils::Status;
using ::picasso::utils::StatusOr;

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
  class Entry {
   public:
    LogLevel level;
    std::string msg;
    time_t time;
    size_t us;
  };  // struct LogBuffer::Entry


 public:
  using LogContainer = std::vector<Entry>;

 public:
  static void Clear();
  static size_t Count();
  static void Log(int indent, LogLevel, const char *file, int line, 
                  const char *fmt, ...);
  static void LogStderr(int indent, LogLevel, const char *file, int line, 
                        const char *fmt, ...); 
  static void LogStatus(int indent, const Status&, const char *file, int line);
  template<typename T>
  static void LogStatusOr(int indent, const StatusOr<T>& s, const char *file, int line) {
    LogStatus(indent, s.DowncastToStatus(), file, line);
  }

  static const LogContainer& GetLogs();

 private:
  static LogBuffer& PrivateInstance() {
    static LogBuffer instance;
    return instance;
  }

 private:
  LogBuffer() = default;
  DISABLE_COPY(LogBuffer);
  DISABLE_MOVE(LogBuffer);

 private:
  void AddEntry(LogLevel, const std::string& msg);

 public:
  LogContainer::iterator begin() { return container_.begin(); }
  LogContainer::iterator end() { return container_.end(); }
  LogContainer::reverse_iterator rbegin() { return container_.rbegin(); }
  LogContainer::reverse_iterator rend() { return container_.rend(); }
 

 private:
  LogContainer container_;

};  // class LogBuffer

}   // namespace logging
}   // namespace picasso

#endif  // SRC_LOGGING_LOG_BUFFER_H
