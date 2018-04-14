/******************************************************************************
 * @file: mesh.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-25
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_ASSETS_MESH_H
#define SRC_ASSETS_MESH_H

#include <map>
#include <string>

#include "utils/macros.h"
#include "models/attrib_pointer.h"
#include "assets/materials/material_registry.h"
#include "utils/dynamic_array.h"
#include "models/transform.h"

#include "camera.h"


namespace picasso {
namespace assets {


using ::picasso::models::AttributeKind;
using ::picasso::models::Transform;
using ::picasso::models::AttributePointer;

using ::picasso::assets::materials::Material;
using ::picasso::assets::materials::MaterialRegistry;
using ::picasso::utils::DynamicArray;

using ::picasso::Camera;

class Mesh {
 public:
  using AttributePointerMap = std::map<AttributeKind, AttributePointer>;

 public:
  DEFINE_PTR_TYPES(Mesh);


 public:
  Mesh() = default;

 public:
  bool Valid() const { return setup_; }

 public:
  void SetVertexBuffer(size_t count, GLfloat *vertices);
  void SetIndexBuffer(size_t count, GLuint *indices);
  bool SetupBuffers();

 private:
  GLuint SetupMaterialVAO(const MaterialRegistry::KeyType&);
  bool SetupAttributeByAttributeKind(Material*, AttributeKind);

 public:
  const AttributePointerMap& AttributePointers = attribute_pointer_map_;
  // TODO(Cristian): Use Status
  bool AddAttributePointer(const AttributePointer&);
  bool RemoveAttributePointer(const AttributePointer&);

 public:
  bool Render(const Transform& transform, const Camera&, const Material&) const;

 private:
  AttributePointerMap attribute_pointer_map_;

  DynamicArray<GLfloat> vertex_buffer_;
  GLuint vbo_ = 0;
  GLuint vao_ = 0;

  DynamicArray<GLuint> index_buffer_;
  GLuint ebo_ = 0;

  bool indexed_ = false;
  bool setup_ = false;
};  // class Mesh

}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_MESH_H
