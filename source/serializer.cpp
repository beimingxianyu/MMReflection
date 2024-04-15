#include "serializer.h"

MM::Reflection::Serializer::Serializer(
    const SerializeCheckFunctionType& check_function,
    const SerializeFunctionType& serialize_function,
    const DeserializeFunctionType& deserialize_function)
    : check_function_(check_function),
      serialize_function_(serialize_function),
      deserialize_function_(deserialize_function) {}

bool MM::Reflection::Serializer::Check(const Meta& meta) const {
  return check_function_(meta);
}

MM::Reflection::DataBuffer& MM::Reflection::Serializer::Serialize(
    DataBuffer& data_buffer, Variable& variable) const {
  return serialize_function_(data_buffer, variable);
}

MM::Reflection::Variable& MM::Reflection::Serializer::Deserialize(
    DataBuffer& data_buffer) const {
  return deserialize_function_(data_buffer);
}