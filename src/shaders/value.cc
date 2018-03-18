/******************************************************************************
 * @file: value.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-10
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <cassert>
#include <cstring>

#include "shaders/value.h"
#include "logging/log.h"

namespace picasso {
namespace shaders {

Value::Value(const Variable *variable) : variable_(variable) {
  assert(variable);
  if (variable->GetKind() == VariableKind::UNIFORM) {
    // We allocate the amount of data needed for the value
    backend_.Reset(variable->GetTypeSize() * variable->GetSize());
    backend_.Zero();
  }
}

bool Value::SendValue() const {
  if (!variable_) {
    LOGERR_WARN("Calling SendValue on a Value without Variable");
    return false;
  }

  if (variable_->GetKind() != VariableKind::UNIFORM) {
    LOGERR_WARN("Calling SendValue on a non uniform Value");
    return false;
  }

  GLenum type = variable_->GetType();
  GLint location = variable_->GetLocation();
  GLsizei size = variable_->GetSize();

  if (size == 1) {
    switch (type) {
      case GL_FLOAT_VEC2: {
        const GLfloat *ptr = GetValue<GLfloat>();
        glUniform2f(location, *ptr, *(ptr+1)); break;
      }
      case GL_FLOAT_VEC3: {
        const GLfloat *ptr = GetValue<GLfloat>();
        glUniform3f(location, *ptr, *(ptr+1), *(ptr+2)); break;
      }
      case GL_FLOAT_VEC4: {
        const GLfloat *ptr = GetValue<GLfloat>();
        glUniform4f(location, *ptr, *(ptr+1), *(ptr+2), *(ptr+3)); break;
      }
      case GL_SAMPLER_2D:
        break;
      default: {
        LOG_WARN("Type \"%s\" is not implemented yet!", 
                    variable_->GetTypeName().c_str());
        return false;
      }
    }
  } else {
    // TODO(Cristian): Implement vector sending
    LOGERR_WARN("Vector sending is not implemented!");
    return false;
  }

  return true;
}

}   // namespace shaders 
}   // namespace picasso
