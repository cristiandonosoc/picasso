/******************************************************************************
 * @file: shader_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_SHADER_REGISTRY_H
#define SRC_ASSETS_SHADER_REGISTRY_H

#include "assets/shader.h"
#include "utils/macros.h"
#include "utils/status_or.h"
#include "utils/registry.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

using ::picasso::utils::StatusOr;

namespace picasso {
namespace assets {

class ShaderRegistry : Registry<ShaderRegistry, std::string, Shader::UniquePtr> {

 public:
  static StatusOr<Shader*> CreateFromFiles(const std::string& name,
                                             const std::string& vertex_path,
                                             const std::string& fragment_path);
  static StatusOr<Shader*> Create(const std::string& name, 
                                  const std::string& vs,
                                  const std::string& fs);
  static Shader *Get(const std::string& name);

  static std::vector<Shader*> GetShaders();

 private:
  static StatusOr<Shader::UniquePtr> InternalCreate(const std::string& name,
                                                    const std::string& vs,
                                                    const std::string& fs);

};

}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_SHADER_REGISTRY_H
