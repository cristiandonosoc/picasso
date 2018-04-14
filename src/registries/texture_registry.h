/******************************************************************************
 * @file: texture_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-14
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_REGISTRIES_TEXTURE_REGISTRY_H
#define SRC_REGISTRIES_TEXTURE_REGISTRY_H

#include "utils/registry.h"
#include "assets/texture.h"

namespace picasso {
namespace registries {

using ::picasso::assets::Texture;
using ::picasso::utils::Registry;

class TextureRegistry : public Registry<TextureRegistry, Texture> {
 public:
  static StatusOr<Result> Create(const std::string& name, const std::string& path) {
    auto& instance = Instance();
    // We check if there is already a texture with that name
    auto it = texture_map_.find(name);
    if (it != texture_map_.end()) {
      return { Status::STATUS_ERROR, "Texture \"%s\" already exists!",
               name.c_str() };
    }
    
    // We attempt to create the texture
    auto res = Texture::Create(name, path);
    if (!res.Ok()) {
      return { res.GetStatus(), res.GetErrorMsg() };
    }

    // Now that we have a valid loaded texture, we can add it 
    texture_map_[name] = res.ConsumeOrDie();
    return name;      // name is the key
  }

};  // class TextureRegistry

}   // namespace registries
}   // namespace picasso

#endif  // SRC_REGISTRIES_TEXTURE_REGISTRY_H
