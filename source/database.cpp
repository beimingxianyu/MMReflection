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
         is_const_ == other.is_const_ && is_refrence_ == other.is_refrence_;
}

MM::Reflection::TypeHashCode MM::Reflection::TypeID::GetHashCode() const {
  static TypeHashCode hash_code_change =
      static_cast<TypeHashCode>(0xAAAAAAAAAAAA);
  constexpr TypeHashCode upper_mask =
      static_cast<TypeHashCode>(0xFFFFFFFF00000000);
  constexpr TypeHashCode lower_mask =
      static_cast<TypeHashCode>(0x00000000FFFFFFFF);
  TypeHashCode result = common_type_hash_code_;
  if (is_const_) {
    TypeHashCode temp = result ^ hash_code_change & lower_mask;
    result &= upper_mask;
    result |= temp;
  }
  if (is_refrence_) {
    TypeHashCode temp = result ^ hash_code_change & upper_mask;
    result &= lower_mask;
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

std::unordered_map<std::string, MM::Reflection::Serializer*>&
MM::Reflection::GetSerializerDatabase() {
  static DatabaseWrapper<std::unordered_map<std::string, Serializer*>> g_serializer_database;

  return g_serializer_database.database_;
}

MM::Reflection::TypeHashCode std::hash<MM::Reflection::TypeID>::operator()(
    const MM::Reflection::TypeID& other) const noexcept {
  return other.GetHashCode();
}
