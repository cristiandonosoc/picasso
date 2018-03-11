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

namespace picasso {
namespace shaders {

class Value {
 public:
  Value() = default;
  Value(const Variable *variable);

 public:
  const Variable *GetVariable() const { return variable_; }

 public:
  void *GetValue() { return backend_.get(); }

  template<typename T>
  void SetValue(const T&);

  template<typename T>
  void SetValues(size_t count, const T*);


 private:
  const Variable *variable_;  // Holds a reference to its variable
  // Buffer to hold in the memory
  std::unique_ptr<uint8_t[]> backend_;
  size_t backend_size_ = 0;
};

/***************************************************************
 * TEMPLATES IMPLEMENTATION
 ***************************************************************/

template <typename T>
void Value::SetValue(const T& value) {
  assert(sizeof(T) == backend_size_);
  T *ptr = reinterpret_cast<T*>(backend_.get());
  *ptr = value;
}

template <typename T>
void Value::SetValues(size_t count, const T* values) {
  assert((count * sizeof(T)) == backend_size_);
  T *ptr = reinterpret_cast<T*>(backend_.get());
  for (size_t i = 0; i < count; i++) {
    *ptr++ = *values++;
  }
}

}   // namespace shaders
}   // namespace picasso



#endif  // SRC_SHADERS_VALUE_H
