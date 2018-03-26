/******************************************************************************
 * @file: mapper.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_MAPPERS_MAPPER_H
#define SRC_MAPPERS_MAPPER_H

#include <map>
#include <set>
#include <functional>

#include "logging/log.h"
#include "utils/singleton.h"

namespace picasso {
namespace mappers {

using ::picasso::utils::Singleton;

template <typename DerivedMapper, typename FromRegistry, typename ToRegistry>
class Mapper : Singleton<Mapper<DerivedMapper, FromRegistry, ToRegistry>> {
 public:
  using FromKeyType = typename FromRegistry::KeyType;
  using ToKeyType = typename ToRegistry::KeyType;
  using FromToMap = std::map<FromKeyType, std::set<ToKeyType>>;

 public:
  static bool AddMapping(const FromKeyType& from_key, const ToKeyType& to_key) {
    auto& map = Instance().map_;
    auto& mapped_keys = map[from_key];
    auto res = mapped_keys.insert(to_key);
    // Only call the callback when it's a new mapping
    if (res.second) {
      return DerivedMapper::AddCallback(from_key, to_key);
    }
    return res.second;
  }

  static bool RemoveMapping(const FromKeyType& from_key, const ToKeyType& to_key) {
    auto& map = Instance().map_;
    auto from_it = map.find(from_key);
    if (from_it == map.end()) {
      return false;
    }

    auto& mapped_keys = from_it->second;
    auto to_it = mapped_keys.find(to_key);
    if (to_it == mapped_keys.end()) {
      return false;
    }

    // We call the callback, and only delete if the result is succesful
    if (DerivedMapper::RemoveCallback(from_key, to_key)) {
      mapped_keys.erase(to_it);
      return true;
    }
    return false;
  }

 protected:
  FromToMap map_;

 public:
  friend class Singleton<Mapper<DerivedMapper, FromRegistry, ToRegistry>>;
};  // class Mapper

}   // namespace mappers
}   // namespace picasso

#endif  // SRC_MAPPERS_MAPPER_H
