/******************************************************************************
 * @file: log_buffer.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-14
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <cstdarg>
#include <chrono>

#include "logging/log_buffer.h"

namespace picasso {
namespace logging {

namespace {

static std::map<LogLevel, const char*> level_map = {
  { LogLevel::LOG_DEBUG,  "[DEBUG]" },
  { LogLevel::LOG_INFO,   "[INFO]"  },
  { LogLevel::LOG_WARN,   "[WARN]"  },
  { LogLevel::LOG_ERROR,  "[ERROR]" },
};

std::string CreateEntryMsg(int indent, LogLevel level, const char *file, int line,
                        const char *fmt, va_list arglist) {
  const char *prefix = level_map[level];
  char fmt_buf[128];
  // Log output will be like
  // <INDENT>[INFO] (main.cc:234): Created shader
  
  if (file) {
    snprintf(fmt_buf, sizeof(fmt_buf),  "%*s%s (%s:%d): %s", 
             indent, " ", prefix, file, line, fmt);
  } else {
    snprintf(fmt_buf, sizeof(fmt_buf), "%*s%s: %s", indent, " ", prefix, fmt);
  }

  char buf[1024];
  vsnprintf(buf, sizeof(buf), fmt_buf, arglist);

  // Return the created buffer
  return buf;
}
                        

}   // namespace

void LogBuffer::Clear() {
  PrivateInstance().container_.clear();
}

void LogBuffer::AddEntry(LogLevel level, const std::string& msg) {
  using Clock = std::chrono::system_clock;
  Entry entry;
  entry.msg = msg;
  entry.level = level;

  // Obtain the timing
  auto now = Clock::now();
  auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);

  entry.time = Clock::to_time_t(now);
  entry.us = (size_t)std::chrono::duration_cast<std::chrono::microseconds>(now - seconds).count();
  PrivateInstance().container_.push_back(std::move(entry));
}

void LogBuffer::Log(int indent, LogLevel level, const char *file, int line, 
                    const char *fmt, ...) {
  va_list arglist;
  va_start(arglist, fmt);
  std::string msg = CreateEntryMsg(indent, level, file, line, fmt, arglist);
  va_end(arglist);
  PrivateInstance().AddEntry(level, msg);
}

void LogBuffer::LogStderr(int indent, LogLevel level, const char *file, int line, 
                    const char *fmt, ...) {
  va_list arglist;
  va_start(arglist, fmt);
  std::string msg = CreateEntryMsg(indent, level, file, line, fmt, arglist);
  va_end(arglist);
  PrivateInstance().AddEntry(level, msg);
  fprintf(stderr, "%s\n", msg.c_str());
  fflush(stderr);
}

size_t LogBuffer::Count() {
  return PrivateInstance().container_.size();
}

const LogBuffer::LogContainer& LogBuffer::GetLogs() {
  return PrivateInstance().container_;
}



}   // namespace logging
}   // namespace picasso


