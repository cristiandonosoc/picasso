/******************************************************************************
 * @file: uniform.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_SHADERS_UNIFORM_H
#define SRC_ASSETS_SHADERS_UNIFORM_H

#include <map>
#include <string>
#include <GL/gl3w.h>
#include <cassert>

#include "utils/dynamic_array.h"
#include "utils/macros.h"

#include "utils/printable_enum.h"

namespace picasso {
namespace assets {
namespace shaders {

using ::picasso::utils::DynamicArray;

PRINTABLE_ENUM(UniformWidget,
    NONE,
    COLOR3,
    COLOR4,
    FLOAT,
    FLOAT_VEC2, FLOAT_VEC3, FLOAT_VEC4,
    FLOAT_MAT2, FLOAT_MAT3, FLOAT_MAT4,
    TEXTURE_2D);

extern const std::map<GLenum, UniformWidget>& UNIFORM_WIDGET_MAPPING;
extern const std::map<UniformWidget, size_t>& UNIFORM_WIDGET_SIZES;

class Uniform;
class UniformValue {
 public:
  UniformValue() = default;
  DEFAULT_MOVE(UniformValue);
 
  /**
   * IMPORTANT(Cristian): Will take ownership of the address of the uniform
   *    It's imperative that the uniform don't move
   *    This will be a bitch do debug down the line...
   */
 public:
  UniformValue(const Uniform&);

 public:
  void Copy(const UniformValue&);

 public:
  void SetValueSize();

 public:
  UniformWidget GetWidget() const;

 public:
  template <typename T>
  T *GetValue();
  template <typename T>
  const T *GetValue() const;

 public:
  template <typename T>
  void SetValue(const T& value);
  template <typename T>
  void SetValues(size_t count, const T* values);

 public:
  // Send the value to the GPU
  bool SendValue(int* texture_unit_count) const;

 public:
  int GetLocation() const;
  const Uniform *GetUniform() { return uniform_; }

 private:
  // Buffer to hold in the memory
  DynamicArray<uint8_t> backend_;
  const Uniform *uniform_ = nullptr;
};  // class UniformValue

class Uniform {
 public:
  Uniform() = default;
  DISABLE_COPY(Uniform);
  DEFAULT_MOVE(Uniform);

 public:
  std::string name;
  std::string type_name;
  int location;
  GLenum type;
  size_t count;
  size_t type_size;
  UniformWidget widget = UniformWidget::NONE;
  UniformValue default_value;
};  // class Uniform

/*******************************************************************
 * TEMPLATE IMPLEMENTATION
 *******************************************************************/

template <typename T>
T *UniformValue::GetValue() { 
  assert(backend_.Size() > 0);
  return (T*)backend_.Get();
}
template <typename T>
const T *UniformValue::GetValue() const { 
  assert(backend_.Size() > 0);
  return (T*)backend_.Get();
}

template <typename T>
void UniformValue::SetValue(const T& value) {
  if (backend_.Size() == 0) {
    SetValueSize();
  }
  T *ptr = reinterpret_cast<T*>(backend_.Get());
  *ptr = value;
}

template <typename T>
void UniformValue::SetValues(size_t count, const T* values) {
  if (backend_.Size() == 0) {
    SetValueSize();
  }
  T *ptr = reinterpret_cast<T*>(backend_.Get());
  for (size_t i = 0; i < count; i++) {
    *ptr++ = *values++;
  }
}

}   // namespace shaders
}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_SHADERS_UNIFORM_H
