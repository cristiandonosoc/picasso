#ifndef SRC_UTILS_GL_H
#define SRC_UTILS_GL_H

#include "result.h"

#include <GL/gl3w.h>
#include <map>

namespace picasso {
namespace utils {

/**
 * GlTypesToString
 * ---------------
 *
 * Singleton that holds the mapping from attribute types to a string
 * glGetString wouldn't accept it and could not find another function that did it.
 * Hopefully there is one out there.
 *
 * You can get the singleton explicitly (GlTypesToString::Instance()) or call
 * a global reference in GL_TYPES_TO_STRING
 *
 * I will resort to the latter as I use CAPS_NAMING to refer to globals
 **/
class GlTypesToString {

 public:
   static const GlTypesToString& Instance() {
     static GlTypesToString instance;
     return instance;
   }
 public:
   ResultOr<std::string> Get(GLenum type) const {
     auto it = mapping_.find(type);
     if (it == mapping_.end()) {
       ResultOr<std::string>::Error("Cannot find type %d\n", type);
     }
     return it->second;
   }

 private:
  GlTypesToString() {
    mapping_[GL_FLOAT] = "GL_FLOAT";
    mapping_[GL_FLOAT_VEC2] = "GL_FLOAT_VEC2";
    mapping_[GL_FLOAT_VEC3] = "GL_FLOAT_VEC3";
    mapping_[GL_FLOAT_VEC4] = "GL_FLOAT_VEC4";
    mapping_[GL_FLOAT_MAT2] = "GL_FLOAT_MAT2";
    mapping_[GL_FLOAT_MAT3] = "GL_FLOAT_MAT3";
    mapping_[GL_FLOAT_MAT4] = "GL_FLOAT_MAT4";
    mapping_[GL_FLOAT_MAT2x3] = "GL_FLOAT_MAT2x3";
    mapping_[GL_FLOAT_MAT2x4] = "GL_FLOAT_MAT2x4";
    mapping_[GL_FLOAT_MAT3x2] = "GL_FLOAT_MAT3x2";
    mapping_[GL_FLOAT_MAT3x4] = "GL_FLOAT_MAT3x4";
    mapping_[GL_FLOAT_MAT4x2] = "GL_FLOAT_MAT4x2";
    mapping_[GL_FLOAT_MAT4x3] = "GL_FLOAT_MAT4x3";
  }

 private:
   std::map<GLenum, std::string> mapping_;
};

const GlTypesToString& GL_TYPES_TO_STRING = GlTypesToString::Instance();

}   // namespace utils
}   // namespace picasso





#endif  // SRC_UTILS_GL_H
