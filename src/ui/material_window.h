/******************************************************************************
 * @file: material_window.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-01
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UI_MATERIAL_WINDOW_H
#define SRC_UI_MATERIAL_WINDOW_H

#include <imgui/imgui.h>

namespace picasso {
namespace ui {

class UiData;
void MaterialWindow(UiData*, ImVec2 start_pos, ImVec2 start_size);

}   // namespace ui
}   // namespace picasso

#endif  // SRC_UI_MATERIAL_WINDOW_H
