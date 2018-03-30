/******************************************************************************
 * @file: uniform.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_SHADERS_UNIFORM_H
#define SRC_ASSETS_SHADERS_UNIFORM_H

#include <map>
#include <string>
#include <GL/gl3w.h>

#include "utils/printable_enum.h"

namespace picasso {
namespace assets {
namespace shaders {

PRINTABLE_ENUM(UniformWidget,
    COLOR,
    FLOAT,
    FLOAT_VEC2,
    FLOAT_VEC3,
    FLOAT_VEC4,
    MAT3,
    MAT4,
    TEXTURE_2D
    );


extern std::map<GLenum, UniformWidget> UNIFORM_WIDGET_MAPPING;

class Uniform {
 public:
  std::string name;
  std::string type_name;
  int location;
  GLenum type;
  size_t count;
  size_t type_size;
  UniformWidget widget;
};  // class Uniform

}   // namespace shaders
}   // namespace assets
}   // namespace picasso



#endif  // SRC_ASSETS_SHADERS_UNIFORM_H
