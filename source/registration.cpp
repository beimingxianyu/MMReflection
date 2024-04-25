#include "registration.h"

namespace MM {
namespace Reflection {
char GetEmptyCharObject() {
  return char{0};
}

std::uint16_t GetEmptyUint16Object() {
  return std::uint16_t{0};
}

std::uint32_t GetEmptyUint32Object() {
  return std::uint32_t{0};
}

std::uint64_t GetEmptyUint64Object() {
  return std::uint64_t{0};
}

std::int8_t GetEmptyInt8Object() {
  return std::int8_t{0};
}

std::int16_t GetEmptyInt16Object() {
  return std::int16_t{0};
}

std::int32_t GetEmptyInt32Object() {
  return std::int32_t{0};
}

std::int64_t GetEmptyInt64Object() {
  return std::int64_t{0};
}

float GetEmptyFloatObject() {
  return float{0.0};
}

double GetEmptyDoubleObject() {
  return double{0.0};
}

std::string GetEmptyStringObject() {
  return std::string("");
}
}
}

MM_REGISTER {
  MM::Reflection::Class<void>{"void"};

  MM::Reflection::Class<char>{"char"}
    .Constructor<>("Empty")
    .Constructor<char>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyCharObject)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<std::uint16_t>{"std::uint16_t"}
    .Constructor<>("Empty")
    .Constructor<std::uint16_t>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyUint16Object)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<std::uint32_t>{"std::uint32_t"}
    .Constructor<>("Empty")
    .Constructor<std::uint32_t>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyUint32Object)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<std::uint64_t>{"std::uint64_t"}
    .Constructor<>("Empty")
    .Constructor<std::uint64_t>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyUint64Object)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<std::int8_t>{"std::int8_t"}
    .Constructor<>("Empty")
    .Constructor<std::int8_t>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyInt8Object)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<std::int16_t>{"std::int16_t"}
    .Constructor<>("Empty")
    .Constructor<std::int16_t>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyInt16Object)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<std::int32_t>{"std::int32_t"}
    .Constructor<>("Empty")
    .Constructor<std::int32_t>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyInt32Object)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<std::int64_t>{"std::int64_t"}
    .Constructor<>("Empty")
    .Constructor<std::int64_t>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyInt64Object)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<float>{"float"}
    .Constructor<>("Empty")
    .Constructor<float>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyFloatObject)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<double>{"double"}
    .Constructor<>("Empty")
    .Constructor<double>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyDoubleObject)
    .SetSerializerName(MM::Reflection::TrivialSerializer::GetSerializerNameStatic());

  MM::Reflection::Class<std::string>{"std::string"}
    .Constructor<>("Empty")
    .Constructor<std::string>("Init")
    .Method(MM::Reflection::Meta::GetEmptyObjectMethodName(), &MM::Reflection::GetEmptyStringObject);

  MM::Reflection::RegisterSerializer<MM::Reflection::TrivialSerializer>();
  MM::Reflection::RegisterSerializer<MM::Reflection::RecursionSerializer>();
}
