/******************************************************************************
 * @file: shader.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_SHADER_H
#define SRC_ASSETS_SHADER_H

#include <map>
#include <memory>
#include <string>
#include <GL/gl3w.h>

/* #include "shaders/variable.h" */
#include "utils/status_or.h"
#include "utils/macros.h"
#include "models/attrib_pointer.h"


using namespace ::picasso::utils;
using ::picasso::models::AttributeKind;

namespace picasso {
namespace assets {

namespace shaders {

class ShaderVariable {
 public:
  std::string name;
  std::string type_name;
  int location;
  GLenum type;
  int count;
  int type_size;
};  // class ShaderVariable

};  // namespace shaders

class Shader {
 public:
  using Attribute = ::picasso::assets::shaders::ShaderVariable;
  using Uniform = ::picasso::assets::shaders::ShaderVariable;

 public:
  using AttributeMap = std::map<std::string, Attribute>;
  using UniformMap = std::map<std::string, Uniform>;

 /* public: */
 /*  using VariableMap = std::map<std::string, Variable>; */
 /*  using AttributeMap = std::map<AttributeKind, std::string>; */

 public:
  DEFINE_PTR_TYPES(Shader);

  // CONSTRUCTORS
 private:
  Shader() = default;
  DISABLE_COPY(Shader);
  DISABLE_MOVE(Shader);

 public:
  ~Shader();

  /* // COPY-MOVE CONSTRUCTORWS */
 /* public: */
  /* DISABLE_COPY(Shader); */
  /* // no-except moving */
  /* Shader(Shader&&) noexcept; */
  /* Shader& operator=(Shader&&) noexcept; */

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
  const AttributeMap& Attributes = attributes_;
  /* const AttributeMap& AttributeMapping = attribute_map_; */
  /* using ConstAttribIt = VariableMap::const_iterator; */
  /* const VariableMap& GetAttributes() const { return attributes_; } */
  /* ConstAttribIt AttribBegin() const { return attributes_.cbegin(); } */
  /* ConstAttribIt AttribEnd() const { return attributes_.cend(); } */

  // UNIFORMS
 public:
  const UniformMap& Uniforms = uniforms_;
  /* using ConstUniformIt = VariableMap::const_iterator; */
  /* const VariableMap& GetUniforms() const { return uniforms_; } */
  /* ConstUniformIt UniformBegin() const { return uniforms_.cbegin(); } */
  /* ConstUniformIt UniformEnd() const { return uniforms_.cend(); } */

 private:
  // Cleans up handles
  void Cleanup();
  void ObtainAttributes();
  void ObtainUniforms();

 private:
  std::string name_;
  std::string vertex_src_;
  std::string fragment_src_;

  AttributeMap attributes_;
  /* AttributeMap attribute_map_; */

  UniformMap uniforms_;

  int vertex_handle_ = 0;
  int fragment_handle_ = 0;
  int shader_handle_ = 0;
  bool valid_ = false;

 public:
  friend class ShaderRegistry;
};

}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_SHADER_H
