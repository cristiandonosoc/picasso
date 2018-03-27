/******************************************************************************
 * @file: printable_enum.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-11
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: 
 * This macro will create a Enum class that wraps the actual enum
 * implementation. The cool thing about this class is that it behaves
 * likes an enum but has a couple of nice static functions associated with
 * it:
 *
 * GetMap() returns a map from the internal type (the actual enum) to a string
 *          with it's name. This will enable us to print the whole enum
 *          Because enums are implemented as ints, they will be always be
 *          sorted by priority.
 * ToString(Enum): Gets the string of a given enum.
 *
 * LIMITATIONS:
 * Currently the macro doesn't support adding explicit numbers,
 * just enum options. But the technique used could be expanded if needed.
 ******************************************************************************/

#ifndef SRC_UTILS_PRINTABLE_ENUM_H
#define SRC_UTILS_PRINTABLE_ENUM_H

#include <cassert>
#include <map>
#include <string>
#include <type_traits>

#include "utils/macros.h"
#include "utils/map_macro.h"
#include "utils/status.h"

// TODO(Cristian): Actually write some unit tests for this

/**
 * The actual class generated has a public InternalEnum
 * with the actual options. Because enums somehow "reveal"
 * their options to the outside, this enables us to us
 * EnumType::<OPTION> instead of the more technically correct
 * EnumType::InternalEnum::<OPTION> (which also works).
 *
 * There is also an internal mapping class that holds the 
 * actual map. We need this because we use the function
 * static trick to create only one singleton instance
 * and use that unique instance's constructor to 
 * populate the map with the options.
 *
 * The other required part is the MAP_ARG_ALL macro
 * that enables us to map a macro to each __VA_ARGS__.
 * That macro is defined and explained in utils/map_macro.h
 *
 * With this we can call ADD_TO_MAPPING (defined later this file),
 * which actually ouputs the code that will add each string
 * into the map.
 */
#define PRINTABLE_ENUM(EnumType, ...)                                         \
  class EnumType final {                                                      \
   public:                                                                    \
    enum InternalEnum { __VA_ARGS__ } ;                                       \
                                                                              \
   public:                                                                    \
    EnumType() = default;                                                     \
    DEFAULT_COPY(EnumType);                                                   \
    DEFAULT_MOVE(EnumType);                                                   \
                                                                              \
   public:                                                                    \
    EnumType(InternalEnum val) : val_(val) {}                                 \
    EnumType& operator=(InternalEnum val) {                                   \
      val_ = val;                                                             \
      return *this;                                                           \
    }                                                                         \
    operator InternalEnum() const { return val_; }                            \
                                                                              \
   public:                                                                    \
    using EnumMap = std::map<InternalEnum, std::string>;                      \
    using StringMap = std::map<std::string, InternalEnum>;                    \
                                                                              \
   private:                                                                   \
    class Mapping final {                                                     \
     private:                                                                 \
      Mapping() {                                                             \
        MAP_ARG_ALL(ADD_TO_MAPPING, EnumType, __VA_ARGS__)                    \
      }                                                                       \
     private:                                                                 \
      EnumMap enum_map_;                                                      \
      StringMap string_map_;                                                  \
     public:                                                                  \
      friend class EnumType;                                                  \
    };                                                                        \
                                                                              \
    static const Mapping& MappingInstance() {                                 \
      static Mapping mapping;                                                 \
      return mapping;                                                         \
    }                                                                         \
                                                                              \
   public:                                                                    \
    static const EnumMap& GetEnumMap() {                                      \
      return MappingInstance().enum_map_;                                     \
    }                                                                         \
                                                                              \
    static const std::string& ToString(EnumType val) {                        \
      const Mapping& mapping = MappingInstance();                             \
      auto it = mapping.enum_map_.find(val);                                  \
      assert(it != mapping.enum_map_.end());                                  \
      return it->second;                                                      \
    }                                                                         \
                                                                              \
   public:                                                                    \
    static const StringMap& GetStringMap() {                                  \
      const Mapping& mapping = MappingInstance();                             \
      return mapping.string_map_;                                             \
    }                                                                         \
                                                                              \
    static ::picasso::utils::StatusOr<InternalEnum>                           \
    FromString(const std::string& val) {                                      \
      const Mapping& mapping = MappingInstance();                             \
      auto it = mapping.string_map_.find(std::string(#EnumType) + val);       \
      if (it == mapping.string_map_.end()) {                                  \
        using STOR = ::picasso::utils::StatusOr<InternalEnum>;                \
        return { STOR::STATUS_ERROR, "Cannot find \"%s\"", val.c_str() };     \
      }                                                                       \
      return it->second;                                                      \
    }                                                                         \
                                                                              \
   private:                                                                   \
    InternalEnum val_;                                                        \
  };                                                                          \


/**
 * This is the function mapped foreach __VA_ARGS__ that are the 
 * enum options. It's basically creating the correct string that's
 * added to the correct key from the pre-coordinated map name
 * available in the context.
 */
#define ADD_TO_MAPPING(enum_name, field) \
  std::string enum_name ## field = #enum_name "::" #field; \
  enum_map_[enum_name::field] = enum_name ## field; \
  string_map_[enum_name ## field] = enum_name::field;

#endif  // SRC_UTILS_PRINTABLE_ENUM_H
