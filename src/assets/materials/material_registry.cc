/******************************************************************************
 * @file: material_registry.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "assets/materials/material_registry.h"

namespace picasso {
namespace assets {
namespace materials {

StatusOr<Material*> MaterialRegistry::Create(const std::string& name) {
  auto res = Register(name);
  if (!res.Ok()) {
    return res;
  }
  Material *material = res.ConsumeOrDie();
  material->name_ = name;
  return material;
}

StatusOr<Material*> MaterialRegistry::Get(const MaterialRegistry::KeyType& name) {
  auto& map = Instance().map_;
  auto it = map.find(name);
  if (it == map.end()) {
    return FILENO_STATUS(Status::STATUS_ERROR, "Cannot find key \"%s\"",
                         name.c_str());
  }
  return it->second.get();
}

const MaterialRegistry::RegistryMapType& MaterialRegistry::GetMaterials() {
  return Instance().map_;
}

}   // namespace materials
}   // namespace assets
}   // namespace picasso
