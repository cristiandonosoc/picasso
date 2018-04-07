/******************************************************************************
 * @file: transform.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-04
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <imgui/imgui.h>

#define GLM_IMPORT_TYPE_PTR
#include "utils/glm.h"
#include "ui/widgets/transform.h"

namespace picasso {
namespace ui {
namespace widgets {

bool TransformWidget(const char *id, Transform& transform) {
  ImGui::PushID(id);
  bool change = false;
  change |= ImGui::InputFloat3("TRANSFORM",
                     (float*)glm::value_ptr(transform.GetTranslation()));
  change |= ImGui::InputFloat3("ROTATION",
                     (float*)glm::value_ptr(transform.GetRotation()));
  change |= ImGui::InputFloat3("SCALE",
                     (float*)glm::value_ptr(transform.GetScale()));

  ImGui::PopID();
  if (change) {
    transform.RecalculateModelMatrix();
  }
  return change;
}

}   // namespace widgets
}   // namespace ui
}   // namespace picasso

