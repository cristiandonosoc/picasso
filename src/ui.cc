#include "ui.h"
#include "shaders/shader_registry.h"
#include "shaders/material_registry.h"

#include <cstring>

namespace picasso {

using ::picasso::shaders::Shader;
using ::picasso::shaders::Material;
using ::picasso::shaders::Variable;
using ::picasso::shaders::Value;

void ImGuiExample(const ImVec4& clear_color, bool show_demo_window,
                  bool show_another_window) {
  // 1. Show a simple window.
  // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
  {
      static float f = 0.0f;
      static int counter = 0;
      ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
      ImGui::Checkbox("Another Window", &show_another_window);

      if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
          counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

  // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
  if (show_another_window)
  {
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
          show_another_window = false;
      ImGui::End();
  }

  // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
  if (show_demo_window)
  {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
      ImGui::ShowDemoWindow(&show_demo_window);
  }
}

void ShaderWindow() {
  ImGuiIO& io = ImGui::GetIO();

  ImGui::SetNextWindowSize({500, io.DisplaySize.y}, ImGuiCond_Once);
  ImGui::Begin("Shaders", nullptr);

  ImGui::BeginChild("Left Pane", {150, 0}, true);
  auto&& shaders = shaders::ShaderRegistry::GetShaders();
  static int selected_shader = -1;
  int i = 0;
  for (auto&& it = shaders.begin();
       it != shaders.end();
       it++, i++) {
    auto&& shader = *it;
    char label[128];
    sprintf(label, "Shader: %s", shader->GetName().c_str());
    if (ImGui::Selectable(label, selected_shader == i)) {
      selected_shader = i;
    }
  }

  ImGui::EndChild();
  ImGui::SameLine();

  Shader *shader = nullptr;
  if (selected_shader >= 0) {
    shader = shaders[selected_shader];
  }

  float text_height = (ImGui::GetContentRegionAvail().y - 2 * ImGui::GetFontSize()) / 2;
  /* ImGui::BeginGroup(); */
    /* ImGui::BeginChild("Vertex Shader", {-1, -window_height / 2}); */
    ImGui::BeginChild("Vertex Shader", {-1, -1});
      ImGui::Text("Vertex Shader");
      ImGui::Separator();
      {

        char buf[4096] = "No source available";
        size_t len = 0;

        if (shader) {
          const std::string& vertex_src = shader->GetVertexSource();
          len = min(vertex_src.size(), sizeof(buf));
          memcpy(buf, vertex_src.c_str(), len);
        }

        ImGui::InputTextMultiline("##vs", buf, len, {-1, text_height}, 
                                  ImGuiInputTextFlags_AllowTabInput);
      }
      {
        char buf[4096] = "No source available";
        size_t len = 0;

        if (shader) {
          const std::string& fragment_src = shader->GetFragmentSource();
          len = min(fragment_src.size(), sizeof(buf));
          memcpy(buf, fragment_src.c_str(), len);
        }

        ImGui::Text("Fragment Shader");
        ImGui::Separator();
        ImGui::InputTextMultiline("##fs", buf, len, {-1, -1}, 
                                ImGuiInputTextFlags_AllowTabInput);
      }
    ImGui::EndChild();
  /* ImGui::EndGroup(); */

  ImGui::End();

}

void MaterialWindow() {
  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowSize({500, io.DisplaySize.y}, ImGuiCond_Once);
  ImGui::Begin("Materials", nullptr);

  ImGui::BeginChild("Left Pane", {150, 0}, true);
  auto&& materials = shaders::MaterialRegistry::GetMaterials();
  static int selected_material = -1;
  int i = 0;
  for (auto&& it = materials.begin();
       it != materials.end();
       it++, i++) {
    Material* material = *it;
    char label[128];
    sprintf(label, "%s", material->GetName().c_str());
    if (ImGui::Selectable(label, selected_material == i)) {
      selected_material = i;
    }
  }

  ImGui::EndChild();
  ImGui::SameLine();

  Material *material= nullptr;
  if (selected_material >= 0) {
    material = materials[selected_material];
  }

  /* float height = ImGui::GetContentRegionAvail().y; */

  if (material) {
    ImGui::BeginChild("Material", {-1, -1});
    ImGui::Text("Uniforms");
    ImGui::Separator();

    for (auto&& it : material->Uniforms) {
      const Variable *variable = it.second.GetVariable();
      ImGui::BulletText("%s", variable->GetName().c_str());
    }

    ImGui::EndChild();
  }


  ImGui::End();

}



void RunUi(UiData *) {
  ShaderWindow();
  MaterialWindow();
}



}   // namespace picasso
