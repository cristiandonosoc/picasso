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
ResultOr<Shader*> ShaderRegistry::CreateFromFiles(
    const std::string& name,
    const std::string& vertex_path,
    const std::string& fragment_path) {
  std::string vs = ReadWholeFile(vertex_path);
  std::string fs = ReadWholeFile(fragment_path);

  return Create(name, vs, fs);
}

ResultOr<Shader*> ShaderRegistry::Create(const std::string& name,
                                           const std::string& vs,
                                           const std::string& fs) {
  return Instance().InternalCreate(name, vs, fs);
}

Shader* ShaderRegistry::Get(const std::string& name) {
  return Instance().InternalGet(name);
}

/**
 * INSTANCE INTERFACE
 **/
ResultOr<Shader*> ShaderRegistry::InternalCreate(const std::string& name,
                                                   const std::string& vs,
                                                   const std::string& fs) {
  // We check if it exists already
  auto it = program_map_.find(name);
  if (it != program_map_.end()) {
    return ResultOr<Shader*>::Error("Shader %s already exists",
                                     name.c_str());
  }

  // We attempt to create it
  auto res = Shader::Create(name, vs, fs); 
  if (!res.Valid()) { 
    return ResultOr<Shader*>::Error(res.ErrorMsg());
  }

  // Add it to the registry
  return (program_map_[name] = res.ConsumeOrDie()).get();
}

Shader *ShaderRegistry::InternalGet(const std::string& name) const {
  auto it = program_map_.find(name);
  if (it == program_map_.end()) {
    return nullptr;
  }
  return it->second.get();
}


}   // namespace shaders
}   // namespace picasso
