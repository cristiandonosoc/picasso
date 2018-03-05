#include "shaders/variable.h"

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

  if (data_.type_size_ > 0) {
    data_.backend_size_ = data_.type_size_ * data_.size_;
    data_.backend_.reset(new uint8_t[data_.backend_size_]);
  }
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



}   // namespace shaders
}   // namespace picasso
