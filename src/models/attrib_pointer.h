/******************************************************************************
 * @file: attrib_pointer.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-11
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: 
 * Describe how to obtain a sample ofan attribute for each vertex run.
 * Using the AttributeKind enum the model will be able to correctly
 * associate this information to the corresponding location
 ******************************************************************************/

#ifndef SRC_MODELS_ATTRIB_POINTER_H
#define SRC_MODELS_ATTRIB_POINTER_H

#include <GL/gl3w.h>
#include "utils/log.h"
#include "utils/gl.h"

namespace picasso {
namespace models {

using ::picasso::utils::GL_TYPES_TO_STRING;

// TODO(Cristian): Find out how to bind arbitraty points
enum AttributeKind {
  VERTEX,
  COLOR, 
  NORMAL,
  UV,
};

class AttributePointer {
 public:
  AttributePointer() = default;
  AttributePointer(AttributeKind kind, GLint size, GLenum type, GLboolean normalize, 
                   GLsizei stride, GLsizei offset) 
    : kind_(kind), size_(size), type_(type), normalize_(normalize), 
      stride_(stride), offset_(offset) {
    type_size_ = GL_TYPES_TO_STRING.GetSize(type).ConsumeOrDie();
  }

 public:
  AttributeKind GetKind() const { return kind_; }
  GLint GetSize() const { return size_; }
  GLenum GetType() const { return type_; }
  GLsizei GetTypeSize() const { return type_size_; }
  GLsizei GetStride() const { return stride_; }
  GLsizei GetOffset() const { return offset_; }
  GLboolean GetNormalize() const { return normalize_; }
  void *GetGLOffset() const {
    return (void*)(size_t)(offset_ * type_size_);
  }

 private:
  AttributeKind kind_;
  GLint size_;
  GLenum type_;
  GLsizei type_size_;
  GLboolean normalize_;
  GLsizei stride_;
  GLsizei offset_;

 public:
  void DebugPrint(int indent = 0) const {
    LOGERR_INDENT_DEBUG(indent, "Debug print of AttributePointer");
    LOGERR_INDENT_DEBUG(indent, "KIND: %s", GetKind() == AttributeKind::VERTEX ? "VERTEX" : "NON-VERTEX");
    LOGERR_INDENT_DEBUG(indent, "SIZE: %d", GetSize());
    LOGERR_INDENT_DEBUG(indent, "TYPE %s", GL_TYPES_TO_STRING.GetName(GetType()).ConsumeOrDie().c_str());
    LOGERR_INDENT_DEBUG(indent, "TYPE SIZE: %d", GetTypeSize());
    LOGERR_INDENT_DEBUG(indent, "NORMALIZE: %s",  GetNormalize() ? "true" : "false");
    LOGERR_INDENT_DEBUG(indent, "STRIDE: %d", GetStride());
    LOGERR_INDENT_DEBUG(indent, "OFFSET: %d", GetOffset());
    LOGERR_INDENT_DEBUG(indent, "GL_OFFSET: %d", GetOffset() * GetTypeSize());
  }
};  // class AttributePointer

}   // namespace models
}   // namespace picasso

#endif  // SRC_MODELS_ATTRIB_POINTER_H
