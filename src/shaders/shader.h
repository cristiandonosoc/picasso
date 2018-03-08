#ifndef SRC_SHADERS_shader_H
#define SRC_SHADERS_shader_H

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

class Shader {
 public:
  DEFINE_PTR_TYPES(Shader);

  // FACTORIES
 private:
  static ResultOr<UniquePtr> Create(const std::string& name, 
                                    const std::string& vertex_src,
                                    const std::string& fragment_src);
  // CONSTRUCTORS
 private:
  Shader() = default;

 public:
  ~Shader();

  // COPY-MOVE CONSTRUCTORWS
 public:
  DISABLE_COPY(Shader);
  // no-except moving
  Shader(Shader&&) noexcept;
  Shader& operator=(Shader&&) noexcept;

  // GETTERS/SETTERS
 public:
  const std::string& GetName() { return name_; }
  int GetShaderHandle() const { return shader_handle_; }
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
  int shader_handle_ = 0;
  bool valid_ = false;

 public:
  friend class ShaderRegistry;
};

}   // namespace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_shader_H
