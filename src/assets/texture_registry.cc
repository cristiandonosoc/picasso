/******************************************************************************
 * @file: texture_registry.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-17
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "assets/texture_registry.h"

using ::picasso::utils::Status;

namespace picasso {
namespace assets {

StatusOr<TextureRegistry::Key> TextureRegistry::Create(const std::string& name, 
                                                       const std::string& path) {
  return Instance().InternalCreate(name, path);
}

StatusOr<TextureRegistry::Key> TextureRegistry::InternalCreate(const std::string& name, 
                                                               const std::string& path) {
  // We check if there is already a texture with that name
  auto it = texture_map_.find(name);
  if (it != texture_map_.end()) {
    return { Status::STATUS_ERROR, "Texture \"%s\" already exists!",
             name.c_str() };
  }
  
  // We attempt to create the texture
  auto res = Texture::Create(name, path);
  if (!res.Ok()) {
    return { res.GetStatus(), res.ErrorMsg() };
  }

  // Now that we have a valid loaded texture, we can add it 
  texture_map_[name] = res.ConsumeOrDie();
  return name;      // name is the key
}

Texture::WeakPtr TextureRegistry::Get(const Key& key) {
  auto&& instance = Instance();
  auto it = instance.texture_map_.find(key);
  if (it == instance.texture_map_.end()) {
    return Texture::WeakPtr();  // weak_ptr equivalent to nullptr
  }
  return it->second;
}

const TextureRegistry::TextureMap& TextureRegistry::GetTextureMap() {
  return Instance().texture_map_;
}

}   // namespace assets
}   // namespace picasso
