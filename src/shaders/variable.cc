#include "shaders/variable.h"

#include "utils/gl.h"
#include "utils/log.h"

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


std::string Variable::GetTypeName() const {
  auto res = GL_TYPES_TO_STRING.GetName(data_.type_);
  if (!res.Valid()) {
    return "INVALID";
  }
  return res.ConsumeOrDie();
}


void Variable::DebugPrint(int indent) const {
  LOGERR_INDENT_DEBUG(indent, "Variable debug print for \"%s\"", data_.name_.c_str());
  if (data_.kind_ == VariableKind::ATTRIBUTE) {
    LOGERR_INDENT_DEBUG(indent, "Kind: ATTRIBUTE");
  } else if (data_.kind_ == VariableKind::UNIFORM) {
    LOGERR_INDENT_DEBUG(indent, "Kind: UNIFORM");
  } else {
    LOGERR_INDENT_DEBUG(indent, "Kind: NONE");
  }
  LOGERR_INDENT_DEBUG(indent, "Location: %d", data_.location_);
  LOGERR_INDENT_DEBUG(indent, "Type: %s", GL_TYPES_TO_STRING.GetName(data_.type_).ConsumeOrDie().c_str());
  LOGERR_INDENT_DEBUG(indent, "Type Size: %zu", data_.type_size_);
  LOGERR_INDENT_DEBUG(indent, "Size: %d", data_.size_);
}

}   // namespace shaders
}   // namespace picasso
