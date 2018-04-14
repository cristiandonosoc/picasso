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


namespace picasso {
namespace assets {
namespace materials {

using ::picasso::utils::StatusOr;
using ::picasso::utils::Registry;

class MaterialRegistry : public Registry<MaterialRegistry, Material> {
 public:
  static StatusOr<Result> Create(const std::string& name) {
    return Register(name);
  }
};  // class MaterialRegistry

}   // namespace materials
}   // namespace assets
}   // namespace picasso

#endif  // SRC_SHADERS_MATERIAL_REGISTRY_H
