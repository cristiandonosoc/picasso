#ifndef SRC_SHADERS_PROGRAM_H
#define SRC_SHADERS_PROGRAM_H

#include "shaders/variable.h"
#include "utils/result.h"
#include "utils/macros.h"

#include <map>
#include <memory>
#include <string>
#include <GL/gl3w.h>

namespace picasso {

using namespace utils;

namespace shaders {

class Material;   // Forward Declaration

using VariableContainer = std::map<std::string, Variable>;

class Program {
 public:
  DEFINE_PTR_TYPES(Program);

  // FACTORIES
 private:
  static ResultOr<UniquePtr> Create(const std::string& name, 
                                    const std::string& vertex_src,
                                    const std::string& fragment_src);
  // CONSTRUCTORS
 private:
  Program() = default;

 public:
  ~Program();

  // COPY-MOVE CONSTRUCTORWS
 public:
  DISABLE_COPY(Program);
  // no-except moving
  Program(Program&&) noexcept;
  Program& operator=(Program&&) noexcept;

  // GETTERS/SETTERS
 public:
  const std::string& GetName() { return name_; }
  int GetProgramHandle() const { return program_handle_; }
  int GetVertexHandle() const { return vertex_handle_; }
  int GetFragmentHandle() const { return fragment_handle_; }

  // ATTRIBUTES
 public:
  const VariableContainer& Attributes = attributes_;
  using ConstAttribIt = VariableContainer::const_iterator;
  const VariableContainer& GetAttributes() const { return attributes_; }
  ConstAttribIt AttribBegin() const { return attributes_.cbegin(); }
  ConstAttribIt AttribEnd() const { return attributes_.cend(); }

  // UNIFORMS
 public:
  const VariableContainer& Uniforms = uniforms_;
  using ConstUniformIt = VariableContainer::const_iterator;
  const VariableContainer& GetUniforms() const { return uniforms_; }
  ConstUniformIt UniformBegin() const { return uniforms_.cbegin(); }
  ConstUniformIt UniformEnd() const { return uniforms_.cend(); }

  // MATERIAL INTERFACE
 public:
  void LinkMaterial(Material*);
  void UnlinkMaterial(Material*);

 private:
  // Cleans up handles
  void Cleanup();
  void ObtainAttributes();
  void ObtainUniforms();

 private:
  std::string name_;
  VariableContainer attributes_;
  VariableContainer uniforms_;
  int vertex_handle_ = 0;
  int fragment_handle_ = 0;
  int program_handle_ = 0;
  bool valid_ = false;

 public:
  friend class ProgramRegistry;
};

}   // namespace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_PROGRAM_H
