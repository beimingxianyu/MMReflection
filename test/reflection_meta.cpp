#include <gtest/gtest.h>

#include "reflection.h"

using namespace MM::Reflection;

TEST(reflection, meta) {
  // default types
  auto& name_to_ID_database = GetNameToTypeHashDatabase();
  auto& meta_database = GetMetaDatabase();

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
  MM::Reflection::Class<std::string>{"std::string"}
      .Constructor<>("Empty")
      .Constructor<std::string>("Init");

  std::uint8_t u_arg_value = 7;
  char c_arg_value = 7;
  std::int8_t s_arg_value = -1;
  Variable u_arg = Variable::CreateVariable<std::uint8_t>{u_arg_value};
  Variable c_arg = Variable::CreateVariable<char>{c_arg_value};
  Variable s_arg = Variable::CreateVariable<std::int8_t>{s_arg_value};

  EXPECT_NE(name_to_ID_database.find("void"), name_to_ID_database.end());
  const TypeHashCode void_hash_code = name_to_ID_database.at("void");
  EXPECT_EQ(void_hash_code, typeid(void).hash_code());
  EXPECT_NE(meta_database.find(void_hash_code), meta_database.end());
  Meta* void_meta = meta_database.at(void_hash_code);

  EXPECT_NE(name_to_ID_database.find("char"), name_to_ID_database.end());
  const TypeHashCode char_hash_code = name_to_ID_database.at("char");
  EXPECT_EQ(char_hash_code, typeid(char).hash_code());
  EXPECT_NE(meta_database.find(char_hash_code), meta_database.end());
  Meta* char_meta = meta_database.at(char_hash_code);
  const Constructor* char_empty_constructor = char_meta->GetConstructor("Empty");
  Variable char_empty_variable = char_empty_constructor->Invoke();
  EXPECT_EQ(char_empty_constructor->IsValid(), true);
  const Constructor* char_init_constructor = char_meta->GetConstructor("Init");
  Variable char_init_variable = char_init_constructor->Invoke(c_arg);
  EXPECT_EQ(char_init_variable->IsValid(), true);
  EXPECT_EQ(*static_cast<char*>(char_init_variable.GetValue()), c_arg_value);

  EXPECT_NE(name_to_ID_database.find("std::uint16_t"), name_to_ID_database.end());
  const TypeHashCode uint16_t_hash_code = name_to_ID_database.at("uint16_t");
  EXPECT_EQ(uint16_t_hash_code, typeid(std::uint16_t).hash_code());
  EXPECT_NE(meta_database.find(uint16_t_hash_code), meta_database.end());
  Meta* uint16_t_meta = meta_database.at(uint16_t_hash_code);
  const Constructor* uint16_t_empty_constructor = uint16_t_meta->GetConstructor("Empty");
  Variable uint16_t_empty_variable = uint16_t_empty_constructor->Invoke();
  EXPECT_EQ(uint16_t_empty_constructor->IsValid(), true);
  const Constructor* uint16_t_init_constructor = uint16_t_meta->GetConstructor("Init");
  Variable uint16_t_init_variable = uint16_t_init_constructor->Invoke(u_arg);
  EXPECT_EQ(uint16_t_init_variable->IsValid(), true);
  EXPECT_EQ(*static_cast<std::uint16_t*>(uint16_t_init_variable.GetValue()), u_arg_value);

  EXPECT_NE(name_to_ID_database.find("std::uint32_t"), name_to_ID_database.end());
  const TypeHashCode uint32_t_hash_code = name_to_ID_database.at("uint32_t");
  EXPECT_EQ(uint32_t_hash_code, typeid(std::uint32_t).hash_code());
  EXPECT_NE(meta_database.find(uint32_t_hash_code), meta_database.end());
  Meta* uint32_t_meta = meta_database.at(uint32_t_hash_code);
  const Constructor* uint32_t_empty_constructor = uint32_t_meta->GetConstructor("Empty");
  Variable uint32_t_empty_variable = uint32_t_empty_constructor->Invoke();
  EXPECT_EQ(uint32_t_empty_constructor->IsValid(), true);
  const Constructor* uint32_t_init_constructor = uint32_t_meta->GetConstructor("Init");
  Variable uint32_t_init_variable = uint32_t_init_constructor->Invoke(u_arg);
  EXPECT_EQ(uint32_t_init_variable->IsValid(), true);
  EXPECT_EQ(*static_cast<std::uint32_t*>(uint32_t_init_variable.GetValue()), u_arg_value);

  EXPECT_NE(name_to_ID_database.find("std::uint64_t"), name_to_ID_database.end());
  const TypeHashCode uint64_t_hash_code = name_to_ID_database.at("uint64_t");
  EXPECT_EQ(uint64_t_hash_code, typeid(std::uint64_t).hash_code());
  EXPECT_NE(meta_database.find(uint64_t_hash_code), meta_database.end());
  Meta* uint64_t_meta = meta_database.at(uint64_t_hash_code);
  const Constructor* uint64_t_empty_constructor = uint64_t_meta->GetConstructor("Empty");
  Variable uint64_t_empty_variable = uint64_t_empty_constructor->Invoke();
  EXPECT_EQ(uint64_t_empty_constructor->IsValid(), true);
  const Constructor* uint64_t_init_constructor = uint64_t_meta->GetConstructor("Init");
  Variable uint64_t_init_variable = uint64_t_init_constructor->Invoke(u_arg);
  EXPECT_EQ(uint64_t_init_variable->IsValid(), true);
  EXPECT_EQ(*static_cast<std::uint64_t*>(uint64_t_init_variable.GetValue()), u_arg_value);

  EXPECT_NE(name_to_ID_database.find("std::int8_t"), name_to_ID_database.end());
  const TypeHashCode int8_t_hash_code = name_to_ID_database.at("int8_t");
  EXPECT_EQ(int8_t_hash_code, typeid(std::int8_t).hash_code());
  EXPECT_NE(meta_database.find(int8_t_hash_code), meta_database.end());
  Meta* int8_t_meta = meta_database.at(int8_t_hash_code);
  const Constructor* int8_t_empty_constructor = int8_t_meta->GetConstructor("Empty");
  Variable int8_t_empty_variable = int8_t_empty_constructor->Invoke();
  EXPECT_EQ(int8_t_empty_constructor->IsValid(), true);
  const Constructor* int8_t_init_constructor = int8_t_meta->GetConstructor("Init");
  Variable int8_t_init_variable = int8_t_init_constructor->Invoke(s_arg);
  EXPECT_EQ(int8_t_init_variable->IsValid(), true);
  EXPECT_EQ(*static_cast<std::int16_t*>(int8_t_init_variable.GetValue()), s_arg_value);

  EXPECT_NE(name_to_ID_database.find("std::int16_t"), name_to_ID_database.end());
  const TypeHashCode int16_t_hash_code = name_to_ID_database.at("int16_t");
  EXPECT_EQ(int16_t_hash_code, typeid(std::int16_t).hash_code());
  EXPECT_NE(meta_database.find(int16_t_hash_code), meta_database.end());
  Meta* int16_t_meta = meta_database.at(int16_t_hash_code);
  const Constructor* int16_t_empty_constructor = int16_t_meta->GetConstructor("Empty");
  Variable int16_t_empty_variable = int16_t_empty_constructor->Invoke();
  EXPECT_EQ(int16_t_empty_constructor->IsValid(), true);
  const Constructor* int16_t_init_constructor = int16_t_meta->GetConstructor("Init");
  Variable int16_t_init_variable = int16_t_init_constructor->Invoke(s_arg);
  EXPECT_EQ(int16_t_init_variable->IsValid(), true);
  EXPECT_EQ(*static_cast<std::int16_t*>(int16_t_init_variable.GetValue()), s_arg_value);

  EXPECT_NE(name_to_ID_database.find("std::int32_t"), name_to_ID_database.end());
  const TypeHashCode int32_t_hash_code = name_to_ID_database.at("int32_t");
  EXPECT_EQ(int32_t_hash_code, typeid(std::int32_t).hash_code());
  EXPECT_NE(meta_database.find(int32_t_hash_code), meta_database.end());
  Meta* int32_t_meta = meta_database.at(int32_t_hash_code);
  const Constructor* int32_t_empty_constructor = int32_t_meta->GetConstructor("Empty");
  Variable int32_t_empty_variable = int32_t_empty_constructor->Invoke();
  EXPECT_EQ(int32_t_empty_constructor->IsValid(), true);
  const Constructor* int32_t_init_constructor = int32_t_meta->GetConstructor("Init");
  Variable int32_t_init_variable = int32_t_init_constructor->Invoke(s_arg);
  EXPECT_EQ(int32_t_init_variable->IsValid(), true);
  EXPECT_EQ(*static_cast<std::int32_t*>(int32_t_init_variable.GetValue()), s_arg_value);

  EXPECT_NE(name_to_ID_database.find("std::int64_t"), name_to_ID_database.end());
  const TypeHashCode int64_t_hash_code = name_to_ID_database.at("int64_t");
  EXPECT_EQ(int64_t_hash_code, typeid(std::int64_t).hash_code());
  EXPECT_NE(meta_database.find(int64_t_hash_code), meta_database.end());
  Meta* int64_t_meta = meta_database.at(int64_t_hash_code);
  const Constructor* int64_t_empty_constructor = int64_t_meta->GetConstructor("Empty");
  Variable int64_t_empty_variable = int64_t_empty_constructor->Invoke();
  EXPECT_EQ(int64_t_empty_constructor->IsValid(), true);
  const Constructor* int64_t_init_constructor = int64_t_meta->GetConstructor("Init");
  Variable int64_t_init_variable = int64_t_init_constructor->Invoke(s_arg);
  EXPECT_EQ(int64_t_init_variable->IsValid(), true);
  EXPECT_EQ(*static_cast<std::int64_t*>(int64_t_init_variable.GetValue()), s_arg_value);

  EXPECT_NE(GetMetaDatabase().find(typeid(float).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(double).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(std::string).hash_code()), GetMetaDatabase().end());
}
