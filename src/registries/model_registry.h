/******************************************************************************
 * @file: model_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-13
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_REGISTRIES_MODEL_REGISTRY_H
#define SRC_REGISTRIES_MODEL_REGISTRY_H

#include "utils/registry.h"
#include "models/model.h"
#include "memory/arena.h"
#include "utils/status.h"

namespace picasso { 
namespace registries {

using ::picasso::models::Model;
using ::picasso::utils::Registry;
using ::picasso::memory::ArenaAllocator;
using ::picasso::utils::StatusOr;

class ModelRegistry : public Registry<ModelRegistry, std::string, Model,
                                      ArenaAllocator<Model, 1024>> {
 public:
  static StatusOr<Model*> Create(const std::string& name) {
    return Register(name);
  }

};  // class ModelRegistry

}   // namespace registries
}   // namespace picasso

#endif  // SRC_REGISTRIES_MODEL_REGISTRY_H
