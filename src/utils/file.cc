#include "file.h"

#include <fstream>
#include <sstream>

namespace picasso {
namespace utils {

std::string ReadWholeFile(const std::string& path) {
  std::ifstream t(path);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

}   // namespace utils
}   // namespace picasso
