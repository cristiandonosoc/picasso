
/******************************************************************************
 * @file: model.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-11
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_MODELS_MODEL_H
#define SRC_MODELS_MODEL_H

#include <map>
#include <string>

#include "models/attrib_pointer.h"
#include "shaders/material_registry.h"
#include "utils/dynamic_array.h"

namespace picasso {
namespace models {


using ::picasso::shaders::Material;
using ::picasso::shaders::MaterialRegistry;
using ::picasso::utils::DynamicArray;

class Model {
 public:
  using MaterialKeysContainer = std::vector<MaterialRegistry::Key>;
  using AttributePointerMap = std::map<AttributeKind, AttributePointer>;

 public:
  Model() = default;

 public:
  bool Valid() const { return setup_; }

 public:
  void SetVertexBuffer(size_t count, GLfloat *vertices);
  void SetIndexBuffer(size_t count, GLuint *indices);
  bool SetupBuffers();

 private:
  GLuint SetupMaterialVAO(const MaterialRegistry::Key&);
  bool SetupAttributeByAttributeKind(Material*, AttributeKind);

 public:
  const AttributePointerMap& AttributePointers = attribute_pointer_map_;
  // TODO(Cristian): Use Status
  bool AddAttributePointer(const AttributePointer&);
  bool RemoveAttributePointer(const AttributePointer&);

 public:
  const MaterialKeysContainer& MaterialKeys = material_keys_;
  // TODO(Cristian): Use Status
  bool AddMaterialKey(const MaterialRegistry::Key&);
  bool RemoveMaterialKey(const MaterialRegistry::Key&);

 public:
  bool Render() const;

 private:
  AttributePointerMap attribute_pointer_map_;
  std::vector<MaterialRegistry::Key> material_keys_;
  std::map<MaterialRegistry::Key, GLuint> material_vao_map_;

  DynamicArray<GLfloat> vertex_buffer_;
  GLuint vbo_ = 0;

  DynamicArray<GLuint> index_buffer_;
  GLuint ebo_ = 0;

  bool indexed_ = false;
  bool setup_ = false;
};  // class Model

}   // namespace models
}   // namespace picasso

#endif  // SRC_MODELS_MODEL_H
