/******************************************************************************
 * @file: platform.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-15
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "platform.h"
#include "utils/macros.h"

BEGIN_IGNORE_WARNINGS();
#define NOC_FILE_DIALOG_WIN32
#define NOC_FILE_DIALOG_IMPLEMENTATION
#include <external/noc_file_dialog.h>
END_IGNORE_WARNINGS();

namespace picasso {

using ::picasso::utils::Status;

StatusOr<std::string> Platform::FileDialog(const std::string&) {
  const char *filename = noc_file_dialog_open(NOC_FILE_DIALOG_OPEN,
      nullptr,
      nullptr,
      nullptr);

  if (filename) {
    return std::string(filename);
  } else {
    return { Status::STATUS_ERROR, "Could not open file" };
  }
}

}   // namespace picasso

