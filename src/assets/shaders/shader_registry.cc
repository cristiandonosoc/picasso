/******************************************************************************
 * @file: shader_registry.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "assets/shaders/shader_registry.h"
#include "utils/file.h"

namespace picasso {
namespace assets {
namespace shaders {

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
StatusOr<ShaderRegistry::KeyType> ShaderRegistry::CreateFromFiles(
    const std::string& name,
    const std::string& vertex_path,
    const std::string& fragment_path) {
  std::string vs = ReadWholeFile(vertex_path);
  std::string fs = ReadWholeFile(fragment_path);

  return Create(name, vs, fs);
}

StatusOr<ShaderRegistry::KeyType> ShaderRegistry::Create(const std::string& name,
                                           const std::string& vs,
                                           const std::string& fs) {
  auto& map = Instance().map_;

  // We check if it exists already
  auto it = map.find(name);
  if (it != map.end()) {
    return { Status::STATUS_ERROR, "Shader %s already exists", name.c_str() };
  }

  // We attempt to create it
  auto res = InternalCreate(name, vs, fs); 
  if (!res.Ok()) { 
    return res;
  }

  // Add it to the registry
  map[name] = res.ConsumeOrDie();
  return name;
}

StatusOr<Shader*> ShaderRegistry::Get(const std::string& name) {
  auto& map = Instance().map_;
  auto it = map.find(name);
  if (it == map.end()) {
    return FILENO_STATUS(Status::STATUS_ERROR, "ShaderRegistry: cannot find key \"%s\"",
                         name.c_str());
  }
  return it->second.get();
}

std::vector<Shader*> ShaderRegistry::GetShaders() {
  std::vector<Shader*> programs;
  auto& map = Instance().map_;
  programs.reserve(map.size());
  for (auto&& it : map) {
    programs.push_back(it.second.get());
  }

  return programs;
}

StatusOr<Shader::UniquePtr> ShaderRegistry::InternalCreate(const std::string& name,
                                           const std::string& vertex_src,
                                           const std::string& fragment_src) {
  // If some result is invalid, the Shader destructor will
  // free the resources
  Shader::UniquePtr shader(new Shader());   // private constructor
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
  return shader;
}

}   // namespace shaders
}   // namespace assets
}   // namespace picasso
