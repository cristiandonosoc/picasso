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
#include <type_traits>

#include "logging/log.h"
#include "utils/singleton.h"
#include "utils/status.h"

namespace picasso {
namespace mappers {

using ::picasso::utils::Singleton;
using ::picasso::utils::Status;

/**
 * Base class for a mapping (key to key) between two Registries.
 *
 * @tparam DerivedMapper the class that is inheriting from this Mapper
 *  this is using the CRTP pattern.
 *  The DerivedMapper needs a TypeName static member that is transformable
 *  to const char*, which should hold the the typename.
 *  This is used for debugging purposes.
 * @tparam FromRegistry the registry we're mapping from.
 *  Must inhering from ::picasso::utils::Registry
 * @tparam ToRegistry the registry we're mapping to.
 *  Must inhering from ::picasso::utils::Registry
 */
template <typename DerivedMapper, typename FromRegistry, typename ToRegistry>
class Mapper : Singleton<Mapper<DerivedMapper, FromRegistry, ToRegistry>> {
 public:
  using FromKeyType = typename FromRegistry::KeyType;
  using ToKeyType = typename ToRegistry::KeyType;
  using FromToMap = std::map<FromKeyType, std::set<ToKeyType>>;
  using Singleton<Mapper<DerivedMapper, FromRegistry, ToRegistry>>::Instance;

 public:
  /*************************************************************
   * ADDING
   *************************************************************/

  /**
   * Mappers go one way (FromKey -> ToKey). It's one to many.
   */
  static Status AddMapping(const FromKeyType& from_key, const ToKeyType& to_key) {
    auto& map = Instance().map_;
    auto& mapped_keys = map[from_key];
    auto it = mapped_keys.find(to_key);
    if (it != mapped_keys.end()) {
      return { Status::STATUS_WARNING,
               "Mapper \"%s\": Mapping (%s) -> (%s) already exists",
               DerivedMapper::TypeName().c_str(), from_key.c_str(), to_key.c_str() };
    }

    // We check if the job was actually succesful
    auto add_res = DerivedMapper::AddCallback(from_key, to_key);
    if (!add_res.Ok()) {
      return add_res;
    }

    // Finally we add (we know we don't have a mapping here)
    mapped_keys.insert(to_key);
    return Status::STATUS_OK;
  }

  static Status RemoveMapping(const FromKeyType& from_key, const ToKeyType& to_key) {
    auto& map = Instance().map_;
    auto from_it = map.find(from_key);
    if (from_it == map.end()) {
      return FILENO_STATUS(Status::STATUS_WARNING,
             "Mapper \"%s\": Cannot find key \"%s\"",
             DerivedMapper::TypeName().c_str(), from_key.c_str());
    }

    auto& mapped_keys = from_it->second;
    auto to_it = mapped_keys.find(to_key);
    if (to_it == mapped_keys.end()) {
      return { Status::STATUS_WARNING,
               "Mapper \"%s\": Mapping (%s) -> (%s) doesn't exists",
               DerivedMapper::TypeName().c_str(), from_key.c_str(), to_key.c_str() };
    }

    // We call the callback, and only delete if the result is succesful
    auto remove_res = DerivedMapper::RemoveCallback(from_key, to_key);
    if (remove_res.Ok()) {
      mapped_keys.erase(to_it);
    }
    return remove_res;
  }

 protected:
  FromToMap map_;

 public:
  friend class Singleton<Mapper<DerivedMapper, FromRegistry, ToRegistry>>;
};  // class Mapper

}   // namespace mappers
}   // namespace picasso

#endif  // SRC_MAPPERS_MAPPER_H
