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
#include "utils/key.h"

namespace picasso {
namespace utils {

using ::picasso::utils::Key;
using ::picasso::memory::Arena;

template <typename ParentClass, typename TValueType>
class Registry : Singleton<ParentClass> {
 public:
  using KeyType = Key<size_t>;
  using ValueType = TValueType;

 public:
  using ArenaType = Arena<TValueType, 1024>;
  class Result {
   public:
    Result() = default;
    Result(const typename ArenaType::Result& r) : key(r.index), value(r.instance) {}

   public:
    KeyType key;
    ValueType *value;
  };  // class Registry::Result

 public:
  /* using Singleton<Registry<ParentClass, TValueType>>::Instance; */
  using Singleton<ParentClass>::Instance;

 protected: Registry() {
    elements.reserve(ArenaType::ArenaSize);
  }
  DISABLE_COPY(Registry);
  DISABLE_MOVE(Registry);

 protected:
  template<typename... Args>
  static StatusOr<Result> Register(Args&&... args) {
    auto& instance = Instance();
    // Calling custom allocator
    auto res = instance.arena_.Create(std::forward<Args>(args)...);
    RETURN_OR_ASSIGN(arena_result, res);
    return res.ConsumeOrDie();
  }

 public:
  static Status Unregister(const KeyType& key) {
    auto& instance = Instance();
    return instance.arena_.Destroy(key.Value());
  }

 public:
  static StatusOr<TValueType*> Get(const KeyType& key) {
    return Instance().arena_.Get(key.Value());
  }

  static const std::vector<Result>& GetElements() { 
    auto& instance = Instance();
    const auto& actives = Instance().arena_.GetActive();
    instance.elements.clear();
    for (auto&& active : actives) {
      instance.elements.push_back(active);
    }
    return instance.elements;
  }

 private:
  ArenaType arena_;
  std::vector<Result> elements;

 public:
  friend class Singleton<Registry<ParentClass, TValueType>>;
  friend ParentClass;
};  // class Registry

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_REGISTRY_H
