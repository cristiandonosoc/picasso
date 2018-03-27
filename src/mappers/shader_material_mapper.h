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
using ::picasso::assets::materials::MaterialRegistry;

class ShaderMaterialMapper : public Mapper<ShaderMaterialMapper, ShaderRegistry, MaterialRegistry> {
 public:
  static bool AddCallback(const FromKeyType&, const ToKeyType&) {
    LOG_DEBUG("Called AddCallback: %s", __FUNCTION__);
    return true;
  }

  static bool RemoveCallback(const FromKeyType&, const ToKeyType&) {
    LOG_DEBUG("Called RemoveCallback: %s", __FUNCTION__);
    return true;
  }



};  // class ShaderMaterialMapper

}   // namespace mappers
}   // namespace picasso

#endif  // SRC_MAPPERS_SHADER_MATERIAL_MAPPER_H
