/******************************************************************************
 * @file: macros.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_MACROS_H
#define SRC_UTILS_MACROS_H

#include <memory>

#define DEFINE_PTR_TYPES(class_name) \
  using UniquePtr = std::unique_ptr<class_name>; \
  using SharedPtr = std::shared_ptr<class_name>; \
  using WeakPtr = std::weak_ptr<class_name>; 

#define DISABLE_COPY(class_name) \
  class_name(const class_name&) = delete; \
  class_name& operator=(const class_name&) = delete;

#define DISABLE_MOVE(class_name) \
  class_name(class_name&&) = delete; \
  class_name& operator=(class_name&&) = delete;

#endif  // SRC_UTILS_MACROS_H
