
#include "platform.h"

namespace picasso {

using ::picasso::utils::Status;

StatusOr<std::string> Platform::FileDialog(const std::string&) {
  return FILENO_STATUS(Status::STATUS_ERROR,
                       "Platform function not implemented yet.");
}

}   // namespace picasso
