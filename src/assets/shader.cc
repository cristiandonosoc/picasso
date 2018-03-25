#include "assets/shader.h"

#include "logging/log.h"
#include "utils/status_or.h"
#include "utils/gl.h"
#include "utils/make_unique.h"

#include <cassert>
#include <GL/gl3w.h>
#include <memory>
#include <regex>

namespace picasso {
namespace assets {

using ::picasso::utils::GL_TYPES_TO_STRING;

Shader::~Shader() {
  Cleanup();
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
    GLint location = glGetAttribLocation(shader_handle_, name_ptr.get());

    Attribute attrib;
    attrib.name = name_ptr.get();
    attrib.location = location;
    attrib.type = type;
    attrib.count = size;
    // TODO(Cristian): Don't crash here!
    attrib.type_size = GL_TYPES_TO_STRING.GetSize(type).ConsumeOrDie();
    attrib.type_name = GL_TYPES_TO_STRING.GetName(type).ConsumeOrDie();
    attributes_[attrib.name] = std::move(attrib);
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
    GLint location = glGetUniformLocation(shader_handle_, name_ptr.get());

    Uniform uniform;
    uniform.name = name_ptr.get();
    uniform.location = location;
    uniform.type = type;
    uniform.count = size;
    // TODO(Cristian): Don't crash here!
    uniform.type_size = GL_TYPES_TO_STRING.GetSize(type).ConsumeOrDie();
    uniform.type_name = GL_TYPES_TO_STRING.GetName(type).ConsumeOrDie();
    uniforms_[uniform.name] = std::move(uniform);
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

/************************************************************************************
 * DEBUG PRINT
 ************************************************************************************/

namespace {

void DebugPrintAttribute(const Shader::Attribute& attribute, int indent) {
  LOG_INDENT_DEBUG(indent, "ATTRIBUTE debug print for \"%s\"", attribute.name.c_str());
  LOG_INDENT_DEBUG(indent, "Location: %d", attribute.location);
  LOG_INDENT_DEBUG(indent, "Type: %s", attribute.type_name.c_str());
  LOG_INDENT_DEBUG(indent, "Type Size: %zu", attribute.type_size);
  LOG_INDENT_DEBUG(indent, "Count: %d", attribute.count);
}

void DebugPrintUniform(const Shader::Uniform& uniform, int indent) {
  LOG_INDENT_DEBUG(indent, "UNIFORM debug print for \"%s\"", uniform.name.c_str());
  LOG_INDENT_DEBUG(indent, "Location: %d", uniform.location);
  LOG_INDENT_DEBUG(indent, "Type: %s", uniform.type_name.c_str());
  LOG_INDENT_DEBUG(indent, "Type Size: %zu", uniform.type_size);
  LOG_INDENT_DEBUG(indent, "Count: %d", uniform.count);
}


}   // namespace

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
    DebugPrintAttribute(it.second, indent + 4);
  }

  /* LOG_INDENT_DEBUG(indent, "Found %zu attribte mappings", */ 
  /*                     AttributeMapping.size()); */
  /* for (auto&& it : AttributeMapping) { */
  /*   LOG_INDENT_DEBUG(indent + 4, "%s: %s", */ 
  /*                       AttributeKind::ToString(it.first).c_str(), */
  /*                       it.second.c_str()); */

  /* } */

  LOG_INDENT_DEBUG(indent, "Found %zu uniforms", Uniforms.size());
  for (auto&& it : Uniforms) {
    DebugPrintUniform(it.second, indent + 4);
  }
}


}   // namespace assets
}   // namespace picasso
