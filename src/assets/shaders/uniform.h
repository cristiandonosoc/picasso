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

#include <string>
#include <GL/gl3w.h>

namespace picasso {
namespace assets {
namespace shaders {

class Uniform {
 public:
  std::string name;
  std::string type_name;
  int location;
  GLenum type;
  int count;
  int type_size;
};  // class Uniform

}   // namespace shaders
}   // namespace assets
}   // namespace picasso



#endif  // SRC_ASSETS_SHADERS_UNIFORM_H
