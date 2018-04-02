/******************************************************************************
 * @file: shader_window.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-01
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UI_SHADER_WINDOW_H
#define SRC_UI_SHADER_WINDOW_H

#include "ui/ui.h"
#include "ui/widgets.h"
#include "utils/scope_trigger.h"
#include "assets/shaders/shader_registry.h"
#include "utils/snprintf.h"

#include "logging/log.h"

#include <algorithm>

#undef min
#undef max

namespace picasso {
namespace ui {

using namespace ::picasso::ui::widgets;
using ::picasso::utils::picasso_snprintf;

using namespace ::picasso::assets::shaders;

namespace {

void GenerateWidget(Uniform& uniform) {
  switch (uniform.widget) {
    case UniformWidget::FLOAT: {
      ImGui::InputFloat(uniform.name.c_str(), uniform.default_value.GetValue<float>());
      break;
    }
    case UniformWidget::FLOAT_VEC2: {
      ImGui::InputFloat2(uniform.name.c_str(), uniform.default_value.GetValue<float>());
      break;
    }
    case UniformWidget::FLOAT_VEC3: {
      ImGui::InputFloat3(uniform.name.c_str(), uniform.default_value.GetValue<float>());
      break;
    }
    case UniformWidget::FLOAT_VEC4: {
      ImGui::InputFloat4(uniform.name.c_str(), uniform.default_value.GetValue<float>());
      break;
    }
    case UniformWidget::COLOR3: {
      auto flags = ImGuiColorEditFlags_NoInputs;
      ImGui::ColorEdit3(uniform.name.c_str(), uniform.default_value.GetValue<float>(), flags);
      break;
    }
    case UniformWidget::COLOR4: {
      auto flags = ImGuiColorEditFlags_NoInputs;
      ImGui::ColorEdit4(uniform.name.c_str(), uniform.default_value.GetValue<float>(), flags);
      break;
    }
    default:
      break;
  }
}

void ShowUniformWidget(Uniform& uniform) {
  ImGui::Text("%s", uniform.name.c_str());
  ImGui::BulletText("TYPE: %s", uniform.type_name.c_str());
  ImGui::BulletText("WIDGET:");
  ImGui::SameLine();
  UniformWidget widget = EnumPopup(uniform.name.c_str(), uniform.widget);
  size_t widget_size = UNIFORM_WIDGET_SIZES.find(widget)->second;
  if (widget_size != uniform.type_size) {
    LOG_DEBUG("Widget \"%s\" (%zu) doesn't have the correct size for uniform type \"%s\" (%zu)",
              UniformWidget::ToString(widget).c_str(), widget_size,
              uniform.type_name.c_str(), uniform.type_size);
  } else {
    uniform.widget = widget;
  }

  GenerateWidget(uniform);

  ImGui::Separator();
}

}   // namespace

void ShaderWindow(UiData *, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  SCOPED_TRIGGER(ImGui::Begin("Shaders", &open), ImGui::End());

  static int selected_shader = -1;
  auto&& shaders = ShaderRegistry::GetShaders();

  {
    SCOPED_TRIGGER(ImGui::BeginChild("Left Pane", {150, 0}, true),
                   ImGui::EndChild());
    int i = 0;
    for (auto&& it = shaders.begin();
         it != shaders.end();
         it++, i++) {
      auto&& shader = *it;
      char label[128];
      picasso_snprintf(label, sizeof(label), "Shader: %s", shader->GetName().c_str());
      if (ImGui::Selectable(label, selected_shader == i)) {
        selected_shader = i;
      }
    }
  }

  ImGui::SameLine();

  Shader *shader = nullptr;
  if (selected_shader >= 0) {
    shader = shaders[selected_shader];
  } else {
    return;
  }

  float text_height = (ImGui::GetContentRegionAvail().y - 2 * ImGui::GetFontSize()) / 2;

  SCOPED_TRIGGER(ImGui::BeginChild("Vertex Shader", {-1, -1}), ImGui::EndChild());

  ImGui::Text("Uniforms");
  for (auto& it : shader->Uniforms) {
    ShowUniformWidget(it.second);
  }
  ImGui::Separator();

  // VERTEX
  ImGui::Text("Vertex Shader");
  char vertex_buf[4096] = "No source available";
  size_t len = 0;
  if (shader) {
    const std::string& vertex_src = shader->GetVertexSource();
    len = std::min(vertex_src.size(), sizeof(vertex_buf));
    memcpy(vertex_buf, vertex_src.c_str(), len);
  }
  ImGui::InputTextMultiline("##vs", vertex_buf, len, {-1, text_height},
                            ImGuiInputTextFlags_AllowTabInput);

  ImGui::Separator();

  // FRAGMENT

  char frag_buf[4096] = "No source available";
  len = 0;

  if (shader) {
    const std::string& fragment_src = shader->GetFragmentSource();
    len = std::min(fragment_src.size(), sizeof(frag_buf));
    memcpy(frag_buf, fragment_src.c_str(), len);
  }

  ImGui::Text("Fragment Shader");
  ImGui::InputTextMultiline("##fs", frag_buf, len, {-1, -1},
                          ImGuiInputTextFlags_AllowTabInput);
}



}   // namespace ui
}   // namespace picasso

#endif  // SRC_UI_SHADER_WINDOW_H
