#include <gtest/gtest.h>

#include "reflection.h"

using namespace MM::Reflection;

enum class TestEnum { UNDIFINED, VALUE1, VALUE2};

MM_REGISTER {
  Enum<TestEnum>("TestEnum")
    .Add("UNDIFINED", TestEnum::UNDIFINED)
    .Add("VALUE1", TestEnum::VALUE1)
    .Add("VALUE2", TestEnum::VALUE2);
}

TEST(reflection, enum_test) {
  const Meta* enum_meta = GetMetaDatabase().at(typeid(TestEnum).hash_code());

  EXPECT_EQ(enum_meta->GetAllConstructor().size(), 1);
  EXPECT_EQ(enum_meta->GetAllMethod().size(), 1);
  EXPECT_EQ(enum_meta->GetAllProperty().size(), 0);
  EXPECT_EQ(enum_meta->GetAllEnums().size(), 3);
  EXPECT_NE(enum_meta->GetConstructor(Meta::GetEnumConstructName()), nullptr);
  EXPECT_NE(enum_meta->GetMethod(Meta::GetEmptyObjectMethodName()), nullptr);

  EXPECT_EQ(enum_meta->CreateEnum("UNDIFINED").GetSize(), sizeof(TestEnum));
  EXPECT_EQ(enum_meta->CreateEnum("UNDIFINED").GetMeta(), enum_meta);
  EXPECT_EQ(enum_meta->CreateEnum("UNDIFINED").IsValid(), true);
  EXPECT_EQ(enum_meta->CreateEnum("UNDIFINED").GetType(), &Type::CreateType<TestEnum>());
  EXPECT_EQ(enum_meta->CreateEnum("UNDIFINED").GetValue(), EnumValue{static_cast<EnumValue>(TestEnum::UNDIFINED)});
  EXPECT_EQ(enum_meta->CreateEnum("UNDIFINED").GetEnumName(), "UNDIFINED");
  EXPECT_EQ(enum_meta->CreateEnum("VALUE1").GetValue(), EnumValue{static_cast<EnumValue>(TestEnum::VALUE1)});
  EXPECT_EQ(enum_meta->CreateEnum("VALUE1").GetEnumName(), "VALUE1");
  EXPECT_EQ(enum_meta->CreateEnum("VALUE2").GetValue(), EnumValue{static_cast<EnumValue>(TestEnum::VALUE2)});
  EXPECT_EQ(enum_meta->CreateEnum("VALUE2").GetEnumName(), "VALUE2");
  EXPECT_EQ(enum_meta->CreateEnum("invalid_name").IsValid(), false);

  Variable undifined = enum_meta->CreateEnumVariable("UNDIFINED");
  EXPECT_EQ(undifined.IsValid(), true);
  EXPECT_EQ(undifined.GetValueCast<TestEnum>(), TestEnum::UNDIFINED);
  Variable value1 = enum_meta->CreateEnumVariable("VALUE1");
  EXPECT_EQ(value1.IsValid(), true);
  EXPECT_EQ(value1.GetValueCast<TestEnum>(), TestEnum::VALUE1);
  Variable value2 = enum_meta->CreateEnumVariable("VALUE2");
  EXPECT_EQ(value2.IsValid(), true);
  EXPECT_EQ(value2.GetValueCast<TestEnum>(), TestEnum::VALUE2);
  Variable invalid = enum_meta->CreateEnumVariable("invalid_name");
  EXPECT_EQ(invalid.IsValid(), false);
}
