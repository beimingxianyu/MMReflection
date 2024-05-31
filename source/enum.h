#pragma once

#include <cstdint>
#include <string>

namespace MM {
namespace Reflection {
class Meta;
class Type;

using EnumValue = std::uint64_t;

struct EnumPair {
  std::string enum_name_;
  EnumValue value_;

  friend bool operator==(const EnumPair& lhs, const EnumPair& rhs) {
    return lhs.enum_name_ == rhs.enum_name_ && lhs.value_ == rhs.value_;
  }

  friend bool operator!=(const EnumPair& lhs, const EnumPair& rhs) {
    return !(lhs == rhs);
  }
};

class EnumItem {
public:
  friend class Meta;

public:
  EnumItem(const Meta* meta, const std::string& enum_name);

public:
  bool IsValid() const;

 EnumValue GetValue() const;

  std::uint8_t GetSize() const;

  const std::string& GetEnumName() const;

  const Type* GetType() const;

  const Meta* GetMeta() const;

private:
  EnumItem() = default;
  EnumItem(const Meta* meta, const EnumPair* pair);

private:
  const Meta* meta_{nullptr};
  const EnumPair* pair_{nullptr};
};
}
}

namespace std {
template<>
struct hash<MM::Reflection::EnumPair> {
std::uint64_t operator()(
      const MM::Reflection::EnumPair& value) const noexcept;
};
}