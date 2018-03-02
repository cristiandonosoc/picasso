#ifndef SRC_SHADER_H
#define SRC_SHADER_H

#include <string>
#include <GL/gl3w.h>

namespace picasso {

class ShaderProgram {
 public:
  ShaderProgram(const std::string& vertex_src,
                const std::string& fragment_src);
  ~ShaderProgram();

 private:
  int program_handle_;
  int vertex_handle_;
  int fragment_handle_;
};

}   // namespace picasso

#endif
