/******************************************************************************
 * @file: program_registry.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "shaders/program_registry.h"
#include "utils/file.h"

namespace picasso {

using namespace utils;

namespace shaders {

ResultOr<Program::WeakPtr> ProgramRegistry::CreateFromFiles(
    const std::string& name,
    const std::string& vertex_path,
    const std::string& fragment_path) {
  std::string vs = ReadWholeFile(vertex_path);
  std::string fs = ReadWholeFile(fragment_path);

  return CreateProgram(name, vs, fs);
}
ResultOr<Program::WeakPtr> ProgramRegistry::CreateProgram(
    const std::string& name,
    const std::string& vs,
    const std::string& fs) {
  // We check if it exists already
  auto it = program_map_.find(name);
  if (it != program_map_.end()) {
    return ResultOr<Program::WeakPtr>::Error("Program %s already exists",
                                             name.c_str());
  }

  // We attempt to create it
  auto res = Program::Create(vs, fs); 
  if (!res.Valid()) { 
    return ResultOr<Program::WeakPtr>::Error(res.ErrorMsg());
  }

  // Promote
  Program::SharedPtr shared_ptr(res.ConsumeOrDie());
  // Add it to the registry
  program_map_[name] = shared_ptr;
  
  return Program::WeakPtr(shared_ptr);
}

Program::WeakPtr ProgramRegistry::Get(const std::string& name) const {
  auto it = program_map_.find(name);
  if (it == program_map_.end()) {
    return Program::WeakPtr();
  }
  return it->second;
}


}   // namespace shaders
}   // namespace picasso
