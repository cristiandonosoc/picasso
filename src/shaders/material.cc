/******************************************************************************
 * @file: material.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "assets/shaders/shader.h"
#include "shaders/material.h"

namespace picasso {
namespace shaders {

/**
 * CONSTRUCTORS
 **/
StatusOr<Material::UniquePtr> Material::Create(const std::string& name) {
  UniquePtr material(new Material());   // Private Constructor
  material->data_.name = name;
  return material;
}

Material::Material(Material&& other) noexcept {
  *this = std::move(other);
}

Material& Material::operator=(Material&& other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
  }
  return *this;
}

/**
 * SHADER INTERFACE
 **/
void Material::SetShader(Shader *shader) {
  if (data_.shader == shader) { return; }
  UnlinkShader();
  LinkShader(shader);
}

void Material::UnsetShader() {
  UnlinkShader();
}

void Material::LinkShader(Shader *shader) {
  if (shader) {
    // Copy over uniforms
    for (auto&& it : shader->Uniforms) {
      data_.uniforms[it.first] = UniformValue(&it.second);
    }
  }
  data_.shader = shader;
}

void Material::UnlinkShader() {
  if (data_.shader) {
    data_.attributes.clear();
    data_.uniforms.clear();
  }
  data_.shader = nullptr;
}

}   // namespace shaders
}   // namespace picasso
