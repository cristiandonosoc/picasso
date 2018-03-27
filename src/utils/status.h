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

class Status {
 public:
  enum StatusEnum {
    STATUS_NONE,
    STATUS_OK,
    STATUS_ERROR,
    STATUS_WARNING,
    STATUS_CONSUMED
  };  // enum StatusEnum

 public:
  Status(StatusEnum status, const std::string& error_msg = "") 
    : status_(status), error_msg_(error_msg) {}
  Status(StatusEnum status, const char *fmt, ...) : status_(status) {
    va_list arglist;
    va_start(arglist, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, arglist);
    va_end(arglist);
    error_msg_ = buffer;
  }

 public:
  DEFAULT_COPY(Status);
  DEFAULT_MOVE(Status);

 public:
  bool Ok() const { return status_ == StatusEnum::STATUS_OK; }
  StatusEnum GetStatus() const { return status_; }
  const std::string& GetErrorMsg() const { return error_msg_; }

 protected:
  StatusEnum status_ = StatusEnum::STATUS_OK;
  std::string error_msg_;
};  // class Status

template <typename T>
class StatusOr : public Status {
 public:
  static_assert(!std::is_same<T, StatusEnum>::value,
                "Cannot create StatusOr<StatusEnum>. Just return Status instead");
    
 public:
  // TODO(Cristian): Do we want a copy constructor
  StatusOr(T&& t) : Status(StatusEnum::STATUS_OK), val_(std::move(t)) {}
  StatusOr(const T& t) : Status(StatusEnum::STATUS_OK), val_(t) {}

  StatusOr(StatusEnum status, const std::string& error_msg = "") 
    : Status(status, error_msg) {
    assert(status != StatusEnum::STATUS_OK);
  }

 public:
  DISABLE_COPY(StatusOr);
  DEFAULT_MOVE(StatusOr);

  // Piping another status
 public:
  template <typename U>
  StatusOr(const StatusOr<U>& other_status) 
    : Status(other_status.GetStatus(), other_status.GetErrorMsg()) {}

  StatusOr(StatusEnum status, const char *fmt, ...) : Status(status) {
    assert(status != StatusEnum::STATUS_OK);
    va_list arglist;
    va_start(arglist, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, arglist);
    va_end(arglist);
    error_msg_ = buffer;
  }

 public:
  T&& ConsumeOrDie() {
    assert(Ok());
    return std::move(val_);
  }

 private:
  T val_;
};  // class StatusOr

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_STATUS_H
