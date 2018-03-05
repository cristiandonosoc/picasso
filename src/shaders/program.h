#ifndef SRC_SHADERS_PROGRAM_H
#define SRC_SHADERS_PROGRAM_H

#include "shaders/variable.h"
#include "utils/result.h"

#include <map>
#include <string>
#include <GL/gl3w.h>

namespace picasso {
namespace shaders {

using VariableContainer = std::map<std::string, Variable>;

class Program {
  // FACTORIES
 public:
  static utils::ResultOr<Program> Create(const std::string& vertex_src,
                                               const std::string& fragment_src);
  // CONSTRUCTORS
 public:
  Program();  // Useful for creating stubs
  ~Program();

  // COPY-MOVE CONSTRUCTORWS
 public:
  // No copy of shaders (we don't want to replicate GL state
  Program(const Program&) = delete;
  Program& operator=(const Program&) = delete;
  // no-except moving
  Program(Program&&) noexcept;
  Program& operator=(Program&&) noexcept;

  // GETTERS/SETTERS
 public:
  int GetProgramHandle() const { return program_handle_; }
  int GetVertexHandle() const { return vertex_handle_; }
  int GetFragmentHandle() const { return fragment_handle_; }


  // ATTRIBUTES
 public:
  using ConstAttribIt = VariableContainer::const_iterator;
  const VariableContainer& GetAttributes() const { return attribs_; }
  ConstAttribIt AttribBegin() const { return attribs_.cbegin(); }
  ConstAttribIt AttribEnd() const { return attribs_.cend(); }

  // UNIFORMS
 public:
  const VariableContainer& Uniforms = uniforms_;
  using ConstUniformIt = VariableContainer::const_iterator;
  const VariableContainer& GetUniforms() const { return uniforms_; }
  ConstUniformIt UniformBegin() const { return uniforms_.cbegin(); }
  ConstUniformIt UniformEnd() const { return uniforms_.cend(); }

 private:
  // Cleans up handles
  void Cleanup();
  void ObtainAttributes();
  void ObtainUniforms();

 private:
  VariableContainer attribs_;
  VariableContainer uniforms_;
  int vertex_handle_ = 0;
  int fragment_handle_ = 0;
  int program_handle_ = 0;
  bool valid_ = false;

};

}   // namespace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_PROGRAM_H
