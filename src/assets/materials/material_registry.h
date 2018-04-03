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

#include "memory/arena.h"

#include <map>
#include <vector>

namespace picasso {
namespace assets {
namespace materials {

using ::picasso::utils::StatusOr;
using ::picasso::utils::Registry;

using ::picasso::memory::ArenaAllocator;

class MaterialRegistry : public Registry<MaterialRegistry, std::string, Material,
                                         ArenaAllocator<Material, 1024>> {
 public:
  static StatusOr<Material*> Create(const std::string& name);

 private:
  static StatusOr<Material::UniquePtr> InternalCreate(const std::string& name);
};

}   // namespace materials
}   // namespace assets
}   // namespace picasso

#endif  // SRC_SHADERS_MATERIAL_REGISTRY_H
