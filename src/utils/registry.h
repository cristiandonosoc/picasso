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
#include <memory>

#include "utils/macros.h"
#include "utils/singleton.h"
#include "utils/status.h"
#include "memory/arena.h"

namespace picasso {
namespace utils {

using ::picasso::memory::ArenaDeleter;

template <typename ParentClass,
          typename Key, typename Value,
          typename ArenaAllocator = std::allocator<Value>>
class Registry : Singleton<Registry<ParentClass, Key, Value,
                                    ArenaAllocator>> {
 public:
  using KeyType = Key;
  using ValueType = Value;

 public:
  using AllocatorTraits = std::allocator_traits<ArenaAllocator>;
  using ArenaUniquePtr = std::unique_ptr<Value,
                                         ArenaDeleter<Value, ArenaAllocator>>;

 public:
  using Singleton<Registry<ParentClass, Key, Value, ArenaAllocator>>::Instance;

 protected:
  Registry() = default;
  DISABLE_COPY(Registry);
  DISABLE_MOVE(Registry);

 public:
  static StatusOr<ValueType*> Register(const KeyType& key) {
    auto& instance = Instance();
    auto it = instance.map_.find(key);
    if (it != instance.map_.end()) {
      return FILENO_STATUS(Status::STATUS_ERROR,
                           "Key \"%s\" already exists!",
                           key.c_str());
    }
    ArenaAllocator allocator;
    ArenaUniquePtr value(AllocatorTraits::allocate(allocator, 1));
    if (!value) {
      return FILENO_STATUS(Status::STATUS_ERROR,
                           "Could not allocate memory for key \"%s\"",
                           key.c_str());
    }
    instance.map_[key] = std::move(value);
    return instance.map_[key].get();
  }

  static Status Unregister(const KeyType& key) {
    auto& instance = Instance();
    auto it = instance.map_.find(key);
    if (it == instance.map_.end()) {
      return FILENO_STATUS(Status::STATUS_ERROR,
                           "Key \"%s\" already exists!",
                           key.c_str());
    }
    instance.map_.erase(it);
    return Status::STATUS_OK;
  }

 private:
  std::map<KeyType, ArenaUniquePtr> map_;

 public:
  using RegistryMapType = decltype(map_);
  static const RegistryMapType& GetMap() {
    return Instance().map_;
  }

 public:
  friend class Singleton<Registry<ParentClass, Key, Value, ArenaAllocator>>;
  friend ParentClass;
};  // class Registry

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_REGISTRY_H
