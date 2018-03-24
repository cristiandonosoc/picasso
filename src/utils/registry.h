/******************************************************************************
 * @file: registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-24
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_REGISTRY_H
#define SRC_UTILS_REGISTRY_H

#include <map>

#include "utils/macros.h"
#include "utils/singleton.h"

namespace picasso {
namespace utils {

template <typename ParentClass, typename Key, typename Value>
class Registry : Singleton<Registry<ParentClass, Key, Value>> {
 protected:
  Registry() = default;
  DISABLE_COPY(Registry);
  DISABLE_MOVE(Registry);

 
 public:
  static Registry& Instance() {
    static Registry instance;
    return instance;
  }

 private:
  std::map<Key, Value> map_;

 public:
  using RegistryMapType = decltype(map_);

 public:
  friend class Singleton<Registry<ParentClass, Key, Value>>;
  friend ParentClass;
};  // class Registry

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_REGISTRY_H
