/******************************************************************************
 * @file: shader_registry.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-14
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "registries/shader_registry.h"
#include "utils/file.h"

namespace picasso {
namespace registries {

using ::picasso::utils::Status;
using ::picasso::utils::ReadWholeFile;

namespace {

StatusOr<int> CompileShader(const std::string& shader_name,
                  GLenum shader_kind, const std::string& src) {
  int shader_handle = glCreateShader(shader_kind);
  if (!shader_handle) {
    return { Status::STATUS_ERROR, "Could not create shader \"%s\"",
                                   shader_name.c_str() };
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
    return { Status::STATUS_ERROR, "Error compiling \"%s\": %s\n",
                                   shader_name.c_str(), log };
	}

  return shader_handle;
}



}   // namespace

/**
 * STATIC INTERFACE
 **/
StatusOr<ShaderRegistry::Result> ShaderRegistry::CreateFromFiles(
    const std::string& name,
    const std::string& vertex_path,
    const std::string& fragment_path) {
  std::string vs = ReadWholeFile(vertex_path);
  std::string fs = ReadWholeFile(fragment_path);

  return Create(name, vs, fs);
}

StatusOr<ShaderRegistry::Result> ShaderRegistry::Create(const std::string& name,
                                           const std::string& vs,
                                           const std::string& fs) {
  // We attempt to allocate it
  auto reg_res = Register(name);
  if (!reg_res.Ok()) {
    return reg_res;
  }

  // We attempt to register the shader
  Result registry_result = reg_res.ConsumeOrDie();
  auto load_res = LoadShader(registry_result.value, name, vs, fs);
  if (!load_res.Ok()) {
    Unregister(registry_result.key);
    return load_res;
  }
  return registry_result;
}

Status ShaderRegistry::LoadShader(Shader *shader,
                  const std::string& name,
                  const std::string& vertex_src,
                  const std::string& fragment_src) {
  shader->name_ = name;

  // Vertex Shader
  auto vertex_res = CompileShader("Vertex", GL_VERTEX_SHADER, vertex_src);
  if (!vertex_res.Ok()) {
    return vertex_res;
  }
  shader->vertex_handle_ = vertex_res.ConsumeOrDie();

  // Fragment Shader
  auto fragment_res = CompileShader("Fragment", GL_FRAGMENT_SHADER,
                                    fragment_src);
  if (!fragment_res.Ok()) {
    return fragment_res;
  }
  shader->fragment_handle_ = fragment_res.ConsumeOrDie();

  // Shader
  shader->shader_handle_ = glCreateProgram();
  if (!shader->shader_handle_) {
    return { Status::STATUS_ERROR, "Could not get shader handle" };
  }

  glAttachShader(shader->shader_handle_, shader->vertex_handle_);
  glAttachShader(shader->shader_handle_, shader->fragment_handle_);
  glLinkProgram(shader->shader_handle_);
  GLint is_linked;
  glGetProgramiv(shader->shader_handle_, GL_LINK_STATUS, &is_linked);
  if (is_linked == GL_FALSE) {
    GLchar log[2048];
    glGetShaderInfoLog(shader->shader_handle_, sizeof(log), 0, log);
    return { Status::STATUS_ERROR, "Error linking shader: %s", log };
  }

  // We set the sources
  shader->vertex_src_ = vertex_src;
  shader->fragment_src_ = fragment_src;

  shader->ObtainAttributes();
  shader->ObtainUniforms();

  shader->valid_ = true;
  return Status::STATUS_OK;
}



}   // namespace registries
}   // namespace picasso


