/******************************************************************************
 * @file: ui.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-27
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <algorithm>
#include <cstring>
#include <regex>

#include "ui.h"
#include "assets/shaders/shader_registry.h"
#include "assets/materials/material_registry.h"
#include "logging/log.h"
#include "utils/snprintf.h"
#include "utils/string.h"
#include "assets/texture_registry.h"

#include "mappers/shader_material_mapper.h"

#include "platform.h"

#include <ctime>
#include <iomanip>

#undef min
#undef max

namespace picasso {
namespace ui {

using ::picasso::assets::shaders::ShaderRegistry;
using ::picasso::assets::shaders::Shader;
using ::picasso::assets::shaders::Attribute;
using ::picasso::assets::shaders::Uniform;

using ::picasso::assets::materials::Material;
using ::picasso::assets::materials::MaterialRegistry;
using ::picasso::assets::materials::UniformValue;
using ::picasso::logging::LogBuffer;
using ::picasso::utils::picasso_snprintf;
using ::picasso::assets::Texture;
using ::picasso::assets::TextureRegistry;

using ::picasso::mappers::ShaderMaterialMapper;

using ::picasso::utils::FormattedString;

using ::picasso::Platform;

void SystemWindow(UiData *ui_data, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  ImGui::Begin("System", &open);
  ImGui::ColorEdit3("Clear Color", (float*)&ui_data->clear_color);

  if (ImGui::Button("Open File")) {
    auto res = Platform::FileDialog("");
    LOG_NON_OK_STATUS(res);
  }

  ImGui::End();
}

void ShaderWindow(UiData *, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  ImGui::Begin("Shaders", &open);

  ImGui::BeginChild("Left Pane", {150, 0}, true);
  auto&& shaders = ShaderRegistry::GetShaders();
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
  auto&& materials = MaterialRegistry::GetMaterials();
  static int selected_material = -1;
  static MaterialRegistry::KeyType selected_key;
  int i = 0;
  for (auto&& it = materials.begin();
       it != materials.end();
       it++, i++) {
    MaterialRegistry::KeyType key = it->first;
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


  if (material) {
    ImGui::BeginChild("Material", {-1, -1});

    std::string title = "Material";
    const Shader *material_shader = material->GetShader();
    if (material_shader) {
      title += FormattedString(" [Shader: %s]",
          material_shader->GetName().c_str());
    }

    if (ImGui::CollapsingHeader(title.c_str())) {
      int current_index = -1;
      std::vector<const char*> shader_names;
      std::vector<ShaderRegistry::KeyType> shader_keys;
      const auto& map = ShaderRegistry::GetMap();
      shader_names.reserve(map.size());
      shader_keys.reserve(map.size());
      int index = 0;
      for (const auto& it : ShaderRegistry::GetMap()) {
        const auto& key = it.first;
        shader_keys.push_back(key);

        const Shader *shader = it.second.get();
        shader_names.push_back(shader->GetName().c_str());
        if (shader == material->GetShader()) {
          current_index = index;
        }
        index++;
      }

      int prev_index = current_index;
      ImGui::ListBox("Shader", &current_index,
                     &shader_names[0], shader_names.size(), 4);
      if (prev_index != current_index) {
        const auto& prev_shader_key = shader_keys[prev_index];
        const auto& shader_key = shader_keys[current_index];
        ShaderMaterialMapper::RemoveMapping(prev_shader_key, selected_key);
        ShaderMaterialMapper::AddMapping(shader_key, selected_key);
      }
    }

    ImGui::Text("Uniforms");
    ImGui::Separator();

    static std::regex color_regex("color", std::regex_constants::ECMAScript |
                                           std::regex_constants::icase);

    int mat_count = 0;
    for (auto&& it : material->Uniforms) {
      UniformValue& value = it.second;
      const Uniform *uniform = value.GetUniform();

      ImGui::PushID(mat_count);
      const std::string& name = uniform->name;
      if(ImGui::TreeNode(name.c_str(), "%s", name.c_str())) {
        ImGui::BulletText("Location: %d", uniform->location);
        ImGui::BulletText("Type: %s", uniform->type_name.c_str());
        ImGui::BulletText("Type Size: %zu", uniform->type_size);
        ImGui::BulletText("Size: %zu", uniform->count);
        ImGui::TreePop();
      }

      float *ptr = value.GetValue<GLfloat>();
      ImGui::Text("PTR: %p, VAL: %f", ptr, *ptr);
      if (uniform->type == GL_FLOAT) {
        ImGui::SliderFloat("Value", ptr, 0.0f, 1.0f);
      }

      // TODO(Cristian): Do actual mapping from the shaders
      if (std::regex_search(name, color_regex)) {
        if (uniform->type_size == 4) {
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
  static MaterialRegistry::KeyType selected_key;
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
#ifdef _WIN32
  struct tm result;
  localtime_s(&result, &entry.time);
  char buf[128];
  strftime(buf, sizeof(buf), "%H:%M:%S", &result);
#else
  struct tm result;
  result = *localtime(&entry.time);
  char buf[128];
  strftime(buf, sizeof(buf), "%H:%M:%S", &result);
#endif

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

}   // namespace ui
}   // namespace picasso
