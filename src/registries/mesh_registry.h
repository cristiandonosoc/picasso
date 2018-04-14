/******************************************************************************
 * @file: mesh_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-14
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_REGISTRIES_MESH_REGISTRY_H
#define SRC_REGISTRIES_MESH_REGISTRY_H

#include <string>
#include <map>

#include "assets/mesh.h"
#include "utils/registry.h"
#include "utils/status.h"
#include "logging/log.h"

namespace picasso {
namespace registries {

using ::picasso::assets::Mesh;
using ::picasso::utils::Registry;
using ::picasso::utils::Status;
using ::picasso::utils::StatusOr;

class MeshRegistry : public Registry<MeshRegistry, Mesh> {
 public:
  static StatusOr<Result> Create(const std::string& name) {
    auto register_res = Register(name);
    RETURN_OR_ASSIGN(res, register_res);

    auto& instance = Instance();
    auto it = instance.name_map_.find(name);
    if (it != instance.name_map_.end()) {
      LOG_WARN("OVERWRITING MESH \"%s\"", name.c_str());
    }
    instance.name_map_[name] = res.key;
    return res;
  }

  static StatusOr<Result> GetByName(const std::string& name) {
    auto& instance = Instance();
    auto it = instance.name_map_.find(name);
    if (it == instance.name_map_.end()) {
      return { Status::STATUS_FAILED,
               "Cannot find mesh with name \"%s\"",  name.c_str() };
    }
    return Get(instance.name_map_[name]);
  }

 private:
  std::map<std::string, KeyType> name_map_;
};  // class MeshRegistry


}   // namespace registries
}   // namespace picasso

#endif  // SRC_REGISTRIES_MESH_REGISTRY_H
