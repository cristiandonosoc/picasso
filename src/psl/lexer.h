/******************************************************************************
 * @file: lexer.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-29
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_PSL_LEXER_H
#define SRC_PSL_LEXER_H

#include <cstring>
#include <string>

#include "utils/printable_enum.h"

namespace picasso {
namespace psl {

PRINTABLE_ENUM(Keywords, 
               ATTRIBUTES, 
               MAPPINGS, 
               UNIFORMS);

PRINTABLE_ENUM(Types, 
               COMMENT, 
               END_OF_FILE, 
               IDENTIFIER, 
               KEYWORD, 
               OPERATOR,
               SCOPE_BEGIN, 
               SCOPE_END,
               SEMICOLOR,
               WHITESPACE);

PRINTABLE_ENUM(Operator,
               PARENTHESIS_OPEN,
               PARENTHESIS_CLOSE,
               COMMA);

PRINTABLE_ENUM(TokenType,
               IDENTIFIER,
               KEYWORD,
               OPERATOR,
               INT);

class Token {
 public:
  Token() {
    // The token starts as 0
    memset(this, 0, sizeof(Token));
  }

 public:
  TokenType type;

 public:
  union {
    char *as_identifier;
    int as_int;
    int as_keyword;
    int as_operator;
  };
};  // class Token

class Location {
 public:
  void AdvanceColumn(size_t count = 1);
  void AdvanceLine();

 public:
  size_t column = 0;
  size_t line = 0;
  size_t position = 0;
};

class Lexer {
 public:
  Lexer(const char *data);

 public:
  Token Read();
  Token Peek();

 public:
  std::string Error() const;

 public:
  void Backup();
  void Restore();

 public:
  size_t Line() const { return location_.line; }
  size_t Column() const { return location_.column; }

 private:
  const char *data_;
  size_t length_;
  std::string error_;
  Location location_;
  Location backup_;
};  // namespace Lexer

}   // namespace psl
}   // namespace picasso



#endif  // SRC_PSL_LEXER_H
