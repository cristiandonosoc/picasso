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

#include "utils/glm.h"

namespace picasso {

using ::picasso::models::Transform;

PRINTABLE_ENUM(CameraProjection, ORTHOGRAPHIC,
                                 PERSPECTIVE);

class Camera {
 public:
  class OrthographicData {
   public:
    glm::vec3 min;
    glm::vec3 max;
  };  // class OrthographicData

  class PerspectiveData {
   public:
    float fov = 45.0f;
    float aspect_ratio = 16.0f / 9.0f;
    glm::vec2 distances = { 0.1f, 100.0f };
  };  // class PerspectiveData

 public:
  Camera() {
    ReloadProjectionMatrix();
  }

 public:
  glm::mat4 GenerateProjectionMatrix() const;

 public:
  void ReloadProjectionMatrix();

 public:
  const glm::mat4& Projection() const { return proj_mat_; }

  // Properties
  // IMPORTANT(Cristian): Any change to any of this fields must
  //                      be followed by a call to ReloadProjMatrix()!
  // TODO(Cristian): See a good way to enforce this
 public:
  Transform transform;
  OrthographicData ortho_data;
  PerspectiveData perspective_data;
  CameraProjection proj_type = CameraProjection::PERSPECTIVE;

 private:
  // Must be kept up to date
  glm::mat4 proj_mat_;
};  // class Camera

}   // namespace picasso

#endif  // SRC_CAMERA_H
