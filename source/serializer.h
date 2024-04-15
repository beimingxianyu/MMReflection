#pragma once

#include <functional>

#include "database.h"

namespace MM {
namespace Reflection {
struct SerializeDescriptor {
 std::uint32_t version{0};
 std::uint32_t type_name_size{0};
};


class Serializer {
 public:
  using SerializeCheckFunctionType = std::function<bool(const Meta&)>;
  using SerializeFunctionType =
      std::function<DataBuffer&(DataBuffer&, Variable&)>;
  using DeserializeFunctionType = std::function<Variable&(DataBuffer&)>;

 public:
  Serializer() = delete;
  ~Serializer() = default;
  Serializer(const SerializeCheckFunctionType& check_function,
             const SerializeFunctionType& serialize_function,
             const DeserializeFunctionType& deserialize_function);
  Serializer(const Serializer& other) = delete;
  Serializer(Serializer&& other) noexcept = delete;
  Serializer& operator=(const Serializer& other) = delete;
  Serializer& operator=(Serializer&& other) noexcept = delete;

 public:
  bool Check(const Meta& meta) const;

  DataBuffer& Serialize(DataBuffer& data_buffer, Variable& variable) const;

  Variable& Deserialize(DataBuffer& data_buffer) const;

 private:
  SerializeCheckFunctionType check_function_{nullptr};
  SerializeFunctionType serialize_function_{nullptr};
  DeserializeFunctionType deserialize_function_{nullptr};
};
}  // namespace Reflection
}  // namespace MM
