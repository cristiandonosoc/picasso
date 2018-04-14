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
using ::picasso::utils::StatusOr;

class ModelRegistry : public Registry<ModelRegistry, Model> {
 public:
  static StatusOr<Result> Create(const std::string& name) {
    auto res = Register();
    RETURN_OR_ASSIGN(pair, res);
    pair.value->name = name;
    return pair;
  }
};  // class ModelRegistry

}   // namespace registries
}   // namespace picasso

#endif  // SRC_REGISTRIES_MODEL_REGISTRY_H
