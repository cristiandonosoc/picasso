/******************************************************************************
 * @file: material.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_SHADERS_MATERIAL_H
#define SRC_SHADERS_MATERIAL_H

#include "utils/macros.h"
#include "utils/result.h"
#include "shaders/value.h"

#include <map>

namespace picasso {

using namespace utils;

namespace shaders {

using ValueMap = std::map<std::string, Value>;

class Shader;   // Forward Declaration

class Material {
 public:
   DEFINE_PTR_TYPES(Material);
  
 private:
  static ResultOr<UniquePtr> Create(const std::string& name);

 private:
  Material() {}

 public:
  const std::string& GetName() const { return data_.name_; }
  const Shader *GetShader() const { return data_.shader; }

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
    std::string name_;
    Shader *shader;
    ValueMap attributes;
    ValueMap uniforms;
   public:
    friend class Material;
  } data_;

 public:
  friend class MaterialRegistry;
};

}   // namepsace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_MATERIAL_H
