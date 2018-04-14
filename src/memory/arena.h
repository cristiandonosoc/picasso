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

#include <cassert>
#include <bitset>
#include <cstdint>

#include "utils/singleton.h"
#include "utils/macros.h"
#include "logging/log.h"
#include "utils/status.h"

namespace picasso {
namespace memory {


using ::picasso::utils::Status;
using ::picasso::utils::StatusOr;

template<typename T, size_t ARENA_SIZE = 128>
class Arena {
 public:
  struct Result {
    size_t index;   // Used for deallocating
    T *instance;
  };

 public:
  using ValueType = T;
  static constexpr size_t ArenaSize = ARENA_SIZE;
  static constexpr size_t ArenaByteSize = ARENA_SIZE * sizeof(ValueType);

 public:
  Arena() = default;
  ~Arena() {

  }
  DISABLE_COPY(Arena);
  DISABLE_MOVE(Arena);

 public:
  int GetCount() const { return count_; }

 public:
  StatusOr<T*> Get(size_t index) const {
    assert(index < ArenaSize);
    if (!used_[index]) {
      return { Status::STATUS_FAILED,
               "Cannot get element with index %zu", index };
    }
    return (T*)arena_ + index;
  }

  std::vector<Result> GetActive() const {
    std::vector<Result> result;
    result.reserve(ArenaSize);

    T *base = (T*)arena_;
    for (size_t i = 0; i < ArenaSize; i++) {
      if (used_[i]) {
        result.push_back({i, base + i});
      }
    }
    return result;
  }

 public:
  template <typename... Args>
  StatusOr<Result> Create(Args&&... args) {
    LOG_DEBUG("CREATING NEW OBJECT");
    int index = GetNextAvailableSlot();
    if (index < 0) {
      LOG_ERROR("CANNOT ALLOCATE NEW OBJECT");
      return { Status::STATUS_ERROR, "Cannot allocate new object" };
    }

    // In-place construction the object
    T *slot = (T*)arena_ + index;
    ::new ((void*)slot) T(std::forward<Args>(args)...);
    used_[index] = 1;
    count_++;
    Result res;
    res.index = (size_t)index;
    res.instance = slot;
    return res;
  }

  Status Destroy(size_t index) {
    assert(index < ArenaSize);
    assert(used_[index]);
    // Obtain the pointer
    T *slot = ((T*)arena_) + index;
    // Call the destructor
    slot->~T();
    // Mark memory as free
    used_[index] = 0;
    count_--;

    return Status::STATUS_OK;
  }

 private:
  int GetNextAvailableSlot() {
    int index = -1;
    // Linear search for a free element
    // TODO(Cristian): Do we want a more claver scheme for this?
    for (size_t i = 0; i < ArenaSize; i++) {
      if (!used_[i]) {
        index = i;
        break;
      }
    }
    return index;
  }

 private:
  uint8_t arena_[ArenaByteSize] = {};   // starts at 0
  std::bitset<ArenaSize> used_;
  int count_ = 0;
};  // class Arena

}   // namespace memory
}   // namespace picasso

#endif  // SRC_MEMORY_ARENA_H
