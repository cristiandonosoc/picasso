/******************************************************************************
 * @file: mesh_registry.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-02
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "assets/mesh_registry.h"

namespace picasso {
namespace assets {

StatusOr<Mesh*> MeshRegistry::Create(const std::string& name) {
  auto res = Register(name);
  RETURN_OR_ASSIGN(mesh, res);
  // Mesh initialization goes here
  return mesh;
}


}   // namespace assets
}   // namespace picasso
