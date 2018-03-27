/******************************************************************************
 * @file: texture.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_TEXTURE_H
#define SRC_ASSETS_TEXTURE_H

#include <memory>
#include <external/stb_image.h>

#include "utils/macros.h"
#include "utils/status.h"

using ::picasso::utils::StatusOr;

namespace picasso {
namespace assets {

class Texture {
 public:
  DEFINE_PTR_TYPES(Texture);
  class ImageFreer {
   public:
    void operator()(uint8_t* ptr) { stbi_image_free(ptr); }
  };
  using ImageData = std::unique_ptr<uint8_t, ImageFreer>;

 private:
  static StatusOr<Texture::UniquePtr> Create(const std::string& name,
                                             const std::string& path);

 private:
  Texture() = default;
  DISABLE_COPY(Texture);
  DISABLE_MOVE(Texture);

 public:
  const std::string& GetName() const { return name_; }
  const std::string& GetPath() const { return path_; }
  const uint8_t *GetData() const { return image_data_.get(); }
  int GetWidth() const { return width_; }
  int GetHeight() const { return height_; }
  int GetNumChannels() const { return num_channels_; }
  uint32_t GetId() const { return id_; }


 private:
  std::string name_;
  std::string path_;
  ImageData image_data_; 
  int width_ = 0;
  int height_ = 0;
  int num_channels_ = 0;
  uint32_t id_ = 0;

 public:
  friend class TextureRegistry;
};  // class Texture

}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_TEXTURE_H
