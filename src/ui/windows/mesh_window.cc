/******************************************************************************
 * @file: mesh_window.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-02
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "ui/windows/windows.h"

#include "registries/mesh_registry.h"
#include "utils/scope_trigger.h"
#include "utils/string.h"

#include "ui/widgets/transform.h"

namespace picasso {
namespace ui {
namespace windows {

using ::picasso::registries::MeshRegistry;
using ::picasso::utils::picasso_snprintf;
using ::picasso::assets::Mesh;

using ::picasso::ui::widgets::TransformWidget;

void MeshWindow(UiData*, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  SCOPED_TRIGGER(ImGui::Begin("Meshes", &open), ImGui::End());

  auto&& meshes = MeshRegistry::GetElements();
  static int selected_index = -1;
  static MeshRegistry::KeyType selected_key;
  static MeshRegistry::ValueType *selected_mesh = nullptr;

  {
    SCOPED_TRIGGER(ImGui::BeginChild("Left Pane", {150, 0}, true),
                   ImGui::EndChild());
    int i = 0;
    for (auto&& it = meshes.begin();
         it != meshes.end();
         it++, i++) {
      MeshRegistry::KeyType key = it->key;
      /* Material* material = it->second; */
      Mesh *mesh = it->value;
      char label[128];
      picasso_snprintf(label, sizeof(label), "%s", mesh->GetName().c_str());
      if (ImGui::Selectable(label, selected_index == i)) {
        selected_index = i;
        selected_key = key;
        selected_mesh = it->value;
      }
    }
  }

  ImGui::SameLine();

  if (selected_mesh) {
    SCOPED_TRIGGER(ImGui::BeginChild("Mesh", {-1, -1}),
                   ImGui::EndChild());

  }
}

}   // namespace windows
}   // namespace ui
}   // namespace picasso
