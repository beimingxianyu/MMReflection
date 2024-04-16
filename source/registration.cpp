#include "registration.h"

MM_REGISTER {
  MM::Reflection::Class<void>{"void"};
  MM::Reflection::Class<char>{"char"}.Constructor<>("Empty").Constructor<char>("Init");
  MM::Reflection::Class<std::uint16_t>{"std::uint16_t"}.Constructor<>("Empty").Constructor<std::uint16_t>("Init");
  MM::Reflection::Class<std::uint32_t>{"std::uint32_t"}.Constructor<>("Empty").Constructor<std::uint32_t>("Init");
  MM::Reflection::Class<std::uint64_t>{"std::uint64_t"}.Constructor<>("Empty").Constructor<std::uint64_t>("Init");
  MM::Reflection::Class<std::int8_t>{"std::int8_t"}.Constructor<>("Empty").Constructor<std::int8_t>("Init");
  MM::Reflection::Class<std::int16_t>{"std::int16_t"}.Constructor<>("Empty").Constructor<std::int16_t>("Init");
  MM::Reflection::Class<std::int32_t>{"std::int32_t"}.Constructor<>("Empty").Constructor<std::int32_t>("Init");
  MM::Reflection::Class<std::int64_t>{"std::int64_t"}.Constructor<>("Empty").Constructor<std::int64_t>("Init");
  MM::Reflection::Class<float>{"float"}.Constructor<>("Empty").Constructor<float>("Init");
  MM::Reflection::Class<double>{"double"}.Constructor<>("Empty").Constructor<double>("Init");
  MM::Reflection::Class<std::string>{"std::string"}.Constructor<>("Empty").Constructor<std::string>("Init");
}
