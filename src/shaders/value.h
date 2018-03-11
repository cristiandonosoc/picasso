/******************************************************************************
 * @file: value.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-10
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: Represents a particular value of a shader::Variable
 *               These are the values that are sent to the attributes/uniforms
 ******************************************************************************/

#ifndef SRC_SHADERS_VALUE_H
#define SRC_SHADERS_VALUE_H

#include <cstdint>
#include <memory>

#include "shaders/variable.h"
#include "utils/dynamic_array.h"

using ::picasso::utils::DynamicArray;

namespace picasso {
namespace shaders {



class Value {
 public:
  Value() = default;
  Value(const Variable *variable);

 public:
  const Variable *GetVariable() const { return variable_; }

 public:
  template <typename T>
  T *GetValue() { return (T*)backend_.Get(); }
  template <typename T>
  const T *GetValue() const { return (T*)backend_.Get(); }

 public:
  template <typename T>
  void SetValue(const T&);
  template <typename T>
  void SetValues(size_t count, const T*);

 public:
  // Send the value to the GPU
  bool SendValue() const;

 private:
  const Variable *variable_;  // Holds a reference to its variable
  // Buffer to hold in the memory
  DynamicArray<uint8_t> backend_;
};

/***************************************************************
 * TEMPLATES IMPLEMENTATION
 ***************************************************************/

template <typename T>
void Value::SetValue(const T& value) {
  assert(sizeof(T) == backend_.Size());
  T *ptr = reinterpret_cast<T*>(backend_.Get());
  *ptr = value;
}

template <typename T>
void Value::SetValues(size_t count, const T* values) {
  assert((count * sizeof(T)) == backend_.Size());
  T *ptr = reinterpret_cast<T*>(backend_.Get());
  for (size_t i = 0; i < count; i++) {
    *ptr++ = *values++;
  }
}

}   // namespace shaders
}   // namespace picasso



#endif  // SRC_SHADERS_VALUE_H
