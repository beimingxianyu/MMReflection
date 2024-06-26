#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace MM{
namespace Reflection {
const std::string& GetEmptyString();

class Meta;
class Type;
class Variable;
class DataBuffer;
class SerializerBase;

using TypeHashCode = std::uint64_t;


struct TypeID {
  TypeHashCode common_type_hash_code_{0};
  bool is_top_const_ : 1;
  bool is_low_const_ : 1;
  bool is_l_refrence_ : 1;
  bool is_r_refrence_ : 1;

  bool operator==(const TypeID& other) const;

  TypeHashCode GetHashCode() const;
};

std::unordered_map<std::string, const TypeHashCode>&
GetNameToTypeHashDatabase();

std::unordered_map<std::string, SerializerBase*>& GetSerializerDatabase();

/**
 * \brief The Database of all registered metadata.
 */
std::unordered_map<TypeHashCode, Meta*>& GetMetaDatabase();

std::unordered_map<TypeID, const Type*>& GetTypeDatabase();

}  // namespace Reflection
}

template<>
struct std::hash<MM::Reflection::TypeID> {
  MM::Reflection::TypeHashCode operator()(const MM::Reflection::TypeID& other) const noexcept;
};