/******************************************************************************
 * @file: lexer.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-29
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include "psl/lexer.h"

namespace picasso {
namespace psl {

namespace {

  class OperatorInfo {
   public:
    Operator op;
    int precedence;
  };  // struct OperatorInfo

  OperatorInfo OperatorTable[] = {
    { Operator::PARENTHESIS_OPEN,   700 },
    { Operator::PARENTHESIS_CLOSE,  700 },
    { Operator::COMMA,                1 },
  };



}   // namespace




}   // namespace psl
}   // namespace picasso
