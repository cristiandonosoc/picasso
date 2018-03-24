/******************************************************************************
 * @file: model.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-11
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/


#include <SDL.h>
#include <algorithm>

#include "logging/log.h"
#include "models/model.h"
#include "shaders/shader.h"
#include "utils/macros.h"

BEGIN_IGNORE_WARNINGS();
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
END_IGNORE_WARNINGS();



namespace picasso {
namespace models {

// TODO(Cristian): Move to a better place
//                 And make it configurable through the UI or INI file
static std::map<AttributeKind, std::string> AttributeKindToAttributeName = {
  { AttributeKind::VERTEX, "SV_POSITION" },
  { AttributeKind::COLOR, "SV_COLOR" },
  { AttributeKind::UV, "SV_UV" }
};

using ::picasso::shaders::Shader;
using ::picasso::shaders::Material;

void Model::SetVertexBuffer(size_t count, GLfloat *vertices) {
  vertex_buffer_.Reset(count, vertices);
}

void Model::SetIndexBuffer(size_t count, GLuint *indices) {
  index_buffer_.Reset(count, indices);
}

namespace {

void BindAttributePointer(int location, const AttributePointer& attrib_pointer) {
  glVertexAttribPointer(
      location,  // Attribute location int he shader
      // Size: How many types are per attrib "element".
      //       Effectivelly this is saying where we have
      //       a vec, vec2, vec3 or vec4
      attrib_pointer.GetSize(),    
      // Type of value this attribute is (GL_FLOAT, GL_DOUBLE, etc)
      attrib_pointer.GetType(), 
      // Whether to normalize the data
      attrib_pointer.GetNormalize(),
      // Stride: Space in bytes between two consecutive values of this 
      //         attribute in the array. 
      // A value of 0 is saying that the buffer is densely packed.
      attrib_pointer.GetStride(),    
      // Offset: Space in bytes from the start of the array where the
      //        first value is. 
      // Value starts from the beginning of the array
      (void*)(size_t)attrib_pointer.GetOffset());
}

}   // namespace

bool Model::SetupBuffers() {
  if (setup_) {
    LOG_WARN("Model already setup!");
    return false;
  }

  if (vertex_buffer_.Count() == 0) {
    LOG_WARN("Model without Vertex Buffer set up");
    return false;
  }

  // We add the vertices to the GPU
  
  // 1. VBO
  //    VBO stands for Vertex Buffer Object. 
  //    This is a buffer that contains vertices data.
  

  // 1.1  We create a buffers. In OpenGL, you create buffers that get binded to
  //      certain types of buffers. The in received is the "name" of the buffer
  //      that we are generating. They are unbinded (have no type) at first.
  glGenBuffers(1, &vbo_);
  
  // 1.2 Bind it to a particular type. In this case to GL_ARRAY_BUFFER.
  //     This will make vbo into a VERTEX BUFFER OBJECT (hence the name).
  //     When a buffer is binded some things happen:
  //     - Any previous binded data of another type is discarded
  //     - If the buffer wasn't binded to this type, a new array of that
  //       type is created
  //     - This buffer is the active one for this type.
  //       This means that any future calls that affect GL_ARRAY_BUFFER will
  //       now affect the one binded to the vbo, until another binding occurs.
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  
  // 1.3 Send data to the active buffer (vbo in this case).
  //     This call will scale the buffer to the size specified here.
  //     Any previous data associated with the buffer will be discarded.
  glBufferData(GL_ARRAY_BUFFER,         //  Buffer being sent data
               vertex_buffer_.Size(),   // New size of the buffer
               vertex_buffer_.Get(),    // Pointer to the array to send
               GL_STATIC_DRAW);         // Type of memory storing the data

  // 2. [OPTIONAL] 
  //  OpenGL now has a buffer with data. But it can be when drawing
  //  primitives that a lot of them averlap (ie. a rectangle made out of
  //  2 triangles share 2 of the 3 vertices). It's possible to tell OpenGL
  //  to do indexing drawing. Basically you send a set of points and then
  //  use an array of indices to expand that set to the actual primitives
  //  needed. This saves space and bandwidth because indices are only one int
  //  long, while a vertex can be quite large, depending on the amount of
  //  data associated with it being accessed through the attributes
  //  (position, color, uv, normals, etc.)
  //
  //  The object used for holding the indices is called an
  //  ELEMENT BUFFER OBJECT, or EBO
  //
  //  In order to render with indices, we simply need to bind an EBO
  //  and call glDrawElements instead of glDrawArrays
  if (index_buffer_.Count() > 0) {
    // 2.1 Create the EBO buffer (Same as VBO)
    glGenBuffers(1, &ebo_);

    // 2.2 Bind the EBO. This DOES NOT unbind the VBO because we are
    //     binding a different kind of buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    // 2.3 Send the indices data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 index_buffer_.Size(), 
                 index_buffer_.Get(), 
                 GL_STATIC_DRAW);
    indexed_ =  true;
  }

  // 3. OpenGL has buffer with vertices in it, but has no idea how to interpret them
  //    (which ones are colors, which ones positions, uv, etc.).
  //    For that we need to tell which attribute holds which information where.
  //    That configuration can (and should) be stored in an object that can then
  //    be binded to the current buffer so that OpenGL knows how to interpret it.
  //    These objects are called VERTEX ATTRIBUTE OBJECTS
  //
  //    This is a mapping to a particular location, so we need a shader to which
  //    to know the locations to.
  //    This means that this is a per material object.
  for (const MaterialRegistry::Key& key : material_keys_) {
    GLuint vao = SetupMaterialVAO(key);
    // We store it in the map
    material_vao_map_[key] = vao;
  }
  
  setup_ = true;
  return true;
}

GLuint Model::SetupMaterialVAO(const MaterialRegistry::Key& key) {
  // 3.1 Generate the VAO buffer
  GLuint vao = 0;
  glGenVertexArrays(1, &vao); 

  // 3.2 Bind the current VAO
  glBindVertexArray(vao);

  // From this moment on, the current state of the buffers is saved on the VAO
  // We could leave it as it is, but being explicit is never bad
  
  // 3.3 Bind the VBO to the VAO
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  // [OPTIONAL] 3.4 Bind the EBO. 
  //  0 unbinds, so if we never had indices, this will effectively unbind it
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

  // 4. Set the vertex attributes.
  //    Attributes are inputs for the vertex shader that come from the 
  //    GL_ARRAY_BUFFER. But the GL_ARRAY_BUFFER can store a lot of
  //    information together (Position, Normal, Color, UV, etc.)
  //    The Vertex Attributes Pointers are the way OpenGL gets to know
  //    how to obtain the information from the GL_ARRAY_BUFFER.
  //    You can think this as the key to decode the "multiplexing"
  //    of the GL_ARRAY_BUFFER.
  //    These calls are also recorded in the VAO, so we only 
  //    need to do it once every vertex setup.
  
  // 4.1 Declare the vertex attribute
  //
  // For this we need the locations in the shader, and we obtain
  // that through the material
  Material *material = MaterialRegistry::Get(key);
  if (material) {
    for (auto&& kind_it : AttributeKind::GetEnumMap()) {
      SetupAttributeByAttributeKind(material, kind_it.first);
    }
  } else {
    // TODO(Cristian): Log? At least show in the UI...
  }

  // 4.2 Enable the attribute

  // 5. Disable all the GL state so that subsequent calls
  //    separete from the models don't modify the state
  // IMPORTANT(Cristian): Remember to unbind the VAO first!!!!!!
  //          Otherwise the other unbinding will get set in the VAO too
  //          and you *will* lose a lot of time debugging it
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return vao;
}

bool Model::SetupAttributeByAttributeKind(Material* material, AttributeKind kind)  {
  auto pos_it = attribute_pointer_map_.find(kind);
  if (pos_it == attribute_pointer_map_.end()) {
    LOG_ERROR("Model doesn't have \"%s\" attribute pointer",
              AttributeKind::ToString(kind).c_str());
    return false;
  }
  const AttributePointer& attrib_pointer = pos_it->second;

  // We get the name we should be looking for
  std::string attrib_name = AttributeKindToAttributeName[kind];
  if (attrib_name.empty()) {
    LOG_ERROR("There is no attribute name map for the kind: \"%s\"",
              AttributeKind::ToString(kind).c_str());
    return false;
  }

  // We see if we actually have the attribute mapping
  auto it = material->Attributes.find(attrib_name);
  if (it == material->Attributes.end()) {
    LOG_ERROR("Model doesn't have an %s attribute specification",
              AttributeKind::ToString(kind).c_str());
    return false;
  }

  GLuint location = it->second.GetVariable()->GetLocation();

  // We finally are able to allocate the attribte to the location
  LOG_DEBUG("Found attribute for %s", AttributeKind::ToString(kind).c_str());
  attrib_pointer.DebugPrint();
  BindAttributePointer(location, attrib_pointer);
  glEnableVertexAttribArray(location);
  return true;
}

bool Model::AddMaterialKey(const MaterialRegistry::Key& key) {
  // TODO(Cristian): Send error message
  auto it = find(material_keys_.begin(), material_keys_.end(), key);
  if (it != material_keys_.end()) {
    return false; // Already got it
  }
  material_keys_.push_back(key);
  return true;
}

bool Model::RemoveMaterialKey(const MaterialRegistry::Key& key) {
  auto it = find(material_keys_.begin(), material_keys_.end(), key);
  if (it == material_keys_.end()) {
    return false;   // Cannot find it
  }
  material_keys_.erase(it);
  return true;
}

// TODO(Cristian): Use Status
bool Model::AddAttributePointer(const AttributePointer& attrib_pointer) {
  auto it = attribute_pointer_map_.find(attrib_pointer.GetKind());
  if (it != attribute_pointer_map_.end()) {
    return false;
  }

  attribute_pointer_map_[attrib_pointer.GetKind()] = attrib_pointer;
  return true;
}

// TODO(Cristian): Use Status
bool Model::RemoveAttributePointer(const AttributePointer& attrib_pointer) {
  auto it = attribute_pointer_map_.find(attrib_pointer.GetKind());
  if (it == attribute_pointer_map_.end()) {
    return false;
  }
  attribute_pointer_map_.erase(it);
  return true;
}

bool Model::Render() const {
  if (!setup_) {
    // TODO(Cristian): Send error message
    LOG_WARN("Calling render on an not ready model");
    return false;
  }

  if (MaterialKeys.empty()) {
    // TODO(Cristian): Send error message
    LOG_WARN("Calling render on a model without materials");
    return false;
  }

  for (const MaterialRegistry::Key& key : MaterialKeys) {
    Material *material = MaterialRegistry::Get(key);
    const Shader *shader = material->GetShader();

    if (!shader) { 
      LOG_WARN("Rendering with null shader");
      continue; 
    }
    if (!shader->Valid()) {
      LOG_WARN("Rendering with an invalid shader: %s",
                  shader->GetName().c_str());
      continue;
    }

    // We setup the program
    glUseProgram(shader->GetShaderHandle());

    // We bind our set VAO
    auto it = material_vao_map_.find(key);
    if (it != material_vao_map_.end()) {
      glBindVertexArray(it->second);
    }

    int location = -1;
    glm::mat4 trans;

    // We set the matrix
    location = material->Uniforms.find("M_MODEL")->second.GetVariable()->GetLocation();
    trans = glm::mat4(1.0f);  // uniform
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));

    location = material->Uniforms.find("M_VIEW")->second.GetVariable()->GetLocation();
    trans = glm::mat4(1.0f);  // uniform
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));

    location = material->Uniforms.find("M_PROJ")->second.GetVariable()->GetLocation();
    trans = glm::mat4(1.0f);  // uniform
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));


    // We bind the uniforms
    int texture_unit_count = 0;
    for (auto&& u_it : material->Uniforms) {
      u_it.second.SendValue(&texture_unit_count);
    }


    if (indexed_) {
      // TODO(Cristian): Actually calculate the sizes
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

  }

  // Unbind
  glUseProgram(0);
  glBindVertexArray(0);
  return true;
}

}   // namespace models
}   // namespace picasso
