/******************************************************************************
 * @file: arena.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-02
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_MEMORY_ARENA_H
#define SRC_MEMORY_ARENA_H

#include <bitset>
#include <cstdint>

#include "utils/singleton.h"
#include "utils/macros.h"

namespace picasso {
namespace memory {

using ::picasso::utils::Singleton;

template <typename T, size_t ARENA_SIZE>
class Arena : public Singleton<Arena<T, ARENA_SIZE>> {
 public:
  static constexpr size_t ARENA_SIZE = ARENA_SIZE;
  static constexpr size_t ARENA_BYTE_SIZE = ARENA_SIZE * sizeof(T);

 private:
  Arena() = default;
  DISABLE_COPY(Arena);
  DISABLE_MOVE(Arena);

 public:
  static T* Allocate() {
    auto& instance = Instance();
    // Search for a free element
    for (size_t i = 0; i < ARENA_SIZE; i++) {
      if (!instance.used_[i]) {
        return instance.arena_ + i * sizeof(T);
      }
    }
    // No more space
    return nullptr;
  }

  static void Deallocate(T* ptr) {
    if (!ptr) {
      return;
    }
    auto& instance = Instance();
    // We see if that pointer was allocated
    auto diff = ptr - instance.arena_;
    assert((diff >= 0) &&
           (diff < ARENA_BYTE_SIZE) &&
           ((diff % sizeof(T)) == 0));
    int index = diff / sizeof(T);
    assert(instance.used_[index]);
    instance.used_[index] = 0;
  }

 private:
  uint8_t arena_[ARENA_BYTE_SIZE] = {};  // Starts at 0
  std::bitset<ARENA_SIZE> used_;
};  // class Arena

template <typename T, typename ArenaAllocator>
class ArenaDeleter {
 public:
  void operator()(T* ptr) {
    std::allocator_traits<ArenaAllocator>::deallocate(ptr);
  }
};  // class ArenaDeleter

template<typename T, size_t ARENA_SIZE = 128>
class ArenaAllocator {
 public:
  using value_type = T;

 public:
  ArenaAllocator() noexcept {}
  ~ArenaAllocator() noexcept {}

 public:
  template <typename U>
  ArenaAllocator(const ArenaAllocator<U>&) noexcept {}

 public:
  T* allocate(size_t n) {
    assert(n == 1);
    return Arena<T, ARENA_SIZE>::Allocate();
  }

  void deallocate(T* ptr, size_t n) {
    assert(n == 1);
    return Arena<T, ARENA_SIZE>::Deallocate(ptr);
  }

 public:
};  // class ArenaAllocator

template <typename T, typename U>
constexpr bool operator== (const ArenaAllocator<T>&, const ArenaAllocator<U>&) noexcept {
  return false;
}
// Specialization
template <typename T>
constexpr bool operator==(const ArenaAllocator<T>&, const ArenaAllocator<T>&) noexcept {
  return true;
}

template <typename T, typename U>
constexpr bool operator!= (const ArenaAllocator<T>&, const ArenaAllocator<U>&) noexcept {
  return true;
}
// Specialization
template <typename T>
constexpr bool operator!=(const ArenaAllocator<T>&, const ArenaAllocator<T>&) noexcept {
  return false;
}



}   // namespace memory
}   // namespace picasso

#endif  // SRC_MEMORY_ARENA_H
