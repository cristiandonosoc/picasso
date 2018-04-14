/******************************************************************************
 * @file: status.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-26
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_STATUS_H
#define SRC_UTILS_STATUS_H

#include <cassert>
#include <cstdarg>
#include <string>

#include "utils/macros.h"

namespace picasso {
namespace utils {

#define PROCESS_FMT_VAR_ARGS(var_name, size)            \
  va_list arglist;                                      \
  va_start(arglist, fmt);                               \
  char buffer[1024];                                    \
  vsnprintf(buffer, sizeof(buffer), fmt, arglist);      \
  va_end(arglist);

#define FILENO_STATUS(status, fmt, ...) \
  { __FILE__, __LINE__, status, fmt, __VA_ARGS__ }

#define RETURN_OR_ASSIGN(varname, status)                                     \
  if (!status.Ok()) {                                                         \
    return status;                                                            \
  }                                                                           \
  auto varname = status.ConsumeOrDie() 

#define RETURN_IF_NOT_OK(stmt) \
  auto COMBINE(status, __LINE__) = stmt; \
  if (!COMBINE(status, __LINE__).Ok()) { return COMBINE(status, __LINE__); }


// NOTE(Cristian): Those geniuses at X11 decided that it was a good idea
//                 name their macros Status.... sigh
#ifdef Status
#undef Status
#endif

class Status {
 public:
  enum StatusEnum {
    STATUS_NONE,
    STATUS_OK,
    STATUS_ERROR,
    STATUS_WARNING,
    STATUS_CONSUMED,
    STATUS_FAILED
  };  // enum StatusEnum

 public:
  Status(StatusEnum status, const std::string& error_msg = "")
    : status_(status), error_msg_(error_msg) {}

  PRINTF_FORMAT_ATTRIBUTE(3, 4)
  Status(StatusEnum status, const char *fmt, ...) : status_(status) {
    PROCESS_FMT_VAR_ARGS(buffer, 1024);
    error_msg_ = buffer;
  }

  // FILE:NUMBER INTERFACE
 public:
  Status(const std::string& file, int line,
         StatusEnum status, const std::string& error_msg = "")
    : status_(status), error_msg_(error_msg),
      file_(file), line_(line) {}

  PRINTF_FORMAT_ATTRIBUTE(5, 6)
  Status(const std::string& file, int line,
         StatusEnum status, const char *fmt, ...)
    : status_(status), file_(file), line_(line) {
    PROCESS_FMT_VAR_ARGS(buffer, 1024);
    error_msg_ = buffer;
  }

 public:
  DEFAULT_COPY(Status);
  DEFAULT_MOVE(Status);

 public:
  bool Ok() const { return status_ == StatusEnum::STATUS_OK; }
  StatusEnum GetStatus() const { return status_; }
  const std::string& GetErrorMsg() const { return error_msg_; }
  bool HasFileLineInfo() const { return !file_.empty(); }
  const std::string& GetFile() const { return file_; }
  int GetLine() const { return line_; }

 protected:
  StatusEnum status_ = StatusEnum::STATUS_OK;
  std::string error_msg_;
  std::string file_;
  int line_ = 0;
};  // class Status

template <typename T>
class StatusOr : public Status {
 public:
  static_assert(!std::is_same<T, StatusEnum>::value,
                "Cannot create StatusOr<StatusEnum>. Just return Status instead");
 public:
  DISABLE_COPY(StatusOr);
  DEFAULT_MOVE(StatusOr);

 public:
  // TODO(Cristian): Do we want a copy constructor
  StatusOr(T&& t) : Status(StatusEnum::STATUS_OK), val_(std::move(t)) {}
  StatusOr(const T& t) : Status(StatusEnum::STATUS_OK), val_(t) {}

  StatusOr(StatusEnum status, const std::string& error_msg = "")
    : Status(status, error_msg) {
    assert(status != StatusEnum::STATUS_OK);
  }

  PRINTF_FORMAT_ATTRIBUTE(3, 4)
  StatusOr(StatusEnum status, const char *fmt, ...) : Status(status) {
    assert(status != StatusEnum::STATUS_OK);
    PROCESS_FMT_VAR_ARGS(buffer, 1024);
    error_msg_ = buffer;
  }

  // FILE:NUMBER INTERFACE
 public:
  StatusOr(const std::string& file, int line,
         StatusEnum status, const std::string& error_msg = "")
    : Status(file, line, status, error_msg) {}

  PRINTF_FORMAT_ATTRIBUTE(5, 6)
  StatusOr(const std::string& file, int line,
         StatusEnum status, const char *fmt, ...)
    : Status(file, line, status) {
    PROCESS_FMT_VAR_ARGS(buffer, 1024);
    error_msg_ = buffer;
  }

  // PIPING ANOTHER STATUS
 public:
  StatusOr(const Status& other_status)
    : Status(other_status.GetStatus(), other_status.GetErrorMsg()) {}

  template <typename U>
  StatusOr(const StatusOr<U>& other_status)
    : Status(other_status.GetStatus(), other_status.GetErrorMsg()) {}

 public:
  T&& ConsumeOrDie() {
    if (!Ok()) {
      fprintf(stderr, "ConsumeOrDie called on invalid StatusOr. ErrorMsg: %s\n", 
                      error_msg_.c_str());
      assert(Ok());
    }
    return std::move(val_);
  }

 public:
  Status DowncastToStatus() {
    return Status(status_, error_msg_);
  }

 private:
  T val_;
};  // class StatusOr

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_STATUS_H
