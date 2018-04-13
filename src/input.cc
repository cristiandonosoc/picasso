/******************************************************************************
 * @file: input.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-13
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "input.h"
#include "camera.h"

#include <imgui/imgui.h>

namespace picasso {

extern Camera GLOBAL_CAMERA;

namespace {

void HandleMouse(ImGuiIO& io) {
  // We move the camera in ording or it's view direction
  if (io.MouseWheel) {
    auto direction = GLOBAL_CAMERA.Direction();
    GLOBAL_CAMERA.transform.SetTranslation(GLOBAL_CAMERA.transform.GetTranslation() + 
                                           direction * io.MouseWheel * 0.1f);
    GLOBAL_CAMERA.ReloadViewMatrix();
  }
}


}   // namespace

void HandleInput() {
  auto& io = ImGui::GetIO();
  if (!io.WantCaptureMouse) {
    HandleMouse(io);
  }
}

}   // namespace picasso

