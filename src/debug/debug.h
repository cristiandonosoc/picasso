/******************************************************************************
 * @file: debug.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-07
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_DEBUG_DEBUG_H
#define SRC_DEBUG_DEBUG_H

#include "models/attrib_pointer.h"

#include "utils/macros.h"
#include "utils/glm.h"
#include "utils/singleton.h"
#include "assets/shaders/shader_registry.h"

#include "utils/status.h"

namespace picasso {
namespace debug {

using ::picasso::utils::Singleton;
using ::picasso::models::AttributePointer;
using ::picasso::assets::shaders::ShaderRegistry;
using ::picasso::utils::Status;

class Debug : public Singleton<Debug> {
 public:
  static constexpr size_t VERTEX_LIMIT = 1024;

 public:
  static void FrameInit();
  static Status AddLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);
  static void FrameRender();

  static inline size_t PointToBytes(size_t point_count) {
    return point_count * sizeof(float);
  }

 private:
  Debug();
  DISABLE_COPY(Debug);
  DISABLE_MOVE(Debug);

 private:
  ShaderRegistry::KeyType shader_key_;
  AttributePointer attrib_pos_;
  AttributePointer attrib_color_;
  float points_[VERTEX_LIMIT];
  size_t point_count_ = 0;
  GLuint vbo_;
  GLuint vao_;

 public:
  friend Singleton<Debug>;
};  // class Debug

}   // namespace debug
}   // namespace picasso

#endif  // SRC_DEBUG_DEBUG_H
