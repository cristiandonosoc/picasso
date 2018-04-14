/******************************************************************************
 * @file: model.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-13
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_MODELS_MODEL_H
#define SRC_MODELS_MODEL_H

#include <string>

#include "models/transform.h"
#include "assets/mesh.h"
#include "assets/materials/material.h"
#include "camera.h"

namespace picasso {
namespace models {

using ::picasso::Camera;
using ::picasso::assets::Mesh;
using ::picasso::assets::Material;

class Model {
 public:
  void Render(const Camera&);

 public:
  std::string name;
  Transform transform;
  Mesh *mesh;
  Material *material;
};  // class Model

}   // namespace models
}   // namespace picasso

#endif  // SRC_MODELS_MODEL_H
