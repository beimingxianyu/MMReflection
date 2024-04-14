#include "registration.h"

MM_REGISTER {
  MM::Reflection::Class<void>{"void"};
  MM::Reflection::Class<char>{"char"};
  MM::Reflection::Class<std::uint16_t>{"std::uint16_t"};
  MM::Reflection::Class<std::uint32_t>{"std::uint32_t"};
  MM::Reflection::Class<std::uint64_t>{"std::uint64_t"};
  MM::Reflection::Class<std::int8_t>{"std::int8_t"};
  MM::Reflection::Class<std::int16_t>{"std::int16_t"};
  MM::Reflection::Class<std::int32_t>{"std::int32_t"};
  MM::Reflection::Class<std::int64_t>{"std::int64_t"};
  MM::Reflection::Class<float>{"float"};
  MM::Reflection::Class<double>{"double"};
  MM::Reflection::Class<std::string>{"std::string"};
}
