/******************************************************************************
 * @file: dynamic_array.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-10
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_DYNAMIC_ARRAY_H
#define SRC_UTILS_DYNAMIC_ARRAY_H

#include <memory>

#include "utils/macros.h"

namespace picasso {
namespace utils {

template <typename T>
class DynamicArray {
 public:
  DynamicArray() = default;
  DynamicArray(size_t count) : count_(count), buffer_(new T[count_]) {
    ClearArray();
  }
  DynamicArray(size_t count, T* array) 
    : count_(count), buffer_(new T[count_]) {
    FillArray(count, array);
  }

 public:
  DISABLE_COPY(DynamicArray);
  DynamicArray(DynamicArray&&) noexcept = default;
  DynamicArray& operator=(DynamicArray&&) noexcept = default;

 public:
  void Reset() {
    buffer_.reset();
    count_ = 0;
  }
  void Reset(size_t count) {
    count_ = count;
    buffer_.reset(new T[count_]);
    ClearArray();
  }
  void Reset(size_t count, T* array) {
    count_ = count;
    buffer_.reset(new T[count_]);
    FillArray(count_, array);
  }

 public:
  T *Get() { return buffer_.get(); }
  size_t Count() const { return count_; }
  size_t Size() const { return sizeof(T) * count_; }

 private:
  void FillArray(size_t count, T* array) {
    T *ptr = Get();
    for (size_t i = 0; i < count; i++) {
      *ptr++ = *array++;
    }
  }

  void ClearArray() {
    T *ptr = Get();
    for (size_t i = 0; i < count_; i++) {
      *ptr++ = 0;
    }
  }

 private:
  size_t count_ = 0;
  std::unique_ptr<T[]> buffer_;
};  // class DynamicArray

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_DYNAMIC_ARRAY_H
