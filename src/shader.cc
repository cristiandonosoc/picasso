#include "shader.h"

#include <cassert>
#include <GL/gl3w.h>

namespace picasso {

namespace {

int CompileShader(const std::string& shader_name,
                  GLenum shader_kind, const std::string& src) {
  int handle = glCreateShader(shader_kind);
  assert(handle);
  const GLchar *src_ptr = src.c_str();
  glShaderSource(handle, 1, &src_ptr, 0);
  glCompileShader(handle);
  // Check for error
  GLint success = 0;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    GLchar log[2048];
    glGetShaderInfoLog(handle, sizeof(log), 0, log);
    fprintf(stderr, "ERROR COMPILING %s: %s\n", shader_name.c_str(), log);
    assert(!"Error compiling shader. Aborting.");
	}

  return handle;
}

}   // namespace

ShaderProgram::ShaderProgram(const std::string& vertex_src,
                             const std::string& fragment_src) {

  vertex_handle_ = CompileShader("Vertex Shader", GL_VERTEX_SHADER,
                                 vertex_src);
  fragment_handle_ = CompileShader("Fragment Shader", GL_FRAGMENT_SHADER,
                                   fragment_src);

  program_handle_ = glCreateProgram();
  assert(program_handle_);

  glAttachShader(program_handle_, vertex_handle_);
  glAttachShader(program_handle_, fragment_handle_);
  glLinkProgram(program_handle_);

  GLint is_linked = 0;
  glGetProgramiv(program_handle_, GL_LINK_STATUS, &is_linked);
  assert(is_linked != GL_FALSE);
}

ShaderProgram::~ShaderProgram() {
  if (vertex_handle_) {
    glDeleteShader(vertex_handle_);
  }
  if (fragment_handle_) {
    glDeleteShader(fragment_handle_);
  }
  if (program_handle_) {
    glDeleteShader(program_handle_);
  }


}


}   // namespace picasso
