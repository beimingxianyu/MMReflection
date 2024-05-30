//
// Created by lsj on 24-3-2.
//
#include "database.h"

#include "meta.h"

const std::string& MM::Reflection::GetEmptyString() {
  static std::string empty_string{};
  return empty_string;
}

template<typename T>
struct DatabaseWrapper {
  DatabaseWrapper() = default;
  ~DatabaseWrapper() {
    for (auto& element: database_) {
      delete element.second;
    }
  }

  T database_{};
};

std::unordered_map<std::size_t, MM::Reflection::Meta*>&
MM::Reflection::GetMetaDatabase() {
  // static std::unordered_map<std::size_t, Meta*> g_meta_database;
  static DatabaseWrapper<std::unordered_map<std::size_t, Meta*>> g_meta_database;

  return g_meta_database.database_;
}

std::unordered_map<MM::Reflection::TypeID, const MM::Reflection::Type*>&
MM::Reflection::GetTypeDatabase() {
  // static std::unordered_map<TypeID, const Type*> g_type_database;
  static DatabaseWrapper<std::unordered_map<TypeID, const Type*>> g_type_database;

  return g_type_database.database_;
}

bool MM::Reflection::TypeID::operator==(const TypeID& other) const {
  return common_type_hash_code_ == other.common_type_hash_code_ &&
         is_top_const_ == other.is_top_const_ &&
         is_l_refrence_ == other.is_l_refrence_ &&
         is_r_refrence_ == other.is_r_refrence_ &&
         is_low_const_ == other.is_low_const_;
}

MM::Reflection::TypeHashCode MM::Reflection::TypeID::GetHashCode() const {
  static TypeHashCode hash_code_change =
      static_cast<TypeHashCode>(0xAAAAAAAAAAAA);
  constexpr TypeHashCode mask1 =
      static_cast<TypeHashCode>(0xFFFF000000000000);
  constexpr TypeHashCode mask2 =
      static_cast<TypeHashCode>(0x0000FFFF00000000);
  constexpr TypeHashCode mask3 =
      static_cast<TypeHashCode>(0x00000000FFFF0000);
  constexpr TypeHashCode mask4 =
      static_cast<TypeHashCode>(0x000000000000FFFF);
  TypeHashCode result = common_type_hash_code_;
  if (is_top_const_) {
    const TypeHashCode temp = result ^ hash_code_change & mask1;
    result &= ~mask1;
    result |= temp;
  }
  if (is_low_const_) {
    const TypeHashCode temp = result ^ hash_code_change & mask2;
    result &= ~mask2;
    result |= temp;
  }
  if (is_l_refrence_) {
    const TypeHashCode temp = result ^ hash_code_change & mask3;
    result &= ~mask3;
    result |= temp;
  }
  if (is_r_refrence_) {
    const TypeHashCode temp = result ^ hash_code_change & mask4;
    result &= ~mask4;
    result |= temp;
  }

  return result;
}

std::unordered_map<std::string, const MM::Reflection::TypeHashCode>&
MM::Reflection::GetNameToTypeHashDatabase() {
  static std::unordered_map<std::string, const TypeHashCode>
      name_to_type_hash_data_base{};

  return name_to_type_hash_data_base;
}

std::unordered_map<std::string, MM::Reflection::SerializerBase*>&
MM::Reflection::GetSerializerDatabase() {
  static DatabaseWrapper<std::unordered_map<std::string, SerializerBase*>> g_serializer_database;

  return g_serializer_database.database_;
}

MM::Reflection::TypeHashCode std::hash<MM::Reflection::TypeID>::operator()(
    const MM::Reflection::TypeID& other) const noexcept {
  return other.GetHashCode();
}
