/******************************************************************************
 * @file: uniform_value.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "shaders/uniform_value.h"
#include "logging/log.h"

namespace picasso {
namespace shaders {

UniformValue::UniformValue(const Uniform *uniform) : uniform_(uniform) {
  assert(uniform);
  // We allocate the amount of data needed for the value
  if (uniform->type_size == 0) {
    LOGERR_FATAL("Attempting to allocate a variable (%s) with type size 0.\n"
                 "This is most probably that the size if not set in utils/gl.h",
                 uniform->name.c_str());
  }
  backend_.Reset(uniform->count * uniform->type_size);
  // We zero that memory
  backend_.Zero();
}

bool UniformValue::SendValue(int *texture_unit_count) const {
  if (!uniform_) {
    LOGERR_WARN("Calling SendValue on a Value without Variable");
    return false;
  }

  GLenum type = uniform_->type;
  GLint location = uniform_->location;
  GLsizei count = uniform_->count;

  if (count == 1) {
    switch (type) {
      case GL_FLOAT: {
        const GLfloat *ptr = GetValue<GLfloat>();
        glUniform1f(location, *ptr);
        break;
      }
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
      case GL_FLOAT_MAT4: {
        /* const GLfloat *ptr = GetValue<GLfloat>(); */
        /* glUniformMatrix4fv(location, 1, false, ptr); */
		break;
      }
      case GL_SAMPLER_2D: {
        const GLuint *texture_id = GetValue<uint32_t>();
        glActiveTexture(GL_TEXTURE0 + *texture_unit_count);
        glBindTexture(GL_TEXTURE_2D, *texture_id);
        glUniform1i(location, *texture_unit_count);
        (*texture_unit_count)++;
        // We set the location
        break;
      }
      default: {
        LOG_WARN("Type \"%s\" is not implemented yet!", uniform_->type_name.c_str());
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
