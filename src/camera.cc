/******************************************************************************
 * @file: camera.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-04
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <cassert>

#include "camera.h"

#define GLM_IMPORT_MATRIX_TRANSFORM
#include "utils/glm.h"

namespace picasso {

namespace {

glm::mat4 GeneratePerspectiveMatrix(const Camera& camera) {
  return glm::perspective(glm::radians(camera.perspective_data.fov),
                          camera.perspective_data.aspect_ratio,
                          camera.perspective_data.distances.x,
                          camera.perspective_data.distances.y);
};

}   // namespace

glm::mat4 Camera::GenerateProjectionMatrix() const {
  assert(proj_type == CameraProjection::PERSPECTIVE);
  return GeneratePerspectiveMatrix(*this);
}

void Camera::ReloadProjectionMatrix() {
  proj_mat_ = GenerateProjectionMatrix();
}

}   // namespace picasso
