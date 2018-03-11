/******************************************************************************
 * @file: value.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-10
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <cassert>

#include "shaders/value.h"

namespace picasso {
namespace shaders {

Value::Value(const Variable *variable) : variable_(variable) {
  assert(variable);
  if (variable->GetKind() == VariableKind::UNIFORM) {
    // We allocate the amount of data needed for the value
    backend_size_ = variable->GetTypeSize() * variable->GetSize();
    backend_.reset(new uint8_t[backend_size_]);
  }
}
 

}   // namespace shaders 
}   // namespace picasso
