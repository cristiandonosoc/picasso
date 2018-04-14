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

glm::vec3 Camera::GetLookDirection() const {
  /* glm::vec3 direction(0); */
  /* float pitch = transform.GetRotation().x; */
  /* float yaw = transform.GetRotation().y; */
  /* direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw)); */
  /* direction.y = sin(glm::radians(pitch)); */
  /* direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw)); */
  /* return direction; */
  return transform.CalculateMatrix(false, true, false) * glm::vec4(0, 0, 1, 0);
}

glm::mat4 Camera::GenerateViewMatrix() const {
  const auto& position = transform.GetTranslation();
  const auto& direction = GetLookDirection();
  return glm::lookAt(position, 
                     position + direction,
                     {0, 1, 0});
}

glm::mat4 Camera::GenerateProjectionMatrix() const {
  assert(proj_type == CameraProjection::PERSPECTIVE);
  return GeneratePerspectiveMatrix(*this);
}

void Camera::ReloadProjectionMatrix() {
  proj_mat_ = GenerateProjectionMatrix();
}

void Camera::ReloadViewMatrix() {
  view_mat_ = GenerateViewMatrix();
}

}   // namespace picasso
