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
#include "utils/singleton.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace picasso {

using namespace utils;

namespace shaders {

using ProgramMap = std::map<std::string, Program::UniquePtr>;

class ProgramRegistry : Singleton<ProgramRegistry> {

 private:
  ProgramRegistry() = default;
  DISABLE_COPY(ProgramRegistry);
  DISABLE_MOVE(ProgramRegistry);


 public:
  static ResultOr<Program*> CreateFromFiles(const std::string& name,
                                             const std::string& vertex_path,
                                             const std::string& fragment_path);
  static ResultOr<Program*> Create(const std::string& name, 
                                           const std::string& vs,
                                           const std::string& fs);
  static Program *Get(const std::string& name);

  static std::vector<Program*> GetPrograms() {
    return Instance().InternalGetPrograms();
  }

 protected:
  ResultOr<Program*> InternalCreate(const std::string& name,
                                    const std::string& vs,
                                    const std::string& fs);
  Program *InternalGet(const std::string& name) const;

  std::vector<Program*> InternalGetPrograms() const {
    std::vector<Program*> programs;
    programs.reserve(program_map_.size());
    for (auto&& it : program_map_) {
      programs.push_back(it.second.get());
    }

    return programs;
  }

 private:
  std::map<std::string, Program::UniquePtr> program_map_;


 public:
  friend class Singleton<ProgramRegistry>;
};

}   // namespace shaders
}   // namespace picasso



#endif  // SRC_SHADERS_PROGRAM_REGISTRY_H
