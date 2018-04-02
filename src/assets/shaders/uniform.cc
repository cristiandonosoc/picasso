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
  { UniformWidget::COLOR, 4 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_VEC2, 2 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_VEC3, 3 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_VEC4, 4 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_MAT2, 4 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_MAT3, 9 * sizeof(GLfloat) },
  { UniformWidget::FLOAT_MAT4, 64 },
  { UniformWidget::TEXTURE_2D, 4 },
};

void UniformValue::SetValueSize(const Uniform& uniform) {
  backend_ = {uniform.count * uniform.type_size};
}

}   // namespace shaders
}   // namespace assets
}   // namespace picasso
