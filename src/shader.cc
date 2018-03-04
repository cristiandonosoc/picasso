#include "shader.h"

#include "utils/result.h"
#include "utils/gl.h"

#include <cassert>
#include <GL/gl3w.h>
#include <memory>

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

/**
 * GLAttribute
 **/
int GLAttribute::GetLocation() const { return location_; }
const std::string& GLAttribute::GetName() const { return name_; }
size_t GLAttribute::GetSize() const { return size_; }
GLenum GLAttribute::GetType() const { return type_; }


/**
 * ShaderProgram
 **/
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

  program.ObtainAttributes();
  return ResultOr<ShaderProgram>::Success(std::move(program));
}

ShaderProgram::ShaderProgram() : attribs_() {}

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
  attribs_ = std::move(other.attribs_);
  return *this;
}

/**
 * INTERNAL API
 **/
void ShaderProgram::ObtainAttributes() {
  // Obtain the max size of the attribute names
  GLint max_attrib_size;
  glGetProgramiv(program_handle_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_attrib_size);

  // We obtain the attributes
  GLint attrib_count;
  glGetProgramiv(program_handle_, GL_ACTIVE_ATTRIBUTES, &attrib_count);
  fprintf(stderr, "ATTRIB SIZE: %zu\n", attribs_.size());
  for (GLint i = 0; i < attrib_count; i++) {
    std::unique_ptr<char[]> name_ptr(new char[max_attrib_size]);

    GLsizei length, size;
    GLenum type;
    glGetActiveAttrib(program_handle_, i, max_attrib_size,
                      &length, &size, &type, name_ptr.get());
    assert(length < max_attrib_size);

    // Obtain the location
    GLint location = glGetAttribLocation(program_handle_, name_ptr.get());

    GLAttribute attrib;
    attrib.location_ = location;
    attrib.name_ = name_ptr.get();  // Copy
    attrib.size_ = size;
    attrib.type_ = type;
    attribs_.push_back(std::move(attrib));
  }

  fprintf(stderr, "ATTRIB SIZE: %zu\n", attribs_.size());
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

/**
 * GETTERS/SETTERS
 **/

int ShaderProgram::GetProgramHandle() const { return program_handle_; }
int ShaderProgram::GetVertexHandle() const { return vertex_handle_; }
int ShaderProgram::GetFragmentHandle() const { return fragment_handle_; }
const std::vector<GLAttribute> ShaderProgram::GetAttributes() const {
  return attribs_;
}


}   // namespace picasso
