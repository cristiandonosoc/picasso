/******************************************************************************
 * @file: texture.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-17
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <GL/gl3w.h>

#include "textures/texture.h"
#include "utils/make_unique.h"


namespace picasso {
namespace textures {

using ::picasso::utils::Status;
using ::picasso::utils::MakeUnique;

StatusOr<Texture::UniquePtr> Texture::Create(const std::string& name,
                                             const std::string& path) {
  Texture::UniquePtr texture(new Texture());  // Private constructor
  texture->name_ = name;
  // We try to load the data
  texture->image_data_.reset(stbi_load(path.c_str(), 
                             &texture->width_, &texture->height_,
                             &texture->num_channels_,
                             0));

  if (!texture->image_data_) {
    return { Status::STATUS_ERROR, 
             "Could not load texture \"%s\"", path.c_str() };
  }

  // We generate the OpenGL texture data
  glGenTextures(1, &texture->id_);
  glBindTexture(GL_TEXTURE_2D, texture->id_);
  // We send the image over
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width_, texture->height_,
               0, GL_RGB, GL_UNSIGNED_BYTE, texture->image_data_.get());
  glGenerateMipmap(GL_TEXTURE_2D);

  // Set the current texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}


}   // namesapce textures
}   // namespace picasso
