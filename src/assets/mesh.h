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
#include "shaders/material_registry.h"
#include "utils/dynamic_array.h"
#include "models/transform.h"

using ::picasso::models::Transform;
using ::picasso::models::AttributeKind;
using ::picasso::models::AttributePointer;

namespace picasso {
namespace assets {

using ::picasso::shaders::Material;
using ::picasso::shaders::MaterialKey;
using ::picasso::utils::DynamicArray;

class Mesh {
 public:
  DEFINE_PTR_TYPES(Mesh);

 public:
  using MaterialKeysContainer = std::vector<MaterialKey>;
  using AttributePointerMap = std::map<AttributeKind, AttributePointer>;

 public:
  Mesh() = default;

 public:
  bool Valid() const { return setup_; }

 public:
  void SetVertexBuffer(size_t count, GLfloat *vertices);
  void SetIndexBuffer(size_t count, GLuint *indices);
  bool SetupBuffers();

 private:
  GLuint SetupMaterialVAO(const MaterialKey&);
  bool SetupAttributeByAttributeKind(Material*, AttributeKind);

 public:
  const AttributePointerMap& AttributePointers = attribute_pointer_map_;
  // TODO(Cristian): Use Status
  bool AddAttributePointer(const AttributePointer&);
  bool RemoveAttributePointer(const AttributePointer&);

 public:
  const MaterialKeysContainer& MaterialKeys = material_keys_;
  // TODO(Cristian): Use Status
  bool AddMaterialKey(const MaterialKey&);
  bool RemoveMaterialKey(const MaterialKey&);

 public:
  bool Render(Material*) const;

 private:
  Transform transform_;

  AttributePointerMap attribute_pointer_map_;
  std::vector<MaterialKey> material_keys_;
  std::map<MaterialKey, GLuint> material_vao_map_;

  DynamicArray<GLfloat> vertex_buffer_;
  GLuint vbo_ = 0;

  DynamicArray<GLuint> index_buffer_;
  GLuint ebo_ = 0;

  bool indexed_ = false;
  bool setup_ = false;
};  // class Mesh

}   // namespace assets
}   // namespace picasso

#endif  // SRC_ASSETS_MESH_H
