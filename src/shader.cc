#include "shader.h"
#include "utils/result.h"

#include <cassert>
#include <GL/gl3w.h>

namespace picasso {

using namespace utils;

namespace {

ResultOr<int> CompileShader(const std::string& shader_name,
                  GLenum shader_kind, const std::string& src) {
  int shader_handle = glCreateShader(shader_kind);
  if (!shader_handle) {
    return ResultOr<int>::Error("Could not create shader \"%s\"\n",
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
    return ResultOr<int>::Error("Error compiling \"%s\": %s\n",
                                shader_name.c_str(), log);
	}

  return ResultOr<int>::Success(std::move(shader_handle));
}

}   // namespace

ResultOr<ShaderProgram> ShaderProgram::Create(const std::string& vertex_src,
                                              const std::string& fragment_src) {
  // If some result is invalid, the ShaderProgram destructor will
  // free the resources
  ShaderProgram program;
  // Vertex Shader
  auto vertex_res = CompileShader("Vertex", GL_VERTEX_SHADER, vertex_src);
  if (!vertex_res.Valid()) {
    return ResultOr<ShaderProgram>::Error(vertex_res.ErrorMsg());
  }
  program.vertex_handle_ = vertex_res.ConsumeOrDie();

  // Fragment Shader
  auto fragment_res = CompileShader("Fragment", GL_FRAGMENT_SHADER,
                                    fragment_src);
  if (!fragment_res.Valid()) {
    return ResultOr<ShaderProgram>::Error(fragment_res.ErrorMsg());
  }
  program.fragment_handle_ = fragment_res.ConsumeOrDie();

  // Program
  program.program_handle_ = glCreateProgram();
  if (!program.program_handle_) {
    return ResultOr<ShaderProgram>::Error("Could not get program handle");
  }

  glAttachShader(program.program_handle_, program.vertex_handle_);
  glAttachShader(program.program_handle_, program.fragment_handle_);
  glLinkProgram(program.program_handle_);
  GLint is_linked;
  glGetProgramiv(program.program_handle_, GL_LINK_STATUS, &is_linked);
  if (is_linked == GL_FALSE) {
    GLchar log[2048];
    glGetProgramInfoLog(program.program_handle_, sizeof(log), 0, log);
    return ResultOr<ShaderProgram>::Error("Error linkink program: %s\n", log);
  }

  return ResultOr<ShaderProgram>::Success(std::move(program));
}

ShaderProgram::ShaderProgram() {}

ShaderProgram::~ShaderProgram() {
  Cleanup();
}

/**
 * Operators
 **/
ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept {
  *this = std::move(other);   // Call the move assignment;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
  vertex_handle_ = other.vertex_handle_;
  other.vertex_handle_ = 0;
  fragment_handle_ = other.fragment_handle_;
  other.fragment_handle_ = 0;
  program_handle_ = other.program_handle_;
  other.program_handle_ = 0;
  valid_ = other.valid_;
  other.valid_ = false;
  return *this;
}


void ShaderProgram::Cleanup() {
  if (vertex_handle_) {
    glDeleteShader(vertex_handle_);
  }
  if (fragment_handle_) {
    glDeleteShader(fragment_handle_);
  }
  if (program_handle_) {
    glDeleteProgram(program_handle_);
  }
}


}   // namespace picasso