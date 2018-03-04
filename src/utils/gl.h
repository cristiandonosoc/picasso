#ifndef SRC_UTILS_GL_H
#define SRC_UTILS_GL_H

#include "result.h"

#include <GL/gl3w.h>
#include <map>

namespace picasso {
namespace utils {

/**
 * GLTypesToString
 * ---------------
 *
 * Singleton that holds the mapping from attribute types to a string
 * glGetString wouldn't accept it and could not find another function that did it.
 * Hopefully there is one out there.
 *
 * You can get the singleton explicitly (GLTypesToString::Instance()) or call
 * a global reference in GL_TYPES_TO_STRING
 *
 * I will resort to the latter as I use CAPS_NAMING to refer to globals
 **/
class GLTypesToString {

 public:
   static const GLTypesToString& Instance() {
     static GLTypesToString instance;
     return instance;
   }
 public:
   ResultOr<std::string> Get(GLenum type) const {
     auto it = mapping_.find(type);
     if (it == mapping_.end()) {
       return ResultOr<std::string>::Error("Cannot find type %d\n", type);
     }
     return it->second;
   }

 private:
  GLTypesToString() {
		mapping_[GL_FLOAT] = "GL_FLOAT";
		mapping_[GL_FLOAT_VEC2] = "GL_FLOAT_VEC2";
		mapping_[GL_FLOAT_VEC3] = "GL_FLOAT_VEC3";
		mapping_[GL_FLOAT_VEC4] = "GL_FLOAT_VEC4";
		mapping_[GL_DOUBLE] = "GL_DOUBLE";
		mapping_[GL_DOUBLE_VEC2] = "GL_DOUBLE_VEC2";
		mapping_[GL_DOUBLE_VEC3] = "GL_DOUBLE_VEC3";
		mapping_[GL_DOUBLE_VEC4] = "GL_DOUBLE_VEC4";
		mapping_[GL_INT] = "GL_INT";
		mapping_[GL_INT_VEC2] = "GL_INT_VEC2";
		mapping_[GL_INT_VEC3] = "GL_INT_VEC3";
		mapping_[GL_INT_VEC4] = "GL_INT_VEC4";
		mapping_[GL_UNSIGNED_INT] = "GL_UNSIGNED_INT";
		mapping_[GL_UNSIGNED_INT_VEC2] = "GL_UNSIGNED_INT_VEC2";
		mapping_[GL_UNSIGNED_INT_VEC3] = "GL_UNSIGNED_INT_VEC3";
		mapping_[GL_UNSIGNED_INT_VEC4] = "GL_UNSIGNED_INT_VEC4";
		mapping_[GL_BOOL] = "GL_BOOL";
		mapping_[GL_BOOL_VEC2] = "GL_BOOL_VEC2";
		mapping_[GL_BOOL_VEC3] = "GL_BOOL_VEC3";
		mapping_[GL_BOOL_VEC4] = "GL_BOOL_VEC4";
		mapping_[GL_FLOAT_MAT2] = "GL_FLOAT_MAT2";
		mapping_[GL_FLOAT_MAT3] = "GL_FLOAT_MAT3";
		mapping_[GL_FLOAT_MAT4] = "GL_FLOAT_MAT4";
		mapping_[GL_FLOAT_MAT2x3] = "GL_FLOAT_MAT2x3";
		mapping_[GL_FLOAT_MAT2x4] = "GL_FLOAT_MAT2x4";
		mapping_[GL_FLOAT_MAT3x2] = "GL_FLOAT_MAT3x2";
		mapping_[GL_FLOAT_MAT3x4] = "GL_FLOAT_MAT3x4";
		mapping_[GL_FLOAT_MAT4x2] = "GL_FLOAT_MAT4x2";
		mapping_[GL_FLOAT_MAT4x3] = "GL_FLOAT_MAT4x3";
		mapping_[GL_DOUBLE_MAT2] = "GL_DOUBLE_MAT2";
		mapping_[GL_DOUBLE_MAT3] = "GL_DOUBLE_MAT3";
		mapping_[GL_DOUBLE_MAT4] = "GL_DOUBLE_MAT4";
		mapping_[GL_DOUBLE_MAT2x3] = "GL_DOUBLE_MAT2x3";
		mapping_[GL_DOUBLE_MAT2x4] = "GL_DOUBLE_MAT2x4";
		mapping_[GL_DOUBLE_MAT3x2] = "GL_DOUBLE_MAT3x2";
		mapping_[GL_DOUBLE_MAT3x4] = "GL_DOUBLE_MAT3x4";
		mapping_[GL_DOUBLE_MAT4x2] = "GL_DOUBLE_MAT4x2";
		mapping_[GL_DOUBLE_MAT4x3] = "GL_DOUBLE_MAT4x3";
		mapping_[GL_SAMPLER_1D] = "GL_SAMPLER_1D";
		mapping_[GL_SAMPLER_2D] = "GL_SAMPLER_2D";
		mapping_[GL_SAMPLER_3D] = "GL_SAMPLER_3D";
		mapping_[GL_SAMPLER_CUBE] = "GL_SAMPLER_CUBE";
		mapping_[GL_SAMPLER_1D_SHADOW] = "GL_SAMPLER_1D_SHADOW";
		mapping_[GL_SAMPLER_2D_SHADOW] = "GL_SAMPLER_2D_SHADOW";
		mapping_[GL_SAMPLER_1D_ARRAY] = "GL_SAMPLER_1D_ARRAY";
		mapping_[GL_SAMPLER_2D_ARRAY] = "GL_SAMPLER_2D_ARRAY";
		mapping_[GL_SAMPLER_1D_ARRAY_SHADOW] = "GL_SAMPLER_1D_ARRAY_SHADOW";
		mapping_[GL_SAMPLER_2D_ARRAY_SHADOW] = "GL_SAMPLER_2D_ARRAY_SHADOW";
		mapping_[GL_SAMPLER_2D_MULTISAMPLE] = "GL_SAMPLER_2D_MULTISAMPLE";
		mapping_[GL_SAMPLER_2D_MULTISAMPLE_ARRAY] = "GL_SAMPLER_2D_MULTISAMPLE_ARRAY";
		mapping_[GL_SAMPLER_CUBE_SHADOW] = "GL_SAMPLER_CUBE_SHADOW";
		mapping_[GL_SAMPLER_BUFFER] = "GL_SAMPLER_BUFFER";
		mapping_[GL_SAMPLER_2D_RECT] = "GL_SAMPLER_2D_RECT";
		mapping_[GL_SAMPLER_2D_RECT_SHADOW] = "GL_SAMPLER_2D_RECT_SHADOW";
		mapping_[GL_INT_SAMPLER_1D] = "GL_INT_SAMPLER_1D";
		mapping_[GL_INT_SAMPLER_2D] = "GL_INT_SAMPLER_2D";
		mapping_[GL_INT_SAMPLER_3D] = "GL_INT_SAMPLER_3D";
		mapping_[GL_INT_SAMPLER_CUBE] = "GL_INT_SAMPLER_CUBE";
		mapping_[GL_INT_SAMPLER_1D_ARRAY] = "GL_INT_SAMPLER_1D_ARRAY";
		mapping_[GL_INT_SAMPLER_2D_ARRAY] = "GL_INT_SAMPLER_2D_ARRAY";
		mapping_[GL_INT_SAMPLER_2D_MULTISAMPLE] = "GL_INT_SAMPLER_2D_MULTISAMPLE";
		mapping_[GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] = "GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
		mapping_[GL_INT_SAMPLER_BUFFER] = "GL_INT_SAMPLER_BUFFER";
		mapping_[GL_INT_SAMPLER_2D_RECT] = "GL_INT_SAMPLER_2D_RECT";
		mapping_[GL_UNSIGNED_INT_SAMPLER_1D] = "GL_UNSIGNED_INT_SAMPLER_1D";
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D] = "GL_UNSIGNED_INT_SAMPLER_2D";
		mapping_[GL_UNSIGNED_INT_SAMPLER_3D] = "GL_UNSIGNED_INT_SAMPLER_3D";
		mapping_[GL_UNSIGNED_INT_SAMPLER_CUBE] = "GL_UNSIGNED_INT_SAMPLER_CUBE";
		mapping_[GL_UNSIGNED_INT_SAMPLER_1D_ARRAY] = "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY";
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D_ARRAY] = "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY";
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE] = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE";
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
		mapping_[GL_UNSIGNED_INT_SAMPLER_BUFFER] = "GL_UNSIGNED_INT_SAMPLER_BUFFER";
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D_RECT] = "GL_UNSIGNED_INT_SAMPLER_2D_RECT";
		mapping_[GL_IMAGE_1D] = "GL_IMAGE_1D";
		mapping_[GL_IMAGE_2D] = "GL_IMAGE_2D";
		mapping_[GL_IMAGE_3D] = "GL_IMAGE_3D";
		mapping_[GL_IMAGE_2D_RECT] = "GL_IMAGE_2D_RECT";
		mapping_[GL_IMAGE_CUBE] = "GL_IMAGE_CUBE";
		mapping_[GL_IMAGE_BUFFER] = "GL_IMAGE_BUFFER";
		mapping_[GL_IMAGE_1D_ARRAY] = "GL_IMAGE_1D_ARRAY";
		mapping_[GL_IMAGE_2D_ARRAY] = "GL_IMAGE_2D_ARRAY";
		mapping_[GL_IMAGE_2D_MULTISAMPLE] = "GL_IMAGE_2D_MULTISAMPLE";
		mapping_[GL_IMAGE_2D_MULTISAMPLE_ARRAY] = "GL_IMAGE_2D_MULTISAMPLE_ARRAY";
		mapping_[GL_INT_IMAGE_1D] = "GL_INT_IMAGE_1D";
		mapping_[GL_INT_IMAGE_2D] = "GL_INT_IMAGE_2D";
		mapping_[GL_INT_IMAGE_3D] = "GL_INT_IMAGE_3D";
		mapping_[GL_INT_IMAGE_2D_RECT] = "GL_INT_IMAGE_2D_RECT";
		mapping_[GL_INT_IMAGE_CUBE] = "GL_INT_IMAGE_CUBE";
		mapping_[GL_INT_IMAGE_BUFFER] = "GL_INT_IMAGE_BUFFER";
		mapping_[GL_INT_IMAGE_1D_ARRAY] = "GL_INT_IMAGE_1D_ARRAY";
		mapping_[GL_INT_IMAGE_2D_ARRAY] = "GL_INT_IMAGE_2D_ARRAY";
		mapping_[GL_INT_IMAGE_2D_MULTISAMPLE] = "GL_INT_IMAGE_2D_MULTISAMPLE";
		mapping_[GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY] = "GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
		mapping_[GL_UNSIGNED_INT_IMAGE_1D] = "GL_UNSIGNED_INT_IMAGE_1D";
		mapping_[GL_UNSIGNED_INT_IMAGE_2D] = "GL_UNSIGNED_INT_IMAGE_2D";
		mapping_[GL_UNSIGNED_INT_IMAGE_3D] = "GL_UNSIGNED_INT_IMAGE_3D";
		mapping_[GL_UNSIGNED_INT_IMAGE_2D_RECT] = "GL_UNSIGNED_INT_IMAGE_2D_RECT";
		mapping_[GL_UNSIGNED_INT_IMAGE_CUBE] = "GL_UNSIGNED_INT_IMAGE_CUBE";
		mapping_[GL_UNSIGNED_INT_IMAGE_BUFFER] = "GL_UNSIGNED_INT_IMAGE_BUFFER";
		mapping_[GL_UNSIGNED_INT_IMAGE_1D_ARRAY] = "GL_UNSIGNED_INT_IMAGE_1D_ARRAY";
		mapping_[GL_UNSIGNED_INT_IMAGE_2D_ARRAY] = "GL_UNSIGNED_INT_IMAGE_2D_ARRAY";
		mapping_[GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE] = "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE";
		mapping_[GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY] = "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
		mapping_[GL_UNSIGNED_INT_ATOMIC_COUNTER] = "GL_UNSIGNED_INT_ATOMIC_COUNTER";
  }

 private:
   std::map<GLenum, std::string> mapping_;
};

// Exported singleton variable
extern const GLTypesToString& GL_TYPES_TO_STRING;

}   // namespace utils
}   // namespace picasso


#endif  // SRC_UTILS_GL_H
