/******************************************************************************
 * @file: glm.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-04
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_GLM_H
#define SRC_UTILS_GLM_H

// NOTE(donosoc): This is so that we don't have to add the
//                f*cking BEGIN/END WARNING block everytime
//                we want the glm

#include "utils/macros.h"

BEGIN_IGNORE_WARNINGS();
#include <glm/glm.hpp>
#ifdef GLM_IMPORT_MATRIX_TRANSFORM
#include <glm/gtc/matrix_transform.hpp>
#endif
#ifdef GLM_IMPORT_TYPE_PTR
#include <glm/gtc/type_ptr.hpp>
#endif
END_IGNORE_WARNINGS();

#endif  // SRC_UTILS_GLM_H
