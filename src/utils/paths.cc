/******************************************************************************
 * @file: paths.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-13
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#pragma warning(push, 0)
#include <DG_misc.h>
#pragma warning(push)

#include "utils/paths.h"

namespace picasso {
namespace utils {
namespace paths {

namespace { 

class PathHolder {
  // Interface
 public:
  static const std::string& ExecutableDir() {
    return Instance().executable_dir;
  }

 private:
  static const PathHolder& Instance() {
    static PathHolder instance;
    return instance;
  }

 private:
  PathHolder() {
    // Obtain once
    executable_dir = DG_GetExecutableDir();
  }


 private:
  std::string executable_dir;
};


};

const std::string& GetExecutableDir() {
  return PathHolder::ExecutableDir();
}

}   // namespace paths
}   // namespace utils
}   // namespace picasso

