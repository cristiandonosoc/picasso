/******************************************************************************
 * @file: string.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-27
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_STRING_H
#define SRC_UTILS_STRING_H

#include <cstdarg>
#include <string>

#include "utils/macros.h"
#include "utils/snprintf.h"

namespace picasso {
namespace utils {

template <size_t BUFFER_SIZE = 1024>
PRINTF_FORMAT_ATTRIBUTE(1, 2)
std::string FormattedString(const char *fmt, ...) {
  va_list arglist;                                    
  va_start(arglist, fmt);                            
  char buffer[BUFFER_SIZE];                                    
  vsnprintf(buffer, sizeof(buffer), fmt, arglist);      
  va_end(arglist);
  return buffer;
}

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_STRING_H
