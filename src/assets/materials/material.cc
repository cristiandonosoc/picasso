/******************************************************************************
 * @file: material.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "assets/materials/material.h"
#include "assets/shaders/shader.h"

namespace picasso {
namespace assets {
namespace materials {

/**
 * SHADER INTERFACE
 **/
Status Material::SetShader(Shader *shader) {
  if (shader_ != nullptr) {
    return { Status::STATUS_ERROR,
             "Material \"%s\": already has a shader (\"%s\") associated with it",
             name_.c_str(), shader->GetName().c_str() };
  }
  shader_ = shader;
  // Create the UniformValues
  for (auto&& it : shader->Uniforms) {
    uniforms_[it.first] = UniformValue(&it.second);
  }
  return Status::STATUS_OK;
}

Status Material::UnsetShader() {
  if (shader_ == nullptr) {
    LOG_WARN("Material \"%s\": de-registering from a unset material.",
             name_.c_str());
  }
  shader_ = nullptr;
  uniforms_.clear();
  attributes_.clear();
  return Status::STATUS_OK;
}

}   // namespace materials
}   // namespace assets
}   // namespace picasso
