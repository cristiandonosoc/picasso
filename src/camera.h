/******************************************************************************
 * @file: camera.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-03
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_CAMERA_H
#define SRC_CAMERA_H

#include "utils/macros.h"
#include "utils/printable_enum.h"
#include "models/transform.h"

BEGIN_IGNORE_WARNINGS();
#include <glm/glm.hpp>
END_IGNORE_WARNINGS();

namespace picasso {

using ::picasso::models::Transform;

PRINTABLE_ENUM(CameraProjection, ORTHOGRAPHIC,
                                 PERSPECTIVE);

class Camera {
 public:
  class OrthoData {
   public:
    glm::vec3 min;
    glm::vec3 max;
  };  // class OrthoData

  class PerspectiveData {
   public:
    float fov = 45.0f;
    float aspect_ratio = 16.0f / 9.0f;
    glm::vec2 distances = { 0.1f, 100.0f };
  };  // class PerspectiveData

 private:
  Transform transform_;
  

};  // class Camera

}   // namespace picasso

#endif  // SRC_CAMERA_H
