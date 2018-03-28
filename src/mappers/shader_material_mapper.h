/******************************************************************************
 * @file: shader_material_mapper.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-26
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_MAPPERS_SHADER_MATERIAL_MAPPER_H
#define SRC_MAPPERS_SHADER_MATERIAL_MAPPER_H

#include "mappers/mapper.h"
#include "assets/shaders/shader_registry.h"
#include "assets/materials/material_registry.h"

#include "logging/log.h"

namespace picasso {
namespace mappers {

using ::picasso::assets::shaders::ShaderRegistry;
using ::picasso::assets::shaders::Shader;
using ::picasso::assets::materials::MaterialRegistry;
using ::picasso::assets::materials::Material;

class ShaderMaterialMapper : public Mapper<ShaderMaterialMapper, ShaderRegistry, MaterialRegistry> {
 public:
  static const std::string& TypeName() {
    static std::string type_name_("ShaderMaterialMapper");
    return type_name_;
  }

 public:
  static Status AddCallback(const FromKeyType& shader_key, const ToKeyType& material_key) {
    auto shader_res = ShaderRegistry::Get(shader_key);
    if (!shader_res.Ok()) { 
      return shader_res; 
    }

    auto material_res = MaterialRegistry::Get(material_key);
    if (!material_res.Ok()) { 
      return material_res; 
    }

    Material *material = material_res.ConsumeOrDie();
    Shader *shader = shader_res.ConsumeOrDie();
    return material->SetShader(shader);
  }

  static Status RemoveCallback(const FromKeyType&, const ToKeyType&) {
    LOG_DEBUG("Called RemoveCallback: %s", __FUNCTION__);
    return Status::STATUS_OK;
  }



};  // class ShaderMaterialMapper

}   // namespace mappers
}   // namespace picasso

#endif  // SRC_MAPPERS_SHADER_MATERIAL_MAPPER_H
