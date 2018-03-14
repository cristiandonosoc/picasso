#ifndef SRC_SHADERS_shader_H
#define SRC_SHADERS_shader_H

#include "shaders/variable.h"
#include "utils/result.h"
#include "utils/macros.h"
#include "models/attrib_pointer.h"

#include <map>
#include <memory>
#include <string>
#include <GL/gl3w.h>

namespace picasso {

using namespace utils;

namespace shaders {

using ::picasso::models::AttributeKind;

class Material;   // Forward Declaration


class Shader {
 public:
  using VariableMap = std::map<std::string, Variable>;
  using AttributeMap = std::map<AttributeKind, std::string>;

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

 public:
  void DebugPrint(int indent = 0) const;

  // GETTERS/SETTERS
 public:
  bool Valid() const { return valid_; }
  const std::string& GetName() const { return name_; }
  int GetShaderHandle() const { return shader_handle_; }
  int GetVertexHandle() const { return vertex_handle_; }
  int GetFragmentHandle() const { return fragment_handle_; }

  const std::string& GetVertexSource() const { return vertex_src_; }
  const std::string& GetFragmentSource() const { return fragment_src_; }

  // ATTRIBUTES
 public:
  const VariableMap& Attributes = attributes_;
  const AttributeMap& AttributeMapping = attribute_map_;
  using ConstAttribIt = VariableMap::const_iterator;
  const VariableMap& GetAttributes() const { return attributes_; }
  ConstAttribIt AttribBegin() const { return attributes_.cbegin(); }
  ConstAttribIt AttribEnd() const { return attributes_.cend(); }

  // UNIFORMS
 public:
  const VariableMap& Uniforms = uniforms_;
  using ConstUniformIt = VariableMap::const_iterator;
  const VariableMap& GetUniforms() const { return uniforms_; }
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
  std::string vertex_src_;
  std::string fragment_src_;

  VariableMap attributes_;
  AttributeMap attribute_map_;

  VariableMap uniforms_;

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
