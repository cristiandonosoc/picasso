#ifndef SRC_UTILS_SNPRINTF_H
#define SRC_UTILS_SNPRINTF_H

#include <cstddef>

namespace picasso {
namespace utils {

/**
 * This aims to be a fix for the problem that MSVC
 * doesn't support standards in terms of printf commands,
 * so we need to support our own wrapper
 * The implementation will define which version it needs.
 * TODO(Cristian): Move this to our own platform code.
 */
int picasso_snprintf(char *buf, size_t size, const char *fmt, ...);

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_SNPRINTF_H
