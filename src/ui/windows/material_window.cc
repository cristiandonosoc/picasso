/******************************************************************************
 * @file: material_window.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-02
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <algorithm>

#include "ui/windows/windows.h"

#include "ui/widgets/popup.h"
#include "ui/widgets/uniform.h"

#include "utils/scope_trigger.h"
#include "assets/shaders/shader_registry.h"
#include "assets/materials/material_registry.h"
#include "utils/snprintf.h"
#include "utils/string.h"
#include "mappers/shader_material_mapper.h"

#include "logging/log.h"


namespace picasso {
namespace ui {
namespace windows {

using namespace ::picasso::ui::widgets;
using namespace ::picasso::assets::materials;
using namespace ::picasso::assets::shaders;
using namespace ::picasso::mappers;
using ::picasso::utils::picasso_snprintf;
using ::picasso::utils::FormattedString;

namespace {

void MaterialUniformWidget(UniformValue& value) {
  ImGui::Text("%s", value.GetUniform()->name.c_str());
  ImGui::BulletText("TYPE: %s", value.GetUniform()->type_name.c_str());
  GenerateUniformWidget(value);
}

}   // namespace


void MaterialWindow(UiData *, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;

  SCOPED_TRIGGER(ImGui::Begin("Materials", &open), ImGui::End());

  auto&& materials = MaterialRegistry::GetElements();
  static int selected_material = -1;
  static MaterialRegistry::KeyType selected_key;

  {
    SCOPED_TRIGGER(ImGui::BeginChild("Left Pane", {150, 0}, true),
                   ImGui::EndChild());
    int i = 0;
    for (auto&& it = materials.begin();
         it != materials.end();
         it++, i++) {
      MaterialRegistry::KeyType key = it->key;
      Material *material = it->value;
      /* Material* material = it->second; */
      char label[128];
      picasso_snprintf(label, sizeof(label), "%s", material->GetName().c_str());
      if (ImGui::Selectable(label, selected_material == i)) {
        selected_material = i;
        selected_key = key;
      }
    }
  }

  ImGui::SameLine();

  Material *material= nullptr;
  if (selected_material >= 0) {
    material = materials[selected_material].value;
  }

  if (material) {
    SCOPED_TRIGGER(ImGui::BeginChild("Material", {-1, -1}),
                   ImGui::EndChild());

    int current_index = -1;
    std::vector<std::string> shader_names;
    std::vector<ShaderRegistry::KeyType> shader_keys;
    const auto& shaders = ShaderRegistry::GetElements();
    shader_names.reserve(shaders.size());
    shader_keys.reserve(shaders.size());
    int index = 0;
    for (const auto& it : shaders) {
      shader_keys.push_back(it.key);

      const Shader *shader = it.value;
      shader_names.push_back(shader->GetName().c_str());
      if (shader == material->GetShader()) {
        current_index = index;
      }
      index++;
    }

    int prev_index = current_index;
    current_index = GetPopupIndex("shaders", current_index, shader_names);

    if (prev_index != current_index) {
      if (prev_index >= 0) {
        const auto& prev_shader_key = shader_keys[prev_index];
        auto res = ShaderMaterialMapper::RemoveMapping(prev_shader_key, selected_key);
        LOG_NON_OK_STATUS(res);
      }
      if (current_index >= 0) {
        const auto& shader_key = shader_keys[current_index];
        auto res = ShaderMaterialMapper::AddMapping(shader_key, selected_key);
        LOG_NON_OK_STATUS(res);
      }
    }

    ImGui::Text("Uniforms");
    ImGui::Separator();

    for (auto& it : material->Uniforms) {
      MaterialUniformWidget(it.second);
      ImGui::Separator();
    }
  }
}

}   // namespace windows
}   // namespace ui
}   // namespace picasso
