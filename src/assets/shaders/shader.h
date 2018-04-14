/******************************************************************************
 * @file: shader.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_SHADERS_SHADER_H
#define SRC_ASSETS_SHADERS_SHADER_H

#include <map>
#include <memory>
#include <string>
#include <GL/gl3w.h>

/* #include "shaders/variable.h" */
#include "utils/macros.h"
#include "models/attrib_pointer.h"

#include "assets/shaders/attribute.h"
#include "assets/shaders/uniform.h"

namespace picasso {
namespace assets {
namespace shaders {

using ::picasso::models::AttributeKind;

class Shader {
 public:
  using AttributeMap = std::map<std::string, Attribute>;
  using UniformMap = std::map<std::string, Uniform>;

 public:
  DEFINE_PTR_TYPES(Shader);

  // CONSTRUCTORS
 public:
  Shader(const std::string& name) : name_(name) {}
  DISABLE_COPY(Shader);
  DEFAULT_MOVE(Shader);

 public:
  ~Shader();

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

  // UNIFORMS
 public:
  UniformMap& Uniforms = uniforms_;

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

}   // namespace shaders
}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_SHADERS_SHADER_H
