/******************************************************************************
 * @file: program_registry.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_SHADERS_PROGRAM_REGISTRY_H
#define SRC_SHADERS_PROGRAM_REGISTRY_H

#include "shaders/program.h"
#include "utils/macros.h"
#include "utils/result.h"

#include <map>
#include <memory>
#include <string>

namespace picasso {

using namespace utils;

namespace shaders {

class ProgramRegistry {
 public:
  static ProgramRegistry& Instance() {
    static ProgramRegistry instance;
    return instance;
  }

 private:
  ProgramRegistry() {}
  DISABLE_COPY(ProgramRegistry);
  DISABLE_MOVE(ProgramRegistry);

 public:
  ResultOr<Program::WeakPtr> CreateFromFiles(const std::string& name,
                                             const std::string& vertex_path,
                                             const std::string& fragment_path);
  ResultOr<Program::WeakPtr> CreateProgram(const std::string& name, 
                                           const std::string& vs,
                                           const std::string& fs);

  Program::WeakPtr Get(const std::string& name) const;
  Program::WeakPtr operator[](const std::string& name) const {
    return Get(name);
  }

 private:
  std::map<std::string, std::shared_ptr<Program>> program_map_;
};

}   // namespace shaders
}   // namespace picasso



#endif  // SRC_SHADERS_PROGRAM_REGISTRY_H
