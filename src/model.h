/******************************************************************************
 * @file: model.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-10
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: Base class for models
 ******************************************************************************/

#ifndef SRC_MODEL_H
#define SRC_MODEL_H

#include <map>
#include <string>

#include "shaders/material.h"
#include "utils/dynamic_array.h"

namespace picasso {

using ::picasso::shaders::Material;
using ::picasso::utils::DynamicArray;

class Model {
 public:
  using MaterialMap = std::map<std::string, Material*>;

 public:
  Model() = default;

 public:
  bool Valid() const { return setup_; }

 public:
  void SetVertices(size_t count, GLfloat *vertices);
  void SetIndices(size_t count, GLuint *indices);
  void SetupBuffers();

 public:
  const MaterialMap& Materials = material_map_;
  bool AddMaterial(Material*);
  bool RemoveMaterial(Material*);

 public:
  bool Render() const;

 private:
  MaterialMap material_map_;
  DynamicArray<GLfloat> vertices_;
  GLuint vbo_ = 0;

  DynamicArray<GLuint> indices_;
  GLuint ebo_ = 0;

  GLuint vao_ = 0;

  bool indexed_ = false;
  bool setup_ = false;
};  // class Model

}   // namespace picasso

#endif  // SRC_MODEL_H
