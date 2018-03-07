#ifndef SRC_UI_H
#define SRC_UI_H

#include "imgui.h"

namespace picasso {

class UiData {
 public:
  float clear_color[4];
};

void ImGuiExample(const ImVec4& clear_color, bool show_demo_window,
                  bool show_another_window);

void RunUi(UiData *);

}   // namespace picasso

#endif  // SRC_UI_H
