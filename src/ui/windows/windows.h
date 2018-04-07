/******************************************************************************
 * @file: windows.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-07
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UI_WINDOWS_WINDOWS_H
#define SRC_UI_WINDOWS_WINDOWS_H

#include <imgui/imgui.h>
#include "ui/ui.h"

namespace picasso {
namespace ui {
namespace windows {

void ShaderWindow(UiData*, ImVec2 start_pos, ImVec2 start_size);
void MaterialWindow(UiData *, ImVec2 start_pos, ImVec2 start_size);
void MeshWindow(UiData*, ImVec2 start_pos, ImVec2 start_size);
void CameraWindow(UiData*, ImVec2 start_pos, ImVec2 start_size);

}   // namespace windows
}   // namespace ui
}   // namespace picasso

#endif  // SRC_UI_WINDOWS_WINDOWS_H
