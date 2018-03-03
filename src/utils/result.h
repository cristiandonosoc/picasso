#ifndef SRC_UTILS_RESULT_H
#define SRC_UTILS_RESULT_H

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <type_traits>

namespace picasso {
namespace utils {

/**
 * ResultOr<T>
 * -----------
 *
 * Class that represents a failure | T. This is the way we use to indicate that
 * the return could be invalid.
 *
 * Usage:
 *  T val;
 *  ResultOr<T> res = ReturnThatMightFail();
 *  if (res.Valid()) {
 *    val = std::move(res.Consume());
 *  } else {
 *    fprintf(stderr, "ERROR: %s\n", res.ErrorMsg().c_str());
 *  }
 **/
template<typename T>
class ResultOr {
 // We forbid ResultOr bool
 static_assert(!std::is_same<T, bool>::value,
               "ResultOr<bool> is forbidden");

 /**
  * PUBLIC INTERFACE
  **/
 public:
  // NOTE(Cristian): Disabled construction by copy, because it's very easy
  //                 to simply write
  //
  //                 return expensive_to_copy_value;
  //
  //                 and not realize we're making a copy.
  //                 If it's too annoying, perhaps I'll enable it
  static ResultOr Success(T&& t) {
    return ResultOr(std::move(t));
  }
  ResultOr(const T& t) : val_(t), valid_(true) {}
  ResultOr(T&& t) : val_(std::move(t)), valid_(true) {}

  static ResultOr Error(const char *fmt,  ...) {
    va_list arglist;
    va_start(arglist, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, arglist);
    va_end(arglist);

    auto res = ResultOr();
    res.error_msg_ = buffer;
    return res;
  }
  static ResultOr Error(const std::string& error_msg) {
    auto res = ResultOr();
    res.error_msg_ = error_msg;
    return res;
  }

 public:
  T&& ConsumeOrDie() {
    assert(valid_);
    valid_ = false;
    return std::move(val_);
  }
  bool Valid() const { return valid_; }
  const std::string& ErrorMsg() const { return error_msg_; }

 private:
  ResultOr() : valid_(false) {}

 private:
  T val_;
  bool valid_;
  std::string error_msg_;
};


}   // namespace utils
}   // namespace picasso

#endif
