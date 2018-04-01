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

#include <imgui/imgui.h>

#include <string>
#include <vector>

namespace picasso {
namespace ui {
namespace widgets {

class Popup {
 public:
  Popup(const char *id, int index,
        const std::vector<const char*>& options) : index_(index) {
    const char *title = index >= 0 ? options[index] : "<NONE>";
    Setup(id, title, options);
  }

  ~Popup() {
    if (open_) {
      ImGui::EndPopup();
    }
  }

 public:
  bool Open() const { return open_; }
  bool Index() const { return index_; }

 private:
  inline void Setup(const char *id, const char *title,
                    const std::vector<const char*>& options) {
    if (ImGui::Button(title)) {
      ImGui::OpenPopup(id);
    }
    open_ = ImGui::BeginPopup(id);
    if (open_) {
      if (ImGui::Selectable("<NONE>")) {
        index_ = -1;
      }
      for (size_t i = 0; i < options.size(); i++) {
        if (ImGui::Selectable(options[i])) {
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
                         const std::vector<const char*>& options) {
  Popup popup(id, index, options);
  return popup.Index();
}


}   // namespace widgets
}   // namespace ui
}   // namespace picasso

#endif  // SRC_UI_WIDGETS_H
