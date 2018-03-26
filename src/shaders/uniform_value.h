/******************************************************************************
 * @file: uniform_value.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_SHADERS_UNIFORM_VALUE_H
#define SRC_SHADERS_UNIFORM_VALUE_H

#include <cstdint>
#include <memory>

#include "assets/shaders/shader.h"
#include "utils/dynamic_array.h"


namespace picasso {
namespace shaders {

using ::picasso::assets::shaders::Shader;
using ::picasso::assets::shaders::Uniform;
using ::picasso::utils::DynamicArray;

class UniformValue {
 public:
  UniformValue() = default;
  UniformValue(const Uniform *uniform);

 public:
  const Uniform* GetUniform() const { return uniform_; }

 public:
  // TODO(Cristian): Move to explicit interface
  template <typename T>
  T *GetValue() { return (T*)backend_.Get(); }
  template <typename T>
  const T *GetValue() const { return (T*)backend_.Get(); }

 public:
  // TODO(Cristian): Move to explicit interface
  template <typename T>
  void SetValue(const T&);
  template <typename T>
  void SetValues(size_t count, const T*);

 public:
  // Send the value to the GPU
  bool SendValue(int* texture_unit_count) const;

 private:
  const Uniform *uniform_;  // Holds a reference to its variable
  // Buffer to hold in the memory
  DynamicArray<uint8_t> backend_;
};  // class UniformValue

/***************************************************************
 * TEMPLATES IMPLEMENTATION
 * TODO(Cristian): Move to explicit (non-templated) interface
 ***************************************************************/

template <typename T>
void UniformValue::SetValue(const T& value) {
  assert(sizeof(T) == backend_.Size());
  T *ptr = reinterpret_cast<T*>(backend_.Get());
  *ptr = value;
}

template <typename T>
void UniformValue::SetValues(size_t count, const T* values) {
  assert((count * sizeof(T)) == backend_.Size());
  T *ptr = reinterpret_cast<T*>(backend_.Get());
  for (size_t i = 0; i < count; i++) {
    *ptr++ = *values++;
  }
}

}   // namespace shaders
}   // namespace picasso


#endif  // SRC_SHADERS_UNIFORM_VALUE_H
