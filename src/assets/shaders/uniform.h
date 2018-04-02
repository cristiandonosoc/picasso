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

#include "utils/dynamic_array.h"

#include "utils/printable_enum.h"

namespace picasso {
namespace assets {
namespace shaders {

using ::picasso::utils::DynamicArray;

PRINTABLE_ENUM(UniformWidget,
    NONE,
    COLOR,
    FLOAT,
    FLOAT_VEC2, FLOAT_VEC3, FLOAT_VEC4,
    FLOAT_MAT2, FLOAT_MAT3, FLOAT_MAT4,
    TEXTURE_2D);

extern const std::map<GLenum, UniformWidget>& UNIFORM_WIDGET_MAPPING;

class Uniform;  // Forward-declaration
class UniformValue {
 public:
  UniformValue() = default;

 public:
  void SetValueSize(const Uniform&);

 public:
  // TODO(Cristian): Move to explicit interface
  template <typename T>
  T *GetValue() { return (T*)backend_.Get(); }
  template <typename T>
  const T *GetValue() const { return (T*)backend_.Get(); }

 public:
  template <typename T>
  void SetValue(const T& value) {
    assert(sizeof(T) == backend_.Size());
    T *ptr = reinterpret_cast<T*>(backend_.Get());
    *ptr = value;
  }

  template <typename T>
  void SetValues(size_t count, const T* values) {
    assert((count * sizeof(T)) == backend_.Size());
    T *ptr = reinterpret_cast<T*>(backend_.Get());
    for (size_t i = 0; i < count; i++) {
      *ptr++ = *values++;
    }
  }

 public:
  // Send the value to the GPU
  bool SendValue(int* texture_unit_count) const;

 private:
  // Buffer to hold in the memory
  DynamicArray<uint8_t> backend_;
};  // class UniformValue

class Uniform {
 public:
  Uniform() = default;

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

}   // namespace shaders
}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_SHADERS_UNIFORM_H
