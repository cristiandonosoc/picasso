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

namespace picasso {
namespace assets {
namespace shaders {

std::map<GLenum, UniformWidget> UNIFORM_WIDGET_MAPPING = {
  { GL_FLOAT,         UniformWidget::FLOAT  },
  { GL_FLOAT_VEC2,    UniformWidget::FLOAT_VEC2   },
  { GL_FLOAT_VEC3,    UniformWidget::FLOAT_VEC3   },
  { GL_FLOAT_VEC4,    UniformWidget::FLOAT_VEC4   },
  { GL_SAMPLER_2D,    UniformWidget::TEXTURE_2D   },
};

}   // namespace shaders
}   // namespace assets
}   // namespace picasso
