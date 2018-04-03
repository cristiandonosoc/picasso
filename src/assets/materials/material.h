
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
#include "assets/shaders/uniform.h"
#include "utils/macros.h"
#include "utils/status.h"

namespace picasso {
namespace assets {
namespace materials {

using ::picasso::assets::shaders::Shader;
using ::picasso::assets::shaders::UniformValue;
using ::picasso::utils::Status;

class Material {
 public:
  using ValueMap = std::map<std::string, UniformValue>;

 public:
   DEFINE_PTR_TYPES(Material);

 public:
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
  template<typename T>
  Status SetMetadata(const std::string& unfiform, const std::string& metadata);

 public:
  ValueMap& Uniforms = uniforms_;
  ValueMap& Attributes = attributes_;

 public:
  Status SetShader(Shader *);
  Status UnsetShader();

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
    return FILENO_STATUS(Status::STATUS_ERROR, 
                         "Could not find uniform \"%s\"",
                         uniform.c_str());
  }
  auto&& value = it->second;
  value.SetValue<T>(val);
  return Status::STATUS_OK;
}

template <typename T>
Status Material::SetValues(const std::string& uniform, size_t count, const T* val) {
  auto it = Uniforms.find(uniform);
  if (it == Uniforms.end()) {
    return FILENO_STATUS(Status::STATUS_ERROR, 
                         "Could not find uniform \"%s\"",
                         uniform.c_str());
  }
  auto&& value = it->second;
  value.SetValues<T>(count, val);
  return Status::STATUS_OK;
}

template <typename T>
Status Material::SetMetadata(const std::string& uniform, const std::string& metadata) {
  auto it = Uniforms.find(uniform);
  if (it == Uniforms.end()) {
    return FILENO_STATUS(Status::STATUS_ERROR, 
                         "Could not find uniform \"%s\"",
                         uniform.c_str());
  }
  auto&& value = it->second;
  value.metadata = metadata;
  return Status::STATUS_OK;
}

}   // namespace materials
}   // namepsace assets
}   // namespace picasso

#endif  // SRC_ASSETS_MATERIALS_MATERIAL_H
