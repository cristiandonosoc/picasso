/******************************************************************************
 * @file: header_only_impl.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-13
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: Implementation of the header only libraries
 ******************************************************************************/

#include "utils/macros.h"

// No warnings
BEGIN_IGNORE_WARNINGS();

// DG_MISC
#define DG_MISC_IMPLEMENTATION
#include <external/DG_misc.h>

END_IGNORE_WARNINGS();
