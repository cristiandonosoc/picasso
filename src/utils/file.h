#ifndef SRC_UTILS_FILE_H
#define SRC_UTILS_FILE_H

#include <string>

namespace picasso {
namespace utils {

// TODO(Cristian): Do path checking
std::string ReadWholeFile(const std::string& path);

}   // namespace utils
}   // namespace picasso

#endif
