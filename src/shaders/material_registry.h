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
#include "utils/status_or.h"
#include "utils/singleton.h"

#include <map>
#include <vector>

namespace picasso {

using namespace utils; 

namespace shaders {

class MaterialRegistry : Singleton<MaterialRegistry> {
 public:
  using Key = std::string;
  using MaterialMap = std::map<Key, Material::UniquePtr>;

 private:
  MaterialRegistry() = default;
  DISABLE_COPY(MaterialRegistry);
  DISABLE_MOVE(MaterialRegistry);

 public:
  static StatusOr<Key> Create(const std::string& name);
  static Material *Get(const Key& key);

  // IMPORTANT: The pointers are not assured to be valid 
  //            through time, so they should be *always*
  //            be obtained through here
  // TODO(Cristian): Use shared pointer to represent this relationship
  static const MaterialMap& GetMaterials() {
    return Instance().InternalGetMaterials();
  }

 protected:
  StatusOr<Key> InternalCreate(const std::string& name);
  Material *InternalGet(const std::string& name) const;

  /* std::map<Key, Material*> InternalGetMaterials() const { */
  /*   std::map<Key, Material*> materials; */
  /*   for (auto&& it : material_map_) { */
  /*     materials[it.first] = it.second.get(); */
  /*   } */
  /*   return materials; */
  /* } */
  const MaterialMap& InternalGetMaterials() const {
    return material_map_;
  }

 private:
  MaterialMap material_map_;

 public:
  friend class Singleton<MaterialRegistry>;
};

}   // namespace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_MATERIAL_REGISTRY_H
