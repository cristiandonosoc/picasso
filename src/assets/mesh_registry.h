/******************************************************************************
 * @file: mesh_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-02
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_MESH_REGISTRY_H
#define SRC_ASSETS_MESH_REGISTRY_H

#include "assets/mesh.h"
#include "memory/arena.h"
#include "utils/registry.h"
#include "utils/status.h"

namespace picasso {
namespace assets {

using ::picasso::assets::Mesh;
using ::picasso::memory::ArenaAllocator;
using ::picasso::utils::Registry;
using ::picasso::utils::StatusOr;

class MeshRegistry : public Registry<MeshRegistry, std::string, Mesh,
                                     ArenaAllocator<Mesh, 1024>> {
 public:
  static StatusOr<Mesh*> Create(const std::string& name);

};  // class MeshRegistry

}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_MESH_REGISTRY_H
