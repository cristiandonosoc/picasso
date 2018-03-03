#ifndef SRC_UTILS_RESULT_H
#define SRC_UTILS_RESULT_H

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
  ResultOr(T&& t) : val_(std::move(t)), valid_(true) {}
  static ResultOr Invalid(const char *fmt,  ...) {
    va_list arglist;
    va_start(arglist, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, arglist);
    va_end(arglist);

    auto res = ResultOr();
    res.msg_ = buffer;
    return res;
  }

 public:
  T&& Consume() {
    assert(valid_);
    valid_ = false;
    return std::move(val_);
  }
  bool Valid() const { return valid_; }
  const std::string& ErrorMsg() const { return msg_; }

 private:
  ResultOr() : val_(false) {}

 private:
  T val_;
  bool valid_;
  std::string msg_;
};


}   // namespace utils
}   // namespace picasso

#endif
