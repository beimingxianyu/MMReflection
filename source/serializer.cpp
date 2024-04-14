#include "serializer.h"

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