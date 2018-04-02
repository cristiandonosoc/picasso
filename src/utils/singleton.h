/******************************************************************************
 * @file: singleton.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_SINGLETON_H
#define SRC_UTILS_SINGLETON_H

namespace picasso {
namespace utils {

template<typename T>
class Singleton {
 public:
  static T& Instance() {
    static T instance;
    return instance;
  }
};

}   // namespace utils
}   // namespace picasso



#endif  // SRC_UTILS_SINGLETON_H
