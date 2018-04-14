/******************************************************************************
 * @file: material_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-13
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_REGISTRIES_MATERIAL_REGISTRY_H
#define SRC_REGISTRIES_MATERIAL_REGISTRY_H

#include "utils/registry.h"
#include "utils/status.h"
#include "assets/materials/material.h"

namespace picasso {
namespace registries {

using ::picasso::assets::materials::Material;
using ::picasso::utils::Registry;
using ::picasso::utils::StatusOr;

class MaterialRegistry : public Registry<MaterialRegistry, Material> {
 public:
   static StatusOr<Result> Create(const std::string& name) {
     return Register(name);
   }
};  // class MaterialRegistry

}   // namespace registries
}   // namespace picasso

#endif  // SRC_REGISTRIES_MATERIAL_REGISTRY_H
