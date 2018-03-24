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
  m_model_ = glm::mat4(1.0f);  
  m_model_ = glm::scale(m_model_, scale_);

  // We rotate according each axis
  m_model_ = glm::rotate(m_model_, rotation_.x, glm::vec3(1, 0, 0));
  m_model_ = glm::rotate(m_model_, rotation_.y, glm::vec3(0, 1, 0));
  m_model_ = glm::rotate(m_model_, rotation_.z, glm::vec3(0, 0, 1));

  // Translate
  m_model_ = glm::translate(m_model_, translation_);
}

}   // namespace models
}   // namespace picasso
