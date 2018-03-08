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

#include <map>
#include <vector>

namespace picasso {

using namespace utils; 

namespace shaders {

class MaterialRegistry : Singleton<MaterialRegistry> {
 private:
  MaterialRegistry() = default;
  DISABLE_COPY(MaterialRegistry);
  DISABLE_MOVE(MaterialRegistry);

 public:
  static ResultOr<Material*> Create(const std::string& name);
  static Material *Get(const std::string& name);

  static std::vector<Material*> GetMaterials() {
    return Instance().InternalGetMaterials();
  }

 protected:
  ResultOr<Material*> InternalCreate(const std::string& name);
  Material *InternalGet(const std::string& name) const;

  std::vector<Material*> InternalGetMaterials() const {
    std::vector<Material*> materials;
    materials.reserve(material_map_.size());
    for (auto&& it : material_map_) {
      materials.push_back(it.second.get());
    }

    return materials;
  }

 private:
  std::map<std::string, Material::UniquePtr> material_map_;

 public:
  friend class Singleton<MaterialRegistry>;
};

}   // namespace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_MATERIAL_REGISTRY_H
