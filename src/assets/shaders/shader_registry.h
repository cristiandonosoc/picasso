/******************************************************************************
 * @file: shader_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_SHADERS_SHADER_REGISTRY_H
#define SRC_ASSETS_SHADERS_SHADER_REGISTRY_H

#include "assets/shaders/shader.h"
#include "utils/macros.h"
#include "utils/status.h"
#include "utils/registry.h"

#include "memory/arena.h"

#include <map>
#include <memory>
#include <string>
#include <vector>


namespace picasso {
namespace assets {
namespace shaders {

using ::picasso::utils::Status;
using ::picasso::utils::StatusOr;
using ::picasso::utils::Registry;

class ShaderRegistry : public Registry<ShaderRegistry, Shader> {

 public:
  static StatusOr<Result> CreateFromFiles(const std::string& name,
                                          const std::string& vertex_path,
                                          const std::string& fragment_path);
  static StatusOr<Result> Create(const std::string& name,
                                  const std::string& vs,
                                  const std::string& fs);
 private:
  static Status LoadShader(Shader*,
                           const std::string& name,
                           const std::string& vs,
                           const std::string& fs);

};

}   // namespace shaders
}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_SHADERS_SHADER_REGISTRY_H
