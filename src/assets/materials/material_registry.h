/******************************************************************************
 * @file: material_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_SHADERS_MATERIAL_REGISTRY_H
#define SRC_SHADERS_MATERIAL_REGISTRY_H

#include "assets/materials/material.h"
#include "utils/macros.h"
#include "utils/status.h"
#include "utils/registry.h"

#include <map>
#include <vector>

namespace picasso {
namespace assets {
namespace materials {

using ::picasso::utils::StatusOr;
using ::picasso::utils::Registry;

class MaterialRegistry : public Registry<MaterialRegistry, std::string, Material::UniquePtr> {
 public:
  static StatusOr<KeyType> Create(const std::string& name);
  static Material *Get(const KeyType& key);

  // IMPORTANT: The pointers are not assured to be valid 
  //            through time, so they should be *always*
  //            be obtained through here
  // TODO(Cristian): Use shared pointer to represent this relationship
  static const RegistryMapType& GetMaterials();

 private:
  static StatusOr<Material::UniquePtr> InternalCreate(const std::string& name);
};

}   // namespace materials
}   // namespace assets
}   // namespace picasso

#endif  // SRC_SHADERS_MATERIAL_REGISTRY_H
