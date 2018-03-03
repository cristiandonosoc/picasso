#ifndef SRC_SHADER_H
#define SRC_SHADER_H

#include "utils/result.h"

#include <string>
#include <GL/gl3w.h>

namespace picasso {

class ShaderProgram {
 public:
  static utils::ResultOr<ShaderProgram> Create(const std::string& vertex_src,
                                               const std::string& fragment_src);
 public:
  ShaderProgram();  // Useful for creating stubs
  ~ShaderProgram();

 public:
  // No copy of shaders (we don't want to replicate GL state
  ShaderProgram(const ShaderProgram&) = delete;
  ShaderProgram& operator=(const ShaderProgram&) = delete;

  // no-except moving
  ShaderProgram(ShaderProgram&&) noexcept;
  ShaderProgram& operator=(ShaderProgram&&) noexcept;

 private:
  // Cleans up handles
  void Cleanup();

 private:
  int vertex_handle_ = 0;
  int fragment_handle_ = 0;
  int program_handle_ = 0;
  bool valid_;
};

}   // namespace picasso

#endif
