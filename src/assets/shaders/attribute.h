/******************************************************************************
 * @file: attribute.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_SHADERS_ATTRIBUTE_H
#define SRC_ASSETS_SHADERS_ATTRIBUTE_H

#include <string>
#include <GL/gl3w.h>

namespace picasso {
namespace assets {
namespace shaders {

class Attribute {
 public:
  std::string name;
  std::string type_name;
  int location;
  GLenum type;
  int count;
  int type_size;
};  // class Attribute

}   // namespace shaders
}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_SHADERS_ATTRIBUTE_H
