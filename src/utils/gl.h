#ifndef SRC_UTILS_GL_H
#define SRC_UTILS_GL_H

#include <GL/gl3w.h>
#include <map>

#include "utils/status_or.h"

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

 private:
  class GLTypeInfo {
   public:
     std::string name;
     size_t size;
  };

 public:
   static const GLTypesToString& Instance() {
     static GLTypesToString instance;
     return instance;
   }
 public:
   StatusOr<std::string> GetName(GLenum type) const {
     auto it = mapping_.find(type);
     if (it == mapping_.end()) {
       return StatusOr<std::string>::Error("Cannot find type %d", type);
     }
     return it->second.name;
   }

   StatusOr<size_t> GetSize(GLenum type) const {
     auto it = mapping_.find(type);
     if (it == mapping_.end()) {
       return StatusOr<size_t>::Error("Cannot find type %d", type);
     }
     return it->second.size;
   }

 private:
   std::map<GLenum, GLTypeInfo> mapping_;

 private:
  GLTypesToString() {
		mapping_[GL_FLOAT]                                      = { "GL_FLOAT",             1 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_VEC2]                                 = { "GL_FLOAT_VEC2",        2 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_VEC3]                                 = { "GL_FLOAT_VEC3",        3 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_VEC4]                                 = { "GL_FLOAT_VEC4",        4 * sizeof(GLfloat) };
		mapping_[GL_DOUBLE]                                     = { "GL_DOUBLE",            1 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_VEC2]                                = { "GL_DOUBLE_VEC2",       2 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_VEC3]                                = { "GL_DOUBLE_VEC3",       3 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_VEC4]                                = { "GL_DOUBLE_VEC4",       4 * sizeof(GLdouble) };
		mapping_[GL_INT]                                        = { "GL_INT",               1 * sizeof(GLint) };
		mapping_[GL_INT_VEC2]                                   = { "GL_INT_VEC2",          2 * sizeof(GLint) };
		mapping_[GL_INT_VEC3]                                   = { "GL_INT_VEC3",          3 * sizeof(GLint) };
		mapping_[GL_INT_VEC4]                                   = { "GL_INT_VEC4",          4 * sizeof(GLint) };
		mapping_[GL_UNSIGNED_INT]                               = { "GL_UNSIGNED_INT",      1 * sizeof(GLuint) };
		mapping_[GL_UNSIGNED_INT_VEC2]                          = { "GL_UNSIGNED_INT_VEC2", 2 * sizeof(GLuint) };
		mapping_[GL_UNSIGNED_INT_VEC3]                          = { "GL_UNSIGNED_INT_VEC3", 3 * sizeof(GLuint) };
		mapping_[GL_UNSIGNED_INT_VEC4]                          = { "GL_UNSIGNED_INT_VEC4", 4 * sizeof(GLuint) };
		mapping_[GL_BOOL]                                       = { "GL_BOOL",              1 * sizeof(GLboolean) };
		mapping_[GL_BOOL_VEC2]                                  = { "GL_BOOL_VEC2",         2 * sizeof(GLboolean) };
		mapping_[GL_BOOL_VEC3]                                  = { "GL_BOOL_VEC3",         3 * sizeof(GLboolean) };
		mapping_[GL_BOOL_VEC4]                                  = { "GL_BOOL_VEC4",         4 * sizeof(GLboolean) };
		mapping_[GL_FLOAT_MAT2]                                 = { "GL_FLOAT_MAT2",        2 * 2 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_MAT3]                                 = { "GL_FLOAT_MAT3",        3 * 3 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_MAT4]                                 = { "GL_FLOAT_MAT4",        4 * 4 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_MAT2x3]                               = { "GL_FLOAT_MAT2x3",      2 * 3 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_MAT2x4]                               = { "GL_FLOAT_MAT2x4",      2 * 4 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_MAT3x2]                               = { "GL_FLOAT_MAT3x2",      3 * 2 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_MAT3x4]                               = { "GL_FLOAT_MAT3x4",      3 * 4 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_MAT4x2]                               = { "GL_FLOAT_MAT4x2",      4 * 2 * sizeof(GLfloat) };
		mapping_[GL_FLOAT_MAT4x3]                               = { "GL_FLOAT_MAT4x3",      4 * 3 * sizeof(GLfloat) };
		mapping_[GL_DOUBLE_MAT2]                                = { "GL_DOUBLE_MAT2",       2 * 2 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_MAT3]                                = { "GL_DOUBLE_MAT3",       3 * 3 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_MAT4]                                = { "GL_DOUBLE_MAT4",       4 * 4 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_MAT2x3]                              = { "GL_DOUBLE_MAT2x3",     2 * 3 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_MAT2x4]                              = { "GL_DOUBLE_MAT2x4",     2 * 4 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_MAT3x2]                              = { "GL_DOUBLE_MAT3x2",     3 * 2 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_MAT3x4]                              = { "GL_DOUBLE_MAT3x4",     3 * 4 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_MAT4x2]                              = { "GL_DOUBLE_MAT4x2",     4 * 2 * sizeof(GLdouble) };
		mapping_[GL_DOUBLE_MAT4x3]                              = { "GL_DOUBLE_MAT4x3",     4 * 3 * sizeof(GLdouble) };
		mapping_[GL_SAMPLER_1D]                                 = { "GL_SAMPLER_1D", 0 };
		mapping_[GL_SAMPLER_2D]                                 = { "GL_SAMPLER_2D", 0 };
		mapping_[GL_SAMPLER_3D]                                 = { "GL_SAMPLER_3D", 0 };
		mapping_[GL_SAMPLER_CUBE]                               = { "GL_SAMPLER_CUBE", 0 };
		mapping_[GL_SAMPLER_1D_SHADOW]                          = { "GL_SAMPLER_1D_SHADOW", 0 };
		mapping_[GL_SAMPLER_2D_SHADOW]                          = { "GL_SAMPLER_2D_SHADOW", 0 };
		mapping_[GL_SAMPLER_1D_ARRAY]                           = { "GL_SAMPLER_1D_ARRAY", 0 };
		mapping_[GL_SAMPLER_2D_ARRAY]                           = { "GL_SAMPLER_2D_ARRAY", 0 };
		mapping_[GL_SAMPLER_1D_ARRAY_SHADOW]                    = { "GL_SAMPLER_1D_ARRAY_SHADOW", 0 };
		mapping_[GL_SAMPLER_2D_ARRAY_SHADOW]                    = { "GL_SAMPLER_2D_ARRAY_SHADOW", 0 };
		mapping_[GL_SAMPLER_2D_MULTISAMPLE]                     = { "GL_SAMPLER_2D_MULTISAMPLE", 0 };
		mapping_[GL_SAMPLER_2D_MULTISAMPLE_ARRAY]               = { "GL_SAMPLER_2D_MULTISAMPLE_ARRAY", 0 };
		mapping_[GL_SAMPLER_CUBE_SHADOW]                        = { "GL_SAMPLER_CUBE_SHADOW", 0 };
		mapping_[GL_SAMPLER_BUFFER]                             = { "GL_SAMPLER_BUFFER", 0 };
		mapping_[GL_SAMPLER_2D_RECT]                            = { "GL_SAMPLER_2D_RECT", 0 };
		mapping_[GL_SAMPLER_2D_RECT_SHADOW]                     = { "GL_SAMPLER_2D_RECT_SHADOW", 0 };
		mapping_[GL_INT_SAMPLER_1D]                             = { "GL_INT_SAMPLER_1D", 0 };
		mapping_[GL_INT_SAMPLER_2D]                             = { "GL_INT_SAMPLER_2D", 0 };
		mapping_[GL_INT_SAMPLER_3D]                             = { "GL_INT_SAMPLER_3D", 0 };
		mapping_[GL_INT_SAMPLER_CUBE]                           = { "GL_INT_SAMPLER_CUBE", 0 };
		mapping_[GL_INT_SAMPLER_1D_ARRAY]                       = { "GL_INT_SAMPLER_1D_ARRAY", 0 };
		mapping_[GL_INT_SAMPLER_2D_ARRAY]                       = { "GL_INT_SAMPLER_2D_ARRAY", 0 };
		mapping_[GL_INT_SAMPLER_2D_MULTISAMPLE]                 = { "GL_INT_SAMPLER_2D_MULTISAMPLE", 0 };
		mapping_[GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY]           = { "GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY", 0 };
		mapping_[GL_INT_SAMPLER_BUFFER]                         = { "GL_INT_SAMPLER_BUFFER", 0 };
		mapping_[GL_INT_SAMPLER_2D_RECT]                        = { "GL_INT_SAMPLER_2D_RECT", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_1D]                    = { "GL_UNSIGNED_INT_SAMPLER_1D", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D]                    = { "GL_UNSIGNED_INT_SAMPLER_2D", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_3D]                    = { "GL_UNSIGNED_INT_SAMPLER_3D", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_CUBE]                  = { "GL_UNSIGNED_INT_SAMPLER_CUBE", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_1D_ARRAY]              = { "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D_ARRAY]              = { "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE]        = { "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY]  = { "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_BUFFER]                = { "GL_UNSIGNED_INT_SAMPLER_BUFFER", 0 };
		mapping_[GL_UNSIGNED_INT_SAMPLER_2D_RECT]               = { "GL_UNSIGNED_INT_SAMPLER_2D_RECT", 0 };
		mapping_[GL_IMAGE_1D]                                   = { "GL_IMAGE_1D", 0 };
		mapping_[GL_IMAGE_2D]                                   = { "GL_IMAGE_2D", 0 };
		mapping_[GL_IMAGE_3D]                                   = { "GL_IMAGE_3D", 0 };
		mapping_[GL_IMAGE_2D_RECT]                              = { "GL_IMAGE_2D_RECT", 0 };
		mapping_[GL_IMAGE_CUBE]                                 = { "GL_IMAGE_CUBE", 0 };
		mapping_[GL_IMAGE_BUFFER]                               = { "GL_IMAGE_BUFFER", 0 };
		mapping_[GL_IMAGE_1D_ARRAY]                             = { "GL_IMAGE_1D_ARRAY", 0 };
		mapping_[GL_IMAGE_2D_ARRAY]                             = { "GL_IMAGE_2D_ARRAY", 0 };
		mapping_[GL_IMAGE_2D_MULTISAMPLE]                       = { "GL_IMAGE_2D_MULTISAMPLE", 0 };
		mapping_[GL_IMAGE_2D_MULTISAMPLE_ARRAY]                 = { "GL_IMAGE_2D_MULTISAMPLE_ARRAY", 0 };
		mapping_[GL_INT_IMAGE_1D]                               = { "GL_INT_IMAGE_1D", 0 };
		mapping_[GL_INT_IMAGE_2D]                               = { "GL_INT_IMAGE_2D", 0 };
		mapping_[GL_INT_IMAGE_3D]                               = { "GL_INT_IMAGE_3D", 0 };
		mapping_[GL_INT_IMAGE_2D_RECT]                          = { "GL_INT_IMAGE_2D_RECT", 0 };
		mapping_[GL_INT_IMAGE_CUBE]                             = { "GL_INT_IMAGE_CUBE", 0 };
		mapping_[GL_INT_IMAGE_BUFFER]                           = { "GL_INT_IMAGE_BUFFER", 0 };
		mapping_[GL_INT_IMAGE_1D_ARRAY]                         = { "GL_INT_IMAGE_1D_ARRAY", 0 };
		mapping_[GL_INT_IMAGE_2D_ARRAY]                         = { "GL_INT_IMAGE_2D_ARRAY", 0 };
		mapping_[GL_INT_IMAGE_2D_MULTISAMPLE]                   = { "GL_INT_IMAGE_2D_MULTISAMPLE", 0 };
		mapping_[GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY]             = { "GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_1D]                      = { "GL_UNSIGNED_INT_IMAGE_1D", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_2D]                      = { "GL_UNSIGNED_INT_IMAGE_2D", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_3D]                      = { "GL_UNSIGNED_INT_IMAGE_3D", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_2D_RECT]                 = { "GL_UNSIGNED_INT_IMAGE_2D_RECT", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_CUBE]                    = { "GL_UNSIGNED_INT_IMAGE_CUBE", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_BUFFER]                  = { "GL_UNSIGNED_INT_IMAGE_BUFFER", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_1D_ARRAY]                = { "GL_UNSIGNED_INT_IMAGE_1D_ARRAY", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_2D_ARRAY]                = { "GL_UNSIGNED_INT_IMAGE_2D_ARRAY", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE]          = { "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE", 0 };
		mapping_[GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY]    = { "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY", 0 };
		mapping_[GL_UNSIGNED_INT_ATOMIC_COUNTER]                = { "GL_UNSIGNED_INT_ATOMIC_COUNTER", 0 };
  }

};

// Exported singleton variable
extern const GLTypesToString& GL_TYPES_TO_STRING;

}   // namespace utils
}   // namespace picasso


#endif  // SRC_UTILS_GL_H
