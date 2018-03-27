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

StatusOr<MaterialRegistry::KeyType> MaterialRegistry::Create(const std::string& name) {
  auto& map = Instance().map_;
  auto it = map.find(name);
  if (it != map.end()) {
    return { Status::STATUS_ERROR, "Material \"%s\" already exists", 
                                   name.c_str() };
  }

  // Attempt to create 
  auto res = InternalCreate(name);
  if (!res.Ok()) {
    return { Status::STATUS_ERROR, res.ErrorMsg() };
  }
  map[name] = res.ConsumeOrDie();
  return name;
}

Material *MaterialRegistry::Get(const std::string& name) {
  auto& map = Instance().map_;
  auto it = map.find(name);
  if (it == map.end()) {
    return nullptr;
  }
  return it->second.get();
}

const MaterialRegistry::RegistryMapType& MaterialRegistry::GetMaterials() {
  return Instance().map_;
}

StatusOr<Material::UniquePtr> MaterialRegistry::InternalCreate(const std::string& name) {
  Material::UniquePtr material(new Material());   // Private Constructor
  material->name_ = name;
  return material;
}

}   // namespace materials
}   // namespace assets
}   // namespace picasso
