#ifndef SRC_UITLS_LOG_H
#define SRC_UITLS_LOG_H

#include <cstdio>
#include <cstdarg>
#include <vector>

#include "logging/log_buffer.h"
#include "utils/macros.h"
#include "utils/printable_enum.h"

namespace picasso {
namespace logging {

// NORMAL

#define LOG_FATAL(fmt, ...) ::picasso::logging::LogBuffer::Log(0, ::picasso::logging::LogLevel::LOG_FATAL, __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOG_ERROR(fmt, ...) ::picasso::logging::LogBuffer::Log(0, ::picasso::logging::LogLevel::LOG_ERROR, __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOG_WARN(fmt, ...)  ::picasso::logging::LogBuffer::Log(0, ::picasso::logging::LogLevel::LOG_WARN,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOG_INFO(fmt, ...)  ::picasso::logging::LogBuffer::Log(0, ::picasso::logging::LogLevel::LOG_INFO,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOG_DEBUG(fmt, ...) ::picasso::logging::LogBuffer::Log(0, ::picasso::logging::LogLevel::LOG_DEBUG, __FILE__, __LINE__, fmt, ## __VA_ARGS__);

#define LOG_INDENT_FATAL(indent, fmt, ...) ::picasso::logging::LogBuffer::Log(indent, ::picasso::logging::LogLevel::LOG_FATAL,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOG_INDENT_ERROR(indent, fmt, ...) ::picasso::logging::LogBuffer::Log(indent, ::picasso::logging::LogLevel::LOG_ERROR,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOG_INDENT_WARN(indent, fmt, ...)  ::picasso::logging::LogBuffer::Log(indent, ::picasso::logging::LogLevel::LOG_WARN,   __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOG_INDENT_INFO(indent, fmt, ...)  ::picasso::logging::LogBuffer::Log(indent, ::picasso::logging::LogLevel::LOG_INFO,   __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOG_INDENT_DEBUG(indent, fmt, ...) ::picasso::logging::LogBuffer::Log(indent, ::picasso::logging::LogLevel::LOG_DEBUG,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);

#define LOG_SEPARATOR ::picasso::logging::LogBuffer::Log(0, ::picasso::logging::LogLevel::LOG_DEBUG, __FILE__, __LINE__, "---------------------------------");

#define LOG_STATUS(status) ::picasso::logging::LogBuffer::LogStatus(0, status, __FILE__, __LINE__);
#define LOG_NON_OK_STATUS(status) if (!status.Ok()) { LOG_STATUS(status); }
    
#define LOG_STATUS_OR(status_or) ::picasso::logging::LogBuffer::LogStatusOr(0, status_or, __FILE__, __LINE__);
#define LOG_NON_OK_STATUS_OR(status_or) if (!status_or.Ok()) { LOG STATUS_OR(status_or); }


#define LOG_INDENT_STATUS(indent, status) ::picasso::logging::LogBuffer::LogStatus(indent, status, __FILE__, __LINE__);
#define LOG_INDENT_STATUS_OR(indent, status_or) ::picasso::logging::LogBuffer::LogStatusOr(indent, status_or, __FILE__, __LINE__);

#define LOG_ARRAY(array, count) ::picasso::logging::LogBuffer::LogArray(array, count, __FILE__, __LINE__);

// STDERR

#define LOGERR_FATAL(fmt, ...) ::picasso::logging::LogBuffer::LogStderr(0, ::picasso::logging::LogLevel::LOG_FATAL, __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOGERR_ERROR(fmt, ...) ::picasso::logging::LogBuffer::LogStderr(0, ::picasso::logging::LogLevel::LOG_ERROR, __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOGERR_WARN(fmt, ...)  ::picasso::logging::LogBuffer::LogStderr(0, ::picasso::logging::LogLevel::LOG_WARN,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOGERR_INFO(fmt, ...)  ::picasso::logging::LogBuffer::LogStderr(0, ::picasso::logging::LogLevel::LOG_INFO,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOGERR_DEBUG(fmt, ...) ::picasso::logging::LogBuffer::LogStderr(0, ::picasso::logging::LogLevel::LOG_DEBUG, __FILE__, __LINE__, fmt, ## __VA_ARGS__);

#define LOGERR_INDENT_FATAL(indent, fmt, ...) ::picasso::logging::LogBuffer::LogStderr(indent, ::picasso::logging::LogLevel::LOG_FATAL,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOGERR_INDENT_ERROR(indent, fmt, ...) ::picasso::logging::LogBuffer::LogStderr(indent, ::picasso::logging::LogLevel::LOG_ERROR,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOGERR_INDENT_WARN(indent, fmt, ...)  ::picasso::logging::LogBuffer::LogStderr(indent, ::picasso::logging::LogLevel::LOG_WARN,   __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOGERR_INDENT_INFO(indent, fmt, ...)  ::picasso::logging::LogBuffer::LogStderr(indent, ::picasso::logging::LogLevel::LOG_INFO,   __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#define LOGERR_INDENT_DEBUG(indent, fmt, ...) ::picasso::logging::LogBuffer::LogStderr(indent, ::picasso::logging::LogLevel::LOG_DEBUG,  __FILE__, __LINE__, fmt, ## __VA_ARGS__);

#define LOGERR_SEPARATOR ::picasso::logging::LogBuffer::LogStderr(0, ::picasso::logging::LogLevel::LOG_DEBUG, __FILE__, __LINE__, "---------------------------------");

#define LOGERR_STATUS(status) ::picasso::logging::LogBuffer::LogStderrStatus(0, status, __FILE__, __LINE__);
#define LOGERR_STATUS_OR(status_or) ::picasso::logging::LogBuffer::LogStderrStatusOr(0, status_or, __FILE__, __LINE__);
#define LOGERR_INDENT_STATUS(indent, status) ::picasso::logging::LogBuffer::LogStderrStatus(indent, status, __FILE__, __LINE__);
#define LOGERR_INDENT_STATUS_OR(indent, status_or) ::picasso::logging::LogBuffer::LogStderrStatusOr(indent, status_or, __FILE__, __LINE__);


}   // namespace logging
}   // namespace picasso

#endif  // SRC_UTILS_LOG_H
