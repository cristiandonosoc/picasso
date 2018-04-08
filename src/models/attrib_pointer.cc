/******************************************************************************
 * @file: attrib_pointer.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-07
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "models/attrib_pointer.h"

namespace picasso {
namespace models {

void AttributePointer::Bind(int location) const {
  glEnableVertexAttribArray(location);
  glVertexAttribPointer(
      location,  // Attribute location int he shader
      // Size: How many types are per attrib "element".
      //       Effectivelly this is saying where we have
      //       a vec, vec2, vec3 or vec4
      GetSize(),
      // Type of value this attribute is (GL_FLOAT, GL_DOUBLE, etc)
      GetType(),
      // Whether to normalize the data
      GetNormalize(),
      // Stride: Space in bytes between two consecutive values of this
      //         attribute in the array.
      // A value of 0 is saying that the buffer is densely packed.
      GetStride(),
      // Offset: Space in bytes from the start of the array where the
      //        first value is.
      // Value starts from the beginning of the array
      (void*)(size_t)GetOffset());
}


}   // namespace models
}   // namespace picasso
