/******************************************************************************
 * @file: platform.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-15
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: Abstract class that holds the functionality provided by the 
 *    platform. Each platform has to provided an implementation of these.
 ******************************************************************************/

#ifndef SRC_PLATFORM_H
#define SRC_PLATFORM_H

#include <string>

#include "utils/status.h"

namespace picasso {

using ::picasso::utils::StatusOr;

class Platform {
 public:
  static StatusOr<std::string> FileDialog(const std::string& title);
};

}   // namespace picasso

#endif  // SRC_PLATFORM_H
