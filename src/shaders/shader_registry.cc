/******************************************************************************
 * @file: shader_registry.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "shaders/shader_registry.h"
#include "utils/file.h"

namespace picasso {

using namespace utils;

namespace shaders {

/**
 * STATIC INTERFACE
 **/
StatusOr<Shader*> ShaderRegistry::CreateFromFiles(
    const std::string& name,
    const std::string& vertex_path,
    const std::string& fragment_path) {
  std::string vs = ReadWholeFile(vertex_path);
  std::string fs = ReadWholeFile(fragment_path);

  return Create(name, vs, fs);
}

StatusOr<Shader*> ShaderRegistry::Create(const std::string& name,
                                           const std::string& vs,
                                           const std::string& fs) {
  auto& map = Instance().map_;

  // We check if it exists already
  auto it = map.find(name);
  if (it != map.end()) {
    return StatusOr<Shader*>::Error("Shader %s already exists",
                                     name.c_str());
  }

  // We attempt to create it
  auto res = Shader::Create(name, vs, fs); 
  if (!res.Ok()) { 
    return StatusOr<Shader*>::Error(res.ErrorMsg());
  }

  // Add it to the registry
  return (map[name] = res.ConsumeOrDie()).get();
}

Shader* ShaderRegistry::Get(const std::string& name) {
  auto& map = Instance().map_;
  auto it = map.find(name);
  if (it == map.end()) {
    return nullptr;
  }
  return it->second.get();

}


std::vector<Shader*> ShaderRegistry::GetShaders() {
  std::vector<Shader*> programs;
  auto& map = Instance().map_;
  programs.reserve(map.size());
  for (auto&& it : map) {
    programs.push_back(it.second.get());
  }

  return programs;
}

}   // namespace shaders
}   // namespace picasso
