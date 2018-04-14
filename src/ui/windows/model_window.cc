/******************************************************************************
 * @file: model_window.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-13
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "ui/windows/windows.h"

#include "registries/model_registry.h"
#include "utils/scope_trigger.h"
#include "utils/string.h"

#include "ui/widgets/transform.h"

namespace picasso {
namespace ui {
namespace windows {

using ::picasso::models::Model;
using ::picasso::registries::ModelRegistry;
using ::picasso::utils::picasso_snprintf;

using ::picasso::ui::widgets::TransformWidget;

void ModelWindow(UiData*, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  SCOPED_TRIGGER(ImGui::Begin("Modeles", &open), ImGui::End());

  auto&& meshes = ModelRegistry::GetElements();
  static int selected_index = -1;
  static ModelRegistry::KeyType selected_key;
  static ModelRegistry::ValueType *selected_model = nullptr;

  {
    SCOPED_TRIGGER(ImGui::BeginChild("Left Pane", {150, 0}, true),
                   ImGui::EndChild());
    int i = 0;
    for (auto&& it = meshes.begin();
         it != meshes.end();
         it++, i++) {
      ModelRegistry::KeyType key = it->key;
      /* Material* material = it->second; */
      Model *model = it->value;
      char label[128];
      picasso_snprintf(label, sizeof(label), "%s", model->name.c_str());
      if (ImGui::Selectable(label, selected_index == i)) {
        selected_index = i;
        selected_key = key;
        selected_model = it->value;
      }
    }
  }

  ImGui::SameLine();

  if (selected_model) {
    SCOPED_TRIGGER(ImGui::BeginChild("Model", {-1, -1}),
                   ImGui::EndChild());
      if (TransformWidget("mesh", selected_model->transform)) {
        selected_model->transform.RecalculateModelMatrix();
      }

      auto& model = selected_model->transform.ModelMatrix;
      float row1[4] = { model[0][0], model[1][0], model[2][0], model[3][0] };
      float row2[4] = { model[0][1], model[1][1], model[2][1], model[3][1] };
      float row3[4] = { model[0][2], model[1][2], model[2][2], model[3][2] };
      float row4[4] = { model[0][3], model[1][3], model[2][3], model[3][3] };

      ImGui::InputFloat4("", row1);
      ImGui::InputFloat4("", row2);
      ImGui::InputFloat4("", row3);
      ImGui::InputFloat4("", row4);
  }
}

}   // namespace windows
}   // namespace ui
}   // namespace picasso

