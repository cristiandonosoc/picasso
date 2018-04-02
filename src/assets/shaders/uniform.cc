/******************************************************************************
 * @file: uniform.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-29
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "assets/shaders/uniform.h"
#include "utils/gl.h"
#include "logging/log.h"

namespace picasso {
namespace assets {
namespace shaders {

using ::picasso::utils::GL_TYPES_TO_STRING;

const std::map<GLenum, UniformWidget>& UNIFORM_WIDGET_MAPPING = {
  { GL_FLOAT,         UniformWidget::FLOAT  },
  { GL_FLOAT_VEC2,    UniformWidget::FLOAT_VEC2   },
  { GL_FLOAT_VEC3,    UniformWidget::FLOAT_VEC3   },
  { GL_FLOAT_VEC4,    UniformWidget::FLOAT_VEC4   },
  { GL_FLOAT_MAT2,    UniformWidget::FLOAT_MAT2   },
  { GL_FLOAT_MAT3,    UniformWidget::FLOAT_MAT3   },
  { GL_FLOAT_MAT4,    UniformWidget::FLOAT_MAT4   },
  { GL_SAMPLER_2D,    UniformWidget::TEXTURE_2D   },
};

const std::map<UniformWidget, size_t>& UNIFORM_WIDGET_SIZES = {
  { UniformWidget::NONE, 0 },
  { UniformWidget::FLOAT, sizeof(GLfloat) },
  { UniformWidget::COLOR3, 3 * sizeof(GLfloat) },
  { UniformWidget::COLOR4, 4 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_VEC2, 2 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_VEC3, 3 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_VEC4, 4 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_MAT2, 4 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_MAT3, 9 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_MAT4, 64 },
  { UniformWidget::TEXTURE_2D, 4 },
};

void UniformValue::SetValueSize() {
  backend_ = {uniform_->count * uniform_->type_size};
}

UniformValue::UniformValue(const Uniform& uniform) : uniform_(&uniform) {
  SetValueSize();
}

UniformWidget UniformValue::GetWidget() const {
  return uniform_->widget;
}

int UniformValue::GetLocation() const {
  return uniform_->location;
}


bool UniformValue::SendValue(int *texture_unit_count) const {
  if (!uniform_) {
    LOGERR_WARN("Calling SendValue on a Value without Variable");
    return false;
  }

  GLenum type = uniform_->type;
  GLint location = uniform_->location;
  size_t count = uniform_->count;

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

void UniformValue::Copy(const UniformValue& other) {
  backend_.Reset(other.backend_.Size(),
                 other.backend_.Get());

}



}   // namespace shaders
}   // namespace assets
}   // namespace picasso
