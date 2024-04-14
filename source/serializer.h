#pragma once

#include <functional>

#include "database.h"

namespace MM {
namespace Reflection {
class Serializer {
 public:
  using SerializeCheckFunctionType = std::function<bool(const Meta&)>;
  using SerializeFunctionType =
      std::function<DataBuffer&(DataBuffer&, Variable&)>;
  using DeserializeFunctionType = std::function<Variable&(DataBuffer&)>;

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
