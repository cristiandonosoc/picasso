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

#include "shaders/material.h"
#include "utils/macros.h"
#include "utils/result.h"
#include "utils/singleton.h"

namespace picasso {

using namespace utils; 

namespace shaders {

class MaterialRegistry : public Singleton<MaterialRegistry> {
 private:
  MaterialRegistry() = default;
  DISABLE_COPY(MaterialRegistry);
  DISABLE_MOVE(MaterialRegistry);

 public:
  ResultOr<Material*> Create(const std::string& name);

  Material *Get(const std::string& name) const;
  Material *operator[](const std::string& name) const {
    return Get(name);
  }

 private:
  std::map<std::string, Material::UniquePtr> material_map_;

 public:
  friend class Singleton<MaterialRegistry>;
};

}   // namespace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_MATERIAL_REGISTRY_H
