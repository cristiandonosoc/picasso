/******************************************************************************
 * @file: material.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_SHADERS_MATERIAL_H
#define SRC_SHADERS_MATERIAL_H

#include "shaders/variable.h"
#include "utils/macros.h"
#include "utils/result.h"

#include <map>

namespace picasso {

using namespace utils;

namespace shaders {

class Program;   // Forward Declaration

class Material {
 public:
   DEFINE_PTR_TYPES(Material);
  
 private:
  static ResultOr<UniquePtr> Create();

 private:
  Material() {}

 public:
  void SetProgram(Program *);
  void UnsetProgram();

 private:
  void LinkProgram(Program *);
  void UnlinkProgram();

 public:
  DISABLE_COPY(Material);
  Material(Material&&) noexcept;
  Material& operator=(Material&&) noexcept;

 private:
  class Data {
   public:
    Program *program;
    std::map<std::string, Variable> attributes;
    std::map<std::string, Variable> uniforms;
   public:
    friend class Material;
  } data_;

 public:
  friend class MaterialRegistry;
};

}   // namepsace shaders
}   // namespace picasso

#endif  // SRC_SHADERS_MATERIAL_H
