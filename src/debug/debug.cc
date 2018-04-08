/******************************************************************************
 * @file: debug.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-07
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "debug/debug.h"
#include "utils/paths.h"
#include "camera.h"

#include "logging/log.h"

namespace picasso {
namespace debug {

using ::picasso::utils::paths::GetExecutableDir;
using ::picasso::models::AttributeKind;
using ::picasso::assets::shaders::Shader;

Debug::Debug() {
  shader_key_ = ShaderRegistry::CreateFromFiles("debug_shader", 
      GetExecutableDir() + "/shaders/debug.vert",
      GetExecutableDir() + "/shaders/debug.frag").ConsumeOrDie();
  
  attrib_pos_ = AttributePointer(AttributeKind::VERTEX, 3,
                                 GL_FLOAT, GL_FALSE,
                                 6 * sizeof(float), 0);
  attrib_color_ = AttributePointer(AttributeKind::COLOR, 3,
                                   GL_FLOAT, GL_FALSE,
                                   6 * sizeof(float), 3 * sizeof(float));

  // We generate the buffer
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
}

void Debug::FrameInit() {
  auto& debug = Instance();
  debug.point_count_ = 0;
}

Status Debug::AddLine(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
  auto& debug = Instance();
  size_t offset = 12 * sizeof(float);
  size_t new_point_count = 12;
  size_t byte_offset = PointToBytes(debug.point_count_ + new_point_count);
  if (byte_offset >= VERTEX_LIMIT) {
    return { Status::STATUS_ERROR, 
            "Not enough space in the debug vertex buffer.\n"
            "Available: %zu, Needed: %zu",
            VERTEX_LIMIT, byte_offset };
  }
  float *ptr = debug.points_ + debug.point_count_;
  // start Vertex & color
  for (int i = 0; i < 3; i++) { *ptr++ = start[i]; }
  for (int i = 0; i < 3; i++) { *ptr++ = color[i]; }

  // end Vertex & color
  for (int i = 0; i < 3; i++) { *ptr++ = end[i]; }
  for (int i = 0; i < 3; i++) { *ptr++ = color[i]; }

  debug.point_count_ += offset;
  static int run_count = 0;
  if (run_count < 3) {
    size_t count = debug.point_count_;
    LOG_DEBUG("COUNT: %zu", count);
    LOG_ARRAY(debug.points_, count);
    run_count++;
  }
  return Status::STATUS_OK;

}

void Debug::FrameRender() {
  auto& debug = Instance();
  glBindVertexArray(debug.vao_);
  glBindBuffer(GL_ARRAY_BUFFER, debug.vbo_);

  glBufferData(GL_ARRAY_BUFFER,
               PointToBytes(debug.point_count_),
               debug.points_,
               GL_DYNAMIC_DRAW);

  // Set attribute pointers
  Shader *shader = ShaderRegistry::Get(debug.shader_key_).ConsumeOrDie();
  glUseProgram(shader->GetShaderHandle());

  auto at_it = shader->Attributes.find("SV_POSITION");
  assert(at_it != shader->Attributes.end());
  debug.attrib_pos_.Bind(at_it->second.location);

  at_it = shader->Attributes.find("SV_COLOR");
  assert(at_it != shader->Attributes.end());
  debug.attrib_color_.Bind(at_it->second.location);
  int location = -1;

  // We set the transformation uniforms
  auto u_it = shader->Uniforms.find("M_MODEL");
  assert(u_it != shader->Uniforms.end());
  location = u_it->second.location;
  glm::mat4 model(1.0f);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));

  u_it = shader->Uniforms.find("M_VIEW");
  assert(u_it != shader->Uniforms.end());
  location = u_it->second.location;
  glUniformMatrix4fv(location, 1, GL_FALSE, 
      glm::value_ptr(GLOBAL_CAMERA.View()));

  u_it = shader->Uniforms.find("M_PROJ");
  assert(u_it != shader->Uniforms.end());
  location = u_it->second.location;
  glUniformMatrix4fv(location, 1, GL_FALSE,
      glm::value_ptr(GLOBAL_CAMERA.Projection()));


  glDrawArrays(GL_LINES, 0, debug.point_count_);
}






}   // namespace debug
}   // namespace picasso
