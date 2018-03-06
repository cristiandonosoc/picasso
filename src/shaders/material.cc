/******************************************************************************
 * @file: material.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-05
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "shaders/material.h"
#include "shaders/program.h"

namespace picasso {
namespace shaders {

/**
 * CONSTRUCTORS
 **/
ResultOr<Material::UniquePtr> Material::Create() {
  return UniquePtr(new Material());   // Private Constructor
}

Material::Material(Material&& other) noexcept {
  *this = std::move(other);
}

Material& Material::operator=(Material&& other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
  }
  return *this;
}

/**
 * SHADER INTERFACE
 **/
void Material::SetProgram(Program *program) {
  if (data_.program == program) { return; }
  UnlinkProgram();
  LinkProgram(program);
}

void Material::UnsetProgram() {
  UnlinkProgram();
}

void Material::LinkProgram(Program *program) {
  if (program) {
    program->LinkMaterial(this);

    // Copy over attributes
    for (auto&& it : program->Attributes) {
      data_.attributes[it.first] = it.second;
    }

    // Copy over uniforms
    for (auto&& it : program->Uniforms) {
      data_.uniforms[it.first] = it.second;
    }
  }
  data_.program = program;
}

void Material::UnlinkProgram() {
  if (data_.program) {
    data_.program->UnlinkMaterial(this);
    data_.attributes.clear();
    data_.uniforms.clear();
  }
  data_.program = nullptr;
}

}   // namespace shaders
}   // namespace picasso
