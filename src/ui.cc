#include <algorithm>
#include <cstring>
#include <regex>

#include "ui.h"
#include "shaders/shader_registry.h"
#include "shaders/material_registry.h"
#include "logging/log.h"
#include "utils/snprintf.h"
#include "assets/texture_registry.h"

#include "platform.h"

#include <ctime>
#include <iomanip>

#undef min
#undef max

namespace picasso {

using ::picasso::shaders::Shader;
using ::picasso::shaders::Material;
using ::picasso::shaders::Variable;
using ::picasso::shaders::Value;
using ::picasso::logging::LogBuffer;
using ::picasso::utils::picasso_snprintf;
using ::picasso::assets::Texture;
using ::picasso::assets::TextureRegistry;

using ::picasso::Platform;

#if 0
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

#endif

void SystemWindow(UiData *ui_data, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  ImGui::Begin("System", &open);
  ImGui::ColorEdit3("Clear Color", (float*)&ui_data->clear_color);

  if (ImGui::Button("Open File")) {
    auto res = Platform::FileDialog("");
    if (res.Ok()) {
      LOG_INFO("Found file: %s", res.ConsumeOrDie().c_str());
    } else {
      LOG_ERROR("Could not find file");
    }
  }


  ImGui::End();
}

void ShaderWindow(UiData *, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  ImGui::Begin("Shaders", &open);

  ImGui::BeginChild("Left Pane", {150, 0}, true);
  auto&& shaders = shaders::ShaderRegistry::GetShaders();
  static int selected_shader = -1;
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
          len = std::min(vertex_src.size(), sizeof(buf));
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
          len = std::min(fragment_src.size(), sizeof(buf));
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

void MaterialWindow(UiData *, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  ImGui::Begin("Materials", &open);

  ImGui::BeginChild("Left Pane", {150, 0}, true);
  auto&& materials = shaders::MaterialRegistry::GetMaterials();
  static int selected_material = -1;
  static shaders::MaterialKey selected_key;
  int i = 0;
  for (auto&& it = materials.begin();
       it != materials.end();
       it++, i++) {
    shaders::MaterialKey key = it->first;
    /* Material* material = it->second; */
    char label[128];
    picasso_snprintf(label, sizeof(label), "%s", key.c_str());
    if (ImGui::Selectable(label, selected_material == i)) {
      selected_material = i;
      selected_key = key;
    }
  }

  ImGui::EndChild();
  ImGui::SameLine();

  Material *material= nullptr;
  if (selected_material >= 0) {
    auto it = materials.find(selected_key);
    material = it != materials.end() ? it->second.get() : nullptr;
  }

  /* float height = ImGui::GetContentRegionAvail().y; */

  if (material) {
    ImGui::BeginChild("Material", {-1, -1});
    ImGui::Text("Uniforms");
    ImGui::Separator();

    static std::regex color_regex("color", std::regex_constants::ECMAScript |
                                           std::regex_constants::icase);

    int mat_count = 0;
    for (auto&& it : material->Uniforms) {
      Value& value = it.second;
      const Variable *variable = value.GetVariable();
      const std::string& name = variable->GetName();

      ImGui::PushID(mat_count);
      if(ImGui::TreeNode(name.c_str(), "%s", variable->GetName().c_str())) {
        ImGui::BulletText("Location: %d", variable->GetLocation());
        ImGui::BulletText("Type: %s", variable->GetTypeName().c_str());
        ImGui::BulletText("Type Size: %zu", variable->GetTypeSize());
        ImGui::BulletText("Size: %zu", variable->GetSize());
        ImGui::TreePop();
      }

      float *ptr = value.GetValue<GLfloat>();
      ImGui::Text("PTR: %p, VAL: %f", ptr, *ptr);
      if (variable->GetType() == GL_FLOAT) {
        ImGui::SliderFloat("Value", ptr, 0.0f, 1.0f);
      }

      if (std::regex_search(name, color_regex)) {
        if (variable->GetTypeSize() == 4) {
          ImGui::ColorEdit4("", value.GetValue<float>());
        } else {
          ImGui::ColorEdit3("", value.GetValue<float>());
        }
      }
      ImGui::PopID();
      mat_count++;
      ImGui::Separator();
    }

    ImGui::EndChild();
  }


  ImGui::End();
}

void TextureWindow(UiData*, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  ImGui::Begin("Textures", &open);

  ImGui::BeginChild("Left Pane", {150, 0}, true);
  auto&& texture_map = TextureRegistry::GetTextureMap();
  static int selected_index = -1;
  static ::picasso::shaders::MaterialKey selected_key;
  int i = 0;
  for (auto&& it = texture_map.begin();
       it != texture_map.end();
       it++, i++) {
    auto&& key = it->first;
    /* Material* material = it->second; */
    char label[128];
    picasso_snprintf(label, sizeof(label), "%s", key.c_str());
    if (ImGui::Selectable(label, selected_index == i)) {
      selected_index = i;
      selected_key = key;
    }
  }

  ImGui::EndChild();
  ImGui::SameLine();

  Texture::SharedPtr texture = nullptr;
  if (selected_index >= 0) {
    auto it = texture_map.find(selected_key);
    texture = it != texture_map.end() ? it->second : nullptr;
  }

  /* float height = ImGui::GetContentRegionAvail().y; */

  if (texture) {
    ImGui::BeginChild("Texture", {-1, -1});

    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth());
    ImGui::BulletText("Name: %s", texture->GetName().c_str());
    ImGui::BulletText("Path:");
    ImGui::Text("%s", texture->GetPath().c_str());
    ImGui::BulletText("Width: %d", texture->GetWidth());
    ImGui::BulletText("Height: %d", texture->GetHeight());
    ImGui::BulletText("Num. Channels: %d", texture->GetNumChannels());
    ImGui::BulletText("OpenGL id: %u", texture->GetId());
    ImGui::PopTextWrapPos();
    ImGui::Separator();
    ImGui::Image((void*)(size_t)texture->GetId(), {200, 200});


    ImGui::EndChild();

  }
  ImGui::End();
}

inline void PrintEntry(const LogBuffer::Entry& entry, int count,
                       ImVec4& error_color, ImVec4& warn_color) {
  using ::picasso::logging::LogLevel;
  if (entry.msg.empty()) { return; }

  // Obtain the time
  struct tm result;
  localtime_s(&result, &entry.time);
  char buf[128];
  strftime(buf, sizeof(buf), "%H:%M:%S", &result);

  if (entry.level == LogLevel::LOG_ERROR || entry.level == LogLevel::LOG_WARN) {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float height = ImGui::GetTextLineHeight();
    float width = ImGui::GetWindowContentRegionWidth();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec4& color = entry.level == LogLevel::LOG_ERROR ? error_color : warn_color;
    draw_list->AddRectFilled(pos, {pos.x + width, pos.y + height}, 
                             ImGui::ColorConvertFloat4ToU32(color));
  }

  if (count > 0) {
    ImGui::Text("[%s.%zu]: %s (%d)", buf, entry.us, entry.msg.c_str(), count);
  } else {
    ImGui::Text("[%s.%zu]: %s", buf, entry.us, entry.msg.c_str());
  }
}

void LogWindow(UiData *, ImVec2 start_pos, ImVec2 start_size) {
  static bool open = true;
  static size_t log_count = 0;
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  ImGui::Begin("Log", &open, ImGuiWindowFlags_MenuBar);
  
  static bool docked = true;
  if (ImGui::BeginMenuBar()) {

    if (ImGui::BeginMenu("Docking")) {
      ImGui::MenuItem("Docked", NULL, &docked);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (ImGui::Button("Clear")) { LogBuffer::Clear(); }

  static ImVec4 error_color = {0.7f, 0.0f, 0.0f, 1.0f};
  static ImVec4 warn_color = {0.7f, 0.5f, 0.0f, 1.0f};
  auto flags = ImGuiColorEditFlags_NoInputs;
  ImGui::SameLine();
  ImGui::ColorEdit3("Error", (float*)&error_color, flags);
  ImGui::SameLine();
  ImGui::ColorEdit3("Warn", (float*)&warn_color, flags);
  ImGui::Separator();

  ImGui::BeginChild("scrolling", {0, 0}, false, ImGuiWindowFlags_HorizontalScrollbar);

  bool scroll_bottom = log_count != LogBuffer::Count();
  log_count = LogBuffer::Count();

  LogBuffer::Entry current_log_entry;
  int count = 0;
  for (const LogBuffer::Entry& log_entry : LogBuffer::GetLogs()) {
    if (log_entry.msg != current_log_entry.msg) {
      PrintEntry(current_log_entry, count, error_color, warn_color);
      count = 0;
    } else {
      if (count < 9999) { count++; }
    }
    current_log_entry = log_entry;
  }

  // We need to print the last string
  PrintEntry(current_log_entry, count, error_color, warn_color);

  static bool first_pass = true;
  if (first_pass || (scroll_bottom && count == 0)) {
    first_pass = false;
    ImGui::SetScrollHere();
  }

  ImGui::EndChild();
  ImGui::End();
}

void RunUi(UiData *ui_data) {
  SystemWindow(ui_data,   {0, 0},     {500, 500});
  ShaderWindow(ui_data,   {0, 500},   {500, 500});
  MaterialWindow(ui_data, {0, 1000},  {500, 500});
  TextureWindow(ui_data, {500, 500}, {500, 500});
  LogWindow(ui_data, {600, 800}, {1200, 300});
}



}   // namespace picasso
