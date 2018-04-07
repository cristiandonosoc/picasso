/******************************************************************************
 * @file: camera_window.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-07
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "ui/windows/windows.h"

#include "utils/scope_trigger.h"
#include "logging/log.h"

#include "ui/widgets/transform.h"
#include "camera.h"

using ::picasso::Camera;
extern Camera GLOBAL_CAMERA;

namespace picasso {
namespace ui {
namespace windows {

using ::picasso::ui::widgets::TransformWidget;


void CameraWindow(UiData *, ImVec2 start_pos, ImVec2 start_size) {
  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);
  static bool open = true;
  SCOPED_TRIGGER(ImGui::Begin("Camera", &open), ImGui::End());

  if (TransformWidget(GLOBAL_CAMERA.transform)) {
    GLOBAL_CAMERA.ReloadViewMatrix();
  }
}

}   // namespace windows
}   // namespace ui
}   // namespace picasso
