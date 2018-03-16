#include "shaders/shader.h"

#include "logging/log.h"
#include "utils/status_or.h"
#include "utils/gl.h"
#include "utils/make_unique.h"

#include <cassert>
#include <GL/gl3w.h>
#include <memory>
#include <regex>

namespace picasso {
namespace shaders {

using namespace utils;
using ::picasso::models::AttributeKind;

namespace {

StatusOr<int> CompileShader(const std::string& shader_name,
                  GLenum shader_kind, const std::string& src) {
  int shader_handle = glCreateShader(shader_kind);
  if (!shader_handle) {
    return StatusOr<int>::Error("Could not create shader \"%s\"\n",
                                shader_name.c_str());
  }

  // We compile the shader
  const GLchar *src_ptr = src.c_str();
  glShaderSource(shader_handle, 1, &src_ptr, 0);
  glCompileShader(shader_handle);
  // Check for error
  GLint success = 0;
  glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    GLchar log[2048];
    glGetShaderInfoLog(shader_handle, sizeof(log), 0, log);
    return StatusOr<int>::Error("Error compiling \"%s\": %s\n",
                                shader_name.c_str(), log);
	}

  return shader_handle;
}

}   // namespace

/**
 * Shader
 **/
StatusOr<Shader::UniquePtr> Shader::Create(const std::string& name,
                                           const std::string& vertex_src,
                                           const std::string& fragment_src) {
  // If some result is invalid, the Shader destructor will
  // free the resources
  UniquePtr shader(new Shader());   // private constructor
  shader->name_ = name;

  // Vertex Shader
  auto vertex_res = CompileShader("Vertex", GL_VERTEX_SHADER, vertex_src);
  if (!vertex_res.Ok()) {
    return { Status::STATUS_ERROR, vertex_res.ErrorMsg() };
  }
  shader->vertex_handle_ = vertex_res.ConsumeOrDie();

  // Fragment Shader
  auto fragment_res = CompileShader("Fragment", GL_FRAGMENT_SHADER,
                                    fragment_src);
  if (!fragment_res.Ok()) {
    return { Status::STATUS_ERROR, fragment_res.ErrorMsg() };
  }
  shader->fragment_handle_ = fragment_res.ConsumeOrDie();

  // Shader
  shader->shader_handle_ = glCreateProgram();
  if (!shader->shader_handle_) {
    return StatusOr<UniquePtr>::Error("Could not get shader handle");
  }

  glAttachShader(shader->shader_handle_, shader->vertex_handle_);
  glAttachShader(shader->shader_handle_, shader->fragment_handle_);
  glLinkProgram(shader->shader_handle_);
  GLint is_linked;
  glGetProgramiv(shader->shader_handle_, GL_LINK_STATUS, &is_linked);
  if (is_linked == GL_FALSE) {
    GLchar log[2048];
    glGetShaderInfoLog(shader->shader_handle_, sizeof(log), 0, log);
    return StatusOr<UniquePtr>::Error("Error linkink shader: %s\n", log);
  }

  // We set the sources
  shader->vertex_src_ = vertex_src;
  shader->fragment_src_ = fragment_src;

  shader->ObtainAttributes();
  shader->ObtainUniforms();

  shader->valid_ = true;
  return shader;
}

Shader::~Shader() {
  Cleanup();
}

/**
 * Operators
 **/
Shader::Shader(Shader&& other) noexcept {
  *this = std::move(other);   // Call the move assignment;
}

Shader& Shader::operator=(Shader&& other) noexcept {
  if (this != &other) {
    vertex_handle_ = other.vertex_handle_;
    other.vertex_handle_ = 0;
    fragment_handle_ = other.fragment_handle_;
    other.fragment_handle_ = 0;
    shader_handle_ = other.shader_handle_;
    other.shader_handle_ = 0;
    valid_ = other.valid_;
    other.valid_ = false;
    attributes_ = std::move(other.attributes_);
    uniforms_ = std::move(other.uniforms_);
  }
  return *this;
}

/**
 * INTERNAL API
 **/
void Shader::ObtainAttributes() {
  // Obtain the max size of the attribute names
  GLint max_attrib_size;
  glGetProgramiv(shader_handle_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_attrib_size);

  // We obtain the attributes
  GLint attrib_count = 0;
  glGetProgramiv(shader_handle_, GL_ACTIVE_ATTRIBUTES, &attrib_count);
  for (GLint i = 0; i < attrib_count; i++) {
    std::unique_ptr<char[]> name_ptr(new char[max_attrib_size]);

    GLsizei length, size;
    GLenum type;
    glGetActiveAttrib(shader_handle_, i, max_attrib_size,
                      &length, &size, &type, name_ptr.get());
    assert(length < max_attrib_size);

    // Obtain the location
    GLint location = glGetAttribLocation(shader_handle_, name_ptr.get());
    Variable attrib(VariableKind::ATTRIBUTE, name_ptr.get(), location,
                    type, size);
    attributes_[attrib.GetName()] = std::move(attrib);
  }


  // TODO(Cristian): Eventually remove this
  std::regex attr_regex("in\\s+\\w+\\s+(\\w+)\\s*;\\s+//\\s+ATTR:(\\w+)");
  std::sregex_iterator regend;
  std::sregex_iterator regit(vertex_src_.begin(), vertex_src_.end(), attr_regex);
  for (; regit != regend; regit++) {
    // We get the match
    std::smatch match = *regit;
    const std::string& attr_name = match[1].str();
    const std::string& attr_type = match[2].str();
    
    // We see if its a valid enum option
    auto res = AttributeKind::FromString(attr_type);
    if (res.Ok()) {
      AttributeKind kind = res.ConsumeOrDie();
      attribute_map_[kind] = attr_name;
    }
  }
}

void Shader::ObtainUniforms() {
  // Obtain the max size of the uniforms names
  GLint max_uniform_size;
  glGetProgramiv(shader_handle_, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                 &max_uniform_size);

  // We obtain the uniforms
  GLint uniform_count = 0;
  glGetProgramiv(shader_handle_, GL_ACTIVE_UNIFORMS, &uniform_count);
  for (GLint i = 0; i < uniform_count; i++) {
    std::unique_ptr<char[]> name_ptr(new char[max_uniform_size]);

    GLsizei length, size;
    GLenum type;
    glGetActiveUniform(shader_handle_, i, max_uniform_size,
                       &length, &size, &type, name_ptr.get());
    assert(length < max_uniform_size);

    // Obtain the location
    GLint location = glGetUniformLocation(shader_handle_, name_ptr.get());

    Variable uniform(VariableKind::UNIFORM, name_ptr.get(), location,
                     type, size);
    uniforms_[uniform.GetName()] = std::move(uniform);
  }
}


void Shader::Cleanup() {
  if (vertex_handle_) {
    glDeleteShader(vertex_handle_);
  }
  if (fragment_handle_) {
    glDeleteShader(fragment_handle_);
  }
  if (shader_handle_) {
    glDeleteShader(shader_handle_);
  }
}

/**
 * MATERIAL INTERFACE
 **/
void Shader::LinkMaterial(Material*) {
  LOG_WARN("%s: Not Implemented", __FUNCTION__);
}

void Shader::UnlinkMaterial(Material*) {
  LOG_WARN("%s: Not Implemented", __FUNCTION__);
}


void Shader::DebugPrint(int indent) const {
  LOG_INDENT_DEBUG(indent, "Shader debug print for \"%s\"", name_.c_str());

  // Vertex Shader
  LOG_INDENT_DEBUG(indent, "Vertex Handle: %d", vertex_handle_);
  LOG_INDENT_DEBUG(indent, "Vertex Source: \n%s", vertex_src_.c_str());

  // Fragment Shader
  LOG_INDENT_DEBUG(indent, "Fragment Handle: %d", fragment_handle_);
  LOG_INDENT_DEBUG(indent, "Fragment Source: \n%s", fragment_src_.c_str());


  // Attributes
  LOG_INDENT_DEBUG(indent, "Found %zu attributes", Attributes.size());
  for (auto&& it : Attributes) {
    it.second.DebugPrint(indent + 4);
  }

  LOG_INDENT_DEBUG(indent, "Found %zu attribte mappings", 
                      AttributeMapping.size());
  for (auto&& it : AttributeMapping) {
    LOG_INDENT_DEBUG(indent + 4, "%s: %s", 
                        AttributeKind::ToString(it.first).c_str(),
                        it.second.c_str());

  }

  LOG_INDENT_DEBUG(indent, "Found %zu uniforms", Uniforms.size());
  for (auto&& it : Uniforms) {
    it.second.DebugPrint(indent + 4);
  }
}


}   // namespace shaders
}   // namespace picasso
