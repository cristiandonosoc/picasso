/******************************************************************************
 * @file: transform.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-04
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UI_WIDGETS_TRANSFORM_H
#define SRC_UI_WIDGETS_TRANSFORM_H

#include "models/transform.h"

namespace picasso {
namespace ui {
namespace widgets {

using ::picasso::models::Transform;

bool TransformWidget(const char *id, Transform& transform);

}   // namespace widgets
}   // namespace ui
}   // namespace picasso


#endif  // SRC_UI_WIDGETS_TRANSFORM_H
