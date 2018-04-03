/******************************************************************************
 * @file: widgets.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-01
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "ui/widgets.h"
#include <imgui/imgui.h>
#include "assets/shaders/uniform.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace picasso {
namespace ui {
namespace widgets {

using namespace ::picasso::assets::shaders;

void GenerateUniformWidget(UniformValue& value) {
  const Uniform *uniform = value.GetUniform();
  switch (uniform->widget) {
    case UniformWidget::FLOAT: {
      ImGui::InputFloat(uniform->name.c_str(), value.GetValue<float>());
      break;
    }
    case UniformWidget::FLOAT_VEC2: {
      ImGui::InputFloat2(uniform->name.c_str(), value.GetValue<float>());
      break;
    }
    case UniformWidget::FLOAT_VEC3: {
      ImGui::InputFloat3(uniform->name.c_str(), value.GetValue<float>());
      break;
    }
    case UniformWidget::FLOAT_VEC4: {
      ImGui::InputFloat4(uniform->name.c_str(), value.GetValue<float>());
      break;
    }
    case UniformWidget::COLOR3: {
      auto flags = ImGuiColorEditFlags_NoInputs;
      ImGui::ColorEdit3(uniform->name.c_str(), value.GetValue<float>(), flags);
      break;
    }
    case UniformWidget::COLOR4: {
      auto flags = ImGuiColorEditFlags_NoInputs;
      ImGui::ColorEdit4(uniform->name.c_str(), value.GetValue<float>(), flags);
      break;
    }
    case UniformWidget::TEXTURE_2D: {
    }
    default:
      break;
  }
}

bool TransformWidget(Transform& transform) {
  bool change = false; 
  change |= ImGui::InputFloat3("TRANSFORM", 
                     (float*)glm::value_ptr(transform.GetTranslation()));
  change |= ImGui::InputFloat3("ROTATION", 
                     (float*)glm::value_ptr(transform.GetRotation()));
  change |= ImGui::InputFloat3("SCALE",
                     (float*)glm::value_ptr(transform.GetScale()));
  return change;
}

}   // namespace widgets
}   // namespace ui
}   // namespace picasso
