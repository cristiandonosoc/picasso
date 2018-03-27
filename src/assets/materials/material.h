
/******************************************************************************
 * @file: material.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-26
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_MATERIALS_MATERIAL_H
#define SRC_ASSETS_MATERIALS_MATERIAL_H

#include "utils/macros.h"
#include "utils/status_or.h"

#include "assets/shaders/shader.h"
#include "assets/materials/uniform_value.h"

#include <map>

namespace picasso {
namespace assets {
namespace materials {

using ::picasso::assets::materials::UniformValue;

using ValueMap = std::map<std::string, UniformValue>;
using Shader = ::picasso::assets::shaders::Shader;

class Material {
 public:
   DEFINE_PTR_TYPES(Material);

 private:
  static StatusOr<UniquePtr> Create(const std::string& name);

 private:
  Material() {}

 public:
  const std::string& GetName() const { return data_.name; }
  const Shader *GetShader() const { return data_.shader; }

 public:
  template <typename T>
  Status SetValue(const std::string& uniform, const T&);
  template <typename T>
  Status SetValues(const std::string& uniform, size_t count, const T*);


 public:
  ValueMap& Uniforms = data_.uniforms;
  ValueMap& Attributes = data_.attributes;

 public:
  void SetShader(Shader *);
  void UnsetShader();

 private:
  void LinkShader(Shader *);
  void UnlinkShader();

 public:
  DISABLE_COPY(Material);
  Material(Material&&) noexcept;
  Material& operator=(Material&&) noexcept;

 private:
  class Data {
   public:
    std::string name;
    Shader *shader = nullptr;
    ValueMap attributes;
    ValueMap uniforms;
   public:
    friend class Material;
  } data_;

 public:
  friend class MaterialRegistry;
};

template <typename T>
Status Material::SetValue(const std::string& uniform, const T& val) {
  auto it = data_.uniforms.find(uniform);
  if (it == data_.uniforms.end()) {
    return Status::STATUS_ERROR;
  }
  auto&& value = it->second;
  value.SetValue<T>(val);
  return Status::STATUS_OK;
}

template <typename T>
Status Material::SetValues(const std::string& uniform, size_t count, const T* val) {
  auto it = data_.uniforms.find(uniform);
  if (it == data_.uniforms.end()) {
    LOG_ERROR("Could not find uniform \"%s\"", uniform.c_str());
    return Status::STATUS_ERROR;
  }
  auto&& value = it->second;
  value.SetValues<T>(count, val);
  return Status::STATUS_OK;
}

}   // namespace materials
}   // namepsace assets
}   // namespace picasso

#endif  // SRC_ASSETS_MATERIALS_MATERIAL_H
