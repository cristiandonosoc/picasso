/******************************************************************************
 * @file: status_or.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-15
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_STATUS_OR_H
#define SRC_UTILS_STATUS_OR_H

#include <cassert>
#include <cstdarg>
#include <string>

#include "utils/macros.h"

namespace picasso {
namespace utils {

enum class Status {
  STATUS_NONE,
  STATUS_OK,
  STATUS_ERROR,
  STATUS_CONSUMED
};

template <typename T>
class StatusOr {
 public:
  static_assert(!std::is_same<T, Status>::value,
                "Cannot create StatusOr<Status>. Just return Status instead");
    
 public:
  StatusOr() = default;
  // TODO(Cristian): Do we want a copy constructor
  StatusOr(T&& t) : val_(std::move(t)), status_(Status::STATUS_OK) {}
  StatusOr(const T& t) : val_(t), status_(Status::STATUS_OK) {}
  StatusOr(Status status, const std::string& error_msg = "") 
    : status_(status), error_msg_(error_msg) {
    assert(status != Status::STATUS_OK);
  }
  StatusOr(Status status, const char *fmt, ...) : status_(status) {
    va_list arglist;
    va_start(arglist, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, arglist);
    va_end(arglist);
    error_msg_ = buffer;
  }

 public:
  static StatusOr Error(const std::string& error_msg) {
    return { Status::STATUS_ERROR, error_msg };
  }

  /// Factory for easier mesage generation
  static StatusOr Error(const char *fmt,  ...) {
    va_list arglist;
    va_start(arglist, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, arglist);
    va_end(arglist);

    return { Status::STATUS_ERROR, buffer };
  }

 public:
  bool Ok() const  { return status_ == Status::STATUS_OK; }
  Status GetStatus() const { return status_; }
  const std::string& ErrorMsg() const { return error_msg_; }
  T&& ConsumeOrDie() { 
    assert(Ok());
    status_ = Status::STATUS_CONSUMED;
    return std::move(val_); 
  }

 private:
  T val_;
  Status status_ = Status::STATUS_NONE;
  std::string error_msg_;
};  // class StatusOr

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_STATUS_OR_H
