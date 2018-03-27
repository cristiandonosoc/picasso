
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


#include <map>

#include "assets/shaders/shader.h"
#include "assets/materials/uniform_value.h"
#include "utils/macros.h"
#include "utils/status_or.h"

namespace picasso {
namespace assets {
namespace materials {

using ::picasso::assets::materials::UniformValue;
using ::picasso::assets::shaders::Shader;

class Material {
 public:
  using ValueMap = std::map<std::string, UniformValue>;

 public:
   DEFINE_PTR_TYPES(Material);

 private:
  Material() {}
  DISABLE_COPY(Material);
  DISABLE_MOVE(Material);

 public:
  const std::string& GetName() const { return name_; }
  const Shader *GetShader() const { return shader_; }

 public:
  template <typename T>
  Status SetValue(const std::string& uniform, const T&);
  template <typename T>
  Status SetValues(const std::string& uniform, size_t count, const T*);

 public:
  ValueMap& Uniforms = uniforms_;
  ValueMap& Attributes = attributes_;

 public:
  void SetShader(Shader *);
  void UnsetShader();

 private:
  std::string name_;
  Shader *shader_ = nullptr;
  ValueMap attributes_;
  ValueMap uniforms_;

 public:
  friend class MaterialRegistry;
};

template <typename T>
Status Material::SetValue(const std::string& uniform, const T& val) {
  auto it = Uniforms.find(uniform);
  if (it == uniforms_.end()) {
    return Status::STATUS_ERROR;
  }
  auto&& value = it->second;
  value.SetValue<T>(val);
  return Status::STATUS_OK;
}

template <typename T>
Status Material::SetValues(const std::string& uniform, size_t count, const T* val) {
  auto it = Uniforms.find(uniform);
  if (it == Uniforms.end()) {
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
