/******************************************************************************
 * @file: key.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-13
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: A class type that uses an underlying common type but can 
 *               only be compared against it's own type
 ******************************************************************************/

#ifndef SRC_UTILS_KEY_H
#define SRC_UTILS_KEY_H

#include "utils/macros.h"
#include <sstream>

namespace picasso {
namespace utils {

template <typename T>
class Key {
 public:
  Key() = default;
  Key(const T& t) : value_(t) {}
  DEFAULT_COPY(Key);
  DEFAULT_MOVE(Key);

 public:
  bool operator==(const Key& key) const {
    return value_ == key.value_;
  }
  bool operator!=(const Key& key) const {
    return value_ != key.value_;
  }

  bool operator<(const Key& key) const {
    return value_ < key.value_;
  }

 public:
  const T& Value() const { return value_; }

  std::string ToString() const {
    std::stringstream ss;
    ss << value_;
    return ss.str();
  }

  

 private:
  T value_;
};  // class Key

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_KEY_H
