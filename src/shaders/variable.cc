#include "shaders/variable.h"

#include "logging/log.h"
#include "utils/gl.h"

namespace picasso {

using namespace ::picasso::utils;


namespace shaders {

/**
 * CONSTRUCTORS
 **/
Variable::Variable(VariableKind kind, const std::string& name, int location,
                   GLenum type, GLsizei size) {
  data_.kind_ = kind;
  data_.name_ = name;
  data_.location_ = location;
  data_.type_ = type;
  data_.size_ = size;
  // We need to get the size of the backend
  auto size_res = GL_TYPES_TO_STRING.GetSize(data_.type_);
  assert(size_res.Valid());
  data_.type_size_ = size_res.ConsumeOrDie();
  data_.type_name_ = GL_TYPES_TO_STRING.GetName(data_.type_).ConsumeOrDie();
}

Variable::Variable(Variable&& other) noexcept {
  *this = std::move(other);
}

Variable& Variable::operator=(Variable&& other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
  }
  return *this;
}

void Variable::DebugPrint(int indent) const {
  LOG_INDENT_DEBUG(indent, "Variable debug print for \"%s\"", data_.name_.c_str());
  if (data_.kind_ == VariableKind::ATTRIBUTE) {
    LOG_INDENT_DEBUG(indent, "Kind: ATTRIBUTE");
  } else if (data_.kind_ == VariableKind::UNIFORM) {
    LOG_INDENT_DEBUG(indent, "Kind: UNIFORM");
  } else {
    LOG_INDENT_DEBUG(indent, "Kind: NONE");
  }
  LOG_INDENT_DEBUG(indent, "Location: %d", data_.location_);
  LOG_INDENT_DEBUG(indent, "Type: %s", data_.type_name_.c_str());
  LOG_INDENT_DEBUG(indent, "Type Size: %zu", data_.type_size_);
  LOG_INDENT_DEBUG(indent, "Size: %d", data_.size_);
}

}   // namespace shaders
}   // namespace picasso
