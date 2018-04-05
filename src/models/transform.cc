/******************************************************************************
 * @file: transform.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-24
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "models/transform.h"

BEGIN_IGNORE_WARNINGS();
#include <glm/gtc/matrix_transform.hpp>
END_IGNORE_WARNINGS();


namespace picasso {
namespace models {

void Transform::SetTranslation(const glm::vec3& translation) {
    translation_ = translation;
    RecalculateModelMatrix();
}

void Transform::SetRotation(const glm::vec3& rotation) {
  rotation_ = rotation;
  RecalculateModelMatrix();
}

void Transform::SetScale(const glm::vec3& scale) {
  scale_ = scale;
  RecalculateModelMatrix();
}

void Transform::RecalculateModelMatrix() {
  m_model_ = CalculateMatrix();
}

glm::mat4 Transform::CalculateMatrix(bool translate,
                                     bool rotate,
                                     bool scale) const {
  glm::mat4 mat = glm::mat4(1.0f);
  // We rotate according each axis
  if (rotate) {
    mat = glm::rotate(mat, glm::radians(rotation_.x), glm::vec3(1, 0, 0));
    mat = glm::rotate(mat, glm::radians(rotation_.y), glm::vec3(0, 1, 0));
    mat = glm::rotate(mat, glm::radians(rotation_.z), glm::vec3(0, 0, -1));
  }

  if (scale) {
    mat = glm::scale(mat, scale_);
  }

  // Translate
  if (translate) {
    mat = glm::translate(mat, translation_);
  }
  return mat;
}

}   // namespace models
}   // namespace picasso
