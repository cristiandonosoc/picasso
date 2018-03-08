/******************************************************************************
 * @file: material_registry.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "shaders/material_registry.h"

namespace picasso {
namespace shaders {

ResultOr<Material*> MaterialRegistry::Create(const std::string& name) {
  return Instance().InternalCreate(name);
}

Material *MaterialRegistry::Get(const std::string& name) {
  return Instance().InternalGet(name);
}


ResultOr<Material*> MaterialRegistry::InternalCreate(const std::string& name) {
  auto it = material_map_.find(name);
  if (it != material_map_.end()) {
    return ResultOr<Material*>::Error("Material %s already exists",
                                      name.c_str());
  }

  // Attempt to create 
  auto res = Material::Create(name);
  if (!res.Valid()) {
    return ResultOr<Material*>::Error(res.ErrorMsg());
  }

  return (material_map_[name] = res.ConsumeOrDie()).get();
}

Material *MaterialRegistry::InternalGet(const std::string& name) const {
  auto it = material_map_.find(name);
  if (it == material_map_.end()) {
    return nullptr;
  }
  return it->second.get();
}

}   // namespace shaders
}   // namespace picasso

