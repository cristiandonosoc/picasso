/******************************************************************************
 * @file: make_unique.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: Implementation of std::make_unique for C++11
 ******************************************************************************/

#ifndef SRC_UTILS_MAKE_UNIQUE_H
#define SRC_UTILS_MAKE_UNIQUE_H

#include <memory>

namespace picasso {
namespace utils {

template<typename T, typename... Args>
std::unique_ptr<T> MakeUnique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_MAKE_UNIQUE_H
