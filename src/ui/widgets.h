/******************************************************************************
 * @file: widgets.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-01
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UI_WIDGETS_H
#define SRC_UI_WIDGETS_H

#include "utils/macros.h"
#include "utils/status.h"

#include <imgui/imgui.h>

#include "assets/shaders/uniform.h"
#include "models/transform.h"

#include <cassert>
#include <string>
#include <vector>

namespace picasso {
namespace ui {
namespace widgets {

using ::picasso::assets::shaders::UniformValue;
using ::picasso::models::Transform;

void GenerateUniformWidget(UniformValue& uniform);
bool TransformWidget(Transform& transform);

class Popup {
 public:
  Popup(const char *id, int index,
        const std::vector<std::string>& options,
        bool null_value = true) : index_(index) {
    const char *title;
    if (!null_value) {
      assert((index >= 0) && (index < (int)options.size()));
      title = options[index].c_str();
    } else {
      title = index >= 0 ? options[index].c_str() : "<NONE>";
    }
    Setup(id, title, options, null_value);
  }

  ~Popup() {
    if (open_) {
      ImGui::EndPopup();
    }
  }

 public:
  bool Open() const { return open_; }
  int Index() const { return index_; }

 private:
  inline void Setup(const char *id, const char *title,
                    const std::vector<std::string>& options,
                    bool null_value) {
    ImGui::PushID(id);
    bool click = ImGui::Button(title);
    ImGui::PopID();

    if (click) {
      ImGui::OpenPopup(id);
    }
    open_ = ImGui::BeginPopup(id);
    if (open_) {
      if (null_value) {
        if (ImGui::Selectable("<NONE>")) {
          index_ = -1;
        }
      }
      for (size_t i = 0; i < options.size(); i++) {
        if (ImGui::Selectable(options[i].c_str())) {
          index_ = i;
        }
      }
    }
  }

 private:
  bool open_;
  int index_;

};  // class Popup

inline int GetPopupIndex(const char *id, int index,
                         const std::vector<std::string>& options,
                         bool null_value = true) {
  Popup popup(id, index, options, null_value);
  return popup.Index();
}

template<typename Enum>
Enum EnumPopup(const char *id, Enum val) {
  const auto& options = Enum::GetOptionNames();
  const std::string& name = Enum::ToString(val);
  int index = -1;
  int i = 0;
  for (const auto& option : options) {
    if (option == name) {
      index = i;
      break;
    }
    i++;
  }
  assert(index != -1);
  index = GetPopupIndex(id, index, options, false);
  return Enum::FromString(options[index]).ConsumeOrDie();
}

}   // namespace widgets
}   // namespace ui
}   // namespace picasso

#endif  // SRC_UI_WIDGETS_H
