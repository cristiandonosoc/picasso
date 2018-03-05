#ifndef SRC_SHADERS_VARIABLE_H
#define SRC_SHADERS_VARIABLE_H


#include <cstdint>
#include <GL/gl3w.h>
#include <initializer_list>
#include <memory>
#include <string>

namespace picasso {
namespace shaders {

enum VariableKind {
  NONE,
  ATTRIBUTE,
  UNIFORM,
};

/**
 * Variable
 * ---------------
 *
 * Represents the information of either a GL attribute of uniform
 **/
class Variable {
 public:
   Variable() = default;

 private:
  // Only shaders create attributes
  Variable(VariableKind kind, const std::string& name,
           int location, GLenum type, GLsizei size);

 public:
  Variable(const Variable&) = delete;
  Variable& operator=(const Variable&) = delete;
  Variable(Variable&&) noexcept;
  Variable& operator=(Variable&&) noexcept;

 public:
  VariableKind GetKind() const { return data_.kind_; }
  const std::string& GetName() const { return data_.name_; }
  int GetLocation() const { return data_.location_; }
  GLenum GetType() const { return data_.type_; }
  size_t GetTypeSize() const { return data_.type_size_; }
  size_t GetBackendSize() const { return data_.backend_size_; }
  size_t GetSize() const { return data_.size_; }
  const uint8_t *GetBackend() const { return data_.backend_.get(); }

 public:
  std::string GetTypeName() const;

 private:
  class Data {
    VariableKind kind_;
    GLint location_;
    std::string name_;
    GLenum type_;
    size_t type_size_;
    size_t backend_size_;
    GLsizei size_;
    std::unique_ptr<uint8_t[]> backend_;
   public:
    friend class Variable;
  } data_;

 public:
  friend class Program;
};



}   // namespace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_VARIABLE_H
