/******************************************************************************
 * @file: uniform.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-04
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <imgui/imgui.h>

#include "ui/widgets/uniform.h"

namespace picasso {
namespace ui {
namespace widgets {

using ::picasso::assets::shaders::Uniform;
using ::picasso::assets::shaders::UniformWidget;

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

}   // namespace widgets
}   // namespace ui
}   // namespace picasso
