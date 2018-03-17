/******************************************************************************
 * @file: texture_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-17
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_TEXTURES_TEXTURE_REGISTRY_H
#define SRC_TEXTURES_TEXTURE_REGISTRY_H

#include <map>

#include "textures/texture.h"
#include "utils/singleton.h"
#include "utils/macros.h"
#include "utils/status_or.h"

namespace picasso {
namespace textures {

using ::picasso::utils::Singleton;
using ::picasso::utils::StatusOr;


class TextureRegistry : Singleton<TextureRegistry> {
 public:
  using Key = std::string;
  using TextureMap = std::map<Key, Texture::SharedPtr>;
 private:
  TextureRegistry() = default;
  DISABLE_COPY(TextureRegistry);
  DISABLE_MOVE(TextureRegistry);

 public:
  static StatusOr<Key> Create(const std::string& name, const std::string& path);
  static Texture::WeakPtr Get(const Key& key);
  static const TextureMap& GetTextureMap();

 private:
  TextureMap texture_map_;

 protected:
  StatusOr<Key> InternalCreate(const std::string& name, const std::string& path);
 
 public:
  friend class ::picasso::utils::Singleton<TextureRegistry>;
};  // class TextureRegistry

}   // namespace textures
}   // namespace picasso

#endif  // SRC_TEXTURES_TEXTURE_REGISTRY_H
