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

// Useful for combining names with __LINE__, for automatic variable name
// creation
#define _COMBINE(x, y) x##y
#define COMBINE(x, y) _COMBINE(x, y)

// Setup printf formatting for the formatting function
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#define PRINTF_FORMAT_ATTRIBUTE(fmt_one_index, varargs_one_index) \
  __attribute__ ((format (printf, fmt_one_index, varargs_one_index)))
#else
#define PRINTF_FORMAT_ATTRIBUTE(ignore1, ignore2)
#endif



// Ignore warnings for Windows because they don't have a good way
// of ignoring warnings for certain includes
// (I mean, they did add it in 2018...)
#ifdef _WIN32
#define BEGIN_IGNORE_WARNINGS() __pragma(warning(push, 0))
#define END_IGNORE_WARNINGS() __pragma(warning(pop))
#else
#define BEGIN_IGNORE_WARNINGS() 
#define END_IGNORE_WARNINGS() 
#endif


#define IGNORE_WARNINGS_BEGIN

#define DEFINE_PTR_TYPES(class_name) \
  using UniquePtr = std::unique_ptr<class_name>; \
  using SharedPtr = std::shared_ptr<class_name>; \
  using WeakPtr = std::weak_ptr<class_name>;

#define DEFAULT_COPY(class_name) \
  class_name(const class_name&) = default; \
  class_name& operator=(const class_name&) = default;

#define DEFAULT_MOVE(class_name) \
  class_name(class_name&&) = default; \
  class_name& operator=(class_name&&) = default;

#define DISABLE_COPY(class_name) \
  class_name(const class_name&) = delete; \
  class_name& operator=(const class_name&) = delete;

#define DISABLE_MOVE(class_name) \
  class_name(class_name&&) = delete; \
  class_name& operator=(class_name&&) = delete;

#define DECLARE_NOEXCEPT_MOVE(class_name) \
  class_name(class_name&&) noexcept; \
  class_name& operator=(class_name&&) noexcept;

#endif  // SRC_UTILS_MACROS_H
