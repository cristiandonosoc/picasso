/******************************************************************************
 * @file: transform.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-24
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: Base class for holding a model transformation
 ******************************************************************************/

#ifndef SRC_MODELS_TRANSFORM_H
#define SRC_MODELS_TRANSFORM_H

#include "utils/macros.h"
#include "utils/printable_enum.h"

BEGIN_IGNORE_WARNINGS();
#include <glm/glm.hpp>
END_IGNORE_WARNINGS();

namespace picasso {
namespace models {

class Transform {
 public:
  Transform() : translation_(0.0f), rotation_(0.0f), scale_(1.0f), m_model_(1.0f) {}

 public:
  const glm::vec3& GetTranslation() const { return translation_; }
  const glm::vec3& GetRotation() const { return rotation_; }
  const glm::vec3& GetScale() const { return scale_; }
  const glm::mat4& GetModelMatrix() const { return m_model_; }

 public:
  void SetTranslation(const glm::vec3& translation);
  void SetRotation(const glm::vec3& rotation);
  void SetScale(const glm::vec3& scale);

 public:
  glm::mat4 CalculateMatrix(bool translate = true,
                            bool rotate = true,
                            bool scale = true) const;
  void RecalculateModelMatrix();

 public:
  const glm::mat4& ModelMatrix = m_model_;

 private:
  glm::vec3 translation_;
  glm::vec3 rotation_;
  glm::vec3 scale_;
  // Final matrix that holds the calculated transform
  glm::mat4 m_model_;
};  // class Transform

}   // namespace
}   // namespace picasso



#endif  // SRC_MODELS_TRANSFORM_H
