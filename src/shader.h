#ifndef SRC_SHADER_H
#define SRC_SHADER_H

#include "utils/result.h"

#include <string>
#include <GL/gl3w.h>
#include <vector>

namespace picasso {

class GLAttribute {
 private:
  GLAttribute() {}  // Only shaders create attributes
 public:
  int GetLocation() const;
  const std::string& GetName() const;
  size_t GetSize() const;
  GLenum GetType() const;

 private:
  GLint location_;
  std::string name_;
  GLsizei size_;
  GLenum type_;

 public:
  friend class ShaderProgram;
};

class ShaderProgram {
  // FACTORIES
 public:
  static utils::ResultOr<ShaderProgram> Create(const std::string& vertex_src,
                                               const std::string& fragment_src);
  // CONSTRUCTORS
 public:
  ShaderProgram();  // Useful for creating stubs
  ~ShaderProgram();

  // COPY-MOVE CONSTRUCTORWS
 public:
  // No copy of shaders (we don't want to replicate GL state
  ShaderProgram(const ShaderProgram&) = delete;
  ShaderProgram& operator=(const ShaderProgram&) = delete;
  // no-except moving
  ShaderProgram(ShaderProgram&&) noexcept;
  ShaderProgram& operator=(ShaderProgram&&) noexcept;

  // GETTERS/SETTERS
 public:
  int GetProgramHandle() const;
  int GetVertexHandle() const;
  int GetFragmentHandle() const;
  const std::vector<GLAttribute> GetAttributes() const;

 private:
  // Cleans up handles
  void Cleanup();
  void ObtainAttributes();

 private:
  std::vector<GLAttribute> attribs_;
  int vertex_handle_ = 0;
  int fragment_handle_ = 0;
  int program_handle_ = 0;
  bool valid_ = false;

};

}   // namespace picasso

#endif
