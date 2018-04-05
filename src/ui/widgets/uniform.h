/******************************************************************************
 * @file: uniforms.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-04
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UI_WIDGETS_UNIFORMS_H
#define SRC_UI_WIDGETS_UNIFORMS_H

#include "assets/shaders/uniform.h"

namespace picasso {
namespace ui {
namespace widgets {

using ::picasso::assets::shaders::UniformValue;

void GenerateUniformWidget(UniformValue&);

}   // namespace widgets
}   // namespace ui
}   // namespace picasso

#endif  // SRC_UI_WIDGETS_UNIFORMS_H
