/******************************************************************************
 * @file: model.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-13
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "models/model.h"
#include "logging/log.h"

namespace picasso {
namespace models {

void Model::Render(const Camera& camera) {
  if (!mesh) {
    LOG_WARN("Model %s: Called render without Mesh", name.c_str());
    return;
  }

  if (!material) {
    LOG_WARN("Model %s: Called render without Material", name.c_str());
    return;
  }

  // TODO(Cristian): Pass in the transform
  mesh->Render(transform, camera, *material);
}


}   // namespace models
}   // namespace picasso

