/******************************************************************************
 * @file: ui.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-27
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UI_UI_H
#define SRC_UI_UI_H

#include <imgui/imgui.h>


namespace picasso {
namespace ui {

class UiData {
 public:
  ImVec4 clear_color;
};

void RunUi(UiData *);

}   // namespace ui
}   // namespace picasso

#endif  // SRC_UI_UI_H
