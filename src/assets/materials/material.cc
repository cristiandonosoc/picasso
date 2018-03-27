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
void Material::SetShader(Shader *shader) {
  assert(data_.shader == nullptr);
  data_.shader = shader;
  // Create the UniformValues
  for (auto&& it : shader->Uniforms) {
    data_.uniforms[it.first] = UniformValue(&it.second);
  }
  LOG_WARN("%s: Not completely implemented", __FUNCTION__);
}

void Material::UnsetShader() {
  data_.shader = nullptr;
  data_.uniforms.clear();
  data_.attributes.clear();
  LOG_WARN("%s: Not completely implemented", __FUNCTION__);
}

}   // namespace materials 
}   // namespace assets
}   // namespace picasso
