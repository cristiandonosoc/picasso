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

#include "shaders/variable.h"
#include "utils/macros.h"
#include "utils/result.h"

#include <map>

namespace picasso {

using namespace utils;

namespace shaders {

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
    std::map<std::string, Variable> attributes;
    std::map<std::string, Variable> uniforms;
   public:
    friend class Material;
  } data_;

 public:
  decltype(data_.attributes)& Attributes = data_.attributes;
  decltype(data_.uniforms)& Uniforms = data_.uniforms;

 public:
  friend class MaterialRegistry;
};

}   // namepsace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_MATERIAL_H
