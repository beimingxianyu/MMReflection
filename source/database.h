#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

namespace MM{
namespace Reflection {
const std::string& GetEmptyString();

class Meta;
class Type;
class Variable;
class DataBuffer;

using TypeHashCode = std::uint64_t;
using SerializeFunctionType = std::function<DataBuffer&(DataBuffer&, Variable&)>;
using DeSerializeFunctionType = std::function<Variable&(DataBuffer&)>;

struct TypeID {
  TypeHashCode common_type_hash_code_{0};
  bool is_const_ : 1;
  bool is_refrence_ : 1;

  bool operator==(const TypeID& other) const;

  TypeHashCode GetHashCode() const;
};

std::unordered_map<std::string, const TypeHashCode>&
GetNameToTypeHashDatabase();

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