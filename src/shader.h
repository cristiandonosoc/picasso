#ifndef SRC_SHADER_H
#define SRC_SHADER_H

#include "utils/result.h"

#include <string>
#include <GL/gl3w.h>
#include <vector>

namespace picasso {

enum ShaderVariableKind {
  NONE,
  ATTRIBUTE,
  UNIFORM,
};

/**
 * ShaderVariable
 * ---------------
 *
 * Represents the information of either a GL attribute of uniform
 **/
class ShaderVariable {
 private:
  // Only shaders create attributes
  ShaderVariable(ShaderVariableKind kind) : kind_(kind) {}

 public:
  ShaderVariableKind GetKind() const { return kind_; }
  int GetLocation() const { return location_; }
  const std::string& GetName() const { return name_; }
  size_t GetSize() const { return size_; }
  GLenum GetType() const { return type_; }

 public:
  std::string GetTypeName() const;

 private:
  ShaderVariableKind kind_;
  GLint location_;
  std::string name_;
  GLsizei size_;
  GLenum type_;

 public:
  friend class ShaderProgram;
};

using ShaderVariableContainer = std::vector<ShaderVariable>;

class ShaderProgram {
  // FACTORIES
 public:
  static utils::ResultOr<ShaderProgram> Create(const std::string& vertex_src,
                                               const std::string& fragment_src);
  // CONSTRUCTORS
 public:
  ShaderProgram();  // Useful for creating stubs
  ~ShaderProgram();

  // COPY-MOVE CONSTRUCTORWS
 public:
  // No copy of shaders (we don't want to replicate GL state
  ShaderProgram(const ShaderProgram&) = delete;
  ShaderProgram& operator=(const ShaderProgram&) = delete;
  // no-except moving
  ShaderProgram(ShaderProgram&&) noexcept;
  ShaderProgram& operator=(ShaderProgram&&) noexcept;

  // GETTERS/SETTERS
 public:
  int GetProgramHandle() const { return program_handle_; }
  int GetVertexHandle() const { return vertex_handle_; }
  int GetFragmentHandle() const { return fragment_handle_; }


  // ATTRIBUTES
 public:
  using ConstAttribIt = ShaderVariableContainer::const_iterator;
  const ShaderVariableContainer& GetAttributes() const { return attribs_; }
  ConstAttribIt AttribBegin() const { return attribs_.cbegin(); }
  ConstAttribIt AttribEnd() const { return attribs_.cend(); }

  // UNIFORMS
 public:
  using ConstUniformIt = ShaderVariableContainer::const_iterator;
  const ShaderVariableContainer& GetUniforms() const { return uniforms_; }
  ConstUniformIt UniformBegin() const { return uniforms_.cbegin(); }
  ConstUniformIt UniformEnd() const { return uniforms_.cend(); }

 private:
  // Cleans up handles
  void Cleanup();
  void ObtainAttributes();
  void ObtainUniforms();

 private:
  ShaderVariableContainer attribs_;
  ShaderVariableContainer uniforms_;
  int vertex_handle_ = 0;
  int fragment_handle_ = 0;
  int program_handle_ = 0;
  bool valid_ = false;

};

}   // namespace picasso

#endif
