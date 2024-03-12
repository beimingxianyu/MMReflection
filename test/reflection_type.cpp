#include <gtest/gtest.h>

#include "reflection.h"

using namespace MM::Reflection;

TEST(reflection, type) {
  const Type& int_type = Type::CreateType<int>();
  EXPECT_EQ(int_type.IsValid(), true);
  EXPECT_EQ(int_type.IsRegistered(), true);
  EXPECT_EQ(int_type.GetSize(), sizeof(int));
  EXPECT_EQ(int_type.HaveDestructor(), true);
  EXPECT_EQ(int_type.IsArray(), false);
  EXPECT_EQ(int_type.IsConst(), false);
  EXPECT_EQ(int_type.IsReference(), false);
  EXPECT_EQ(int_type.IsEnum(), false);
  EXPECT_EQ(int_type.IsPointer(), false);
  EXPECT_EQ(int_type.GetTypeName(), "std::int32_t");
  const Meta* int_meta = int_type.GetMate();
  EXPECT_NE(int_meta, nullptr);
  EXPECT_EQ(int_meta->GetTypeName(), "std::int32_t");
  EXPECT_EQ(int_meta->GetAllConstructor().size(), 0);
  EXPECT_EQ(int_meta->GetAllMethod().size(), 0);
  EXPECT_EQ(int_meta->GetAllProperty().size(), 0);

  enum class TestEnum { UNDIFINED, VALUE1, VALUE2};
  const Type& enum_type1 = Type::CreateType<TestEnum>();
  EXPECT_EQ(enum_type1.IsValid(), true);
  EXPECT_EQ(enum_type1.IsRegistered(), false);
  EXPECT_EQ(enum_type1.GetSize(), sizeof(TestEnum));
  EXPECT_EQ(enum_type1.HaveDestructor(), true);
  EXPECT_EQ(enum_type1.IsArray(), false);
  EXPECT_EQ(enum_type1.IsConst(), false);
  EXPECT_EQ(enum_type1.IsReference(), false);
  EXPECT_EQ(enum_type1.IsEnum(), true);
  EXPECT_EQ(enum_type1.IsPointer(), false);
  EXPECT_EQ(enum_type1.GetTypeName(), std::string(""));
  const Meta* enum_meta1 = enum_type1.GetMate();
  EXPECT_EQ(enum_meta1, nullptr);

  const Type& enum_type2 = Type::CreateType<const TestEnum>();
  EXPECT_EQ(enum_type2.IsValid(), true);
  EXPECT_EQ(enum_type2.IsRegistered(), false);
  EXPECT_EQ(enum_type2.GetSize(), sizeof(TestEnum));
  EXPECT_EQ(enum_type2.HaveDestructor(), true);
  EXPECT_EQ(enum_type2.IsArray(), false);
  EXPECT_EQ(enum_type2.IsConst(), true);
  EXPECT_EQ(enum_type2.IsReference(), false);
  EXPECT_EQ(enum_type2.IsEnum(), true);
  EXPECT_EQ(enum_type2.IsPointer(), false);
  EXPECT_EQ(enum_type2.GetTypeName(), std::string(""));

  const Type& enum_type3 = Type::CreateType<TestEnum[3]>();
  EXPECT_EQ(enum_type3.IsValid(), true);
  EXPECT_EQ(enum_type3.IsRegistered(), false);
  EXPECT_EQ(enum_type3.GetSize(), sizeof(TestEnum[3]));
  EXPECT_EQ(enum_type3.HaveDestructor(), true);
  EXPECT_EQ(enum_type3.IsArray(), true);
  EXPECT_EQ(enum_type3.IsConst(), false);
  EXPECT_EQ(enum_type3.IsReference(), false);
  EXPECT_EQ(enum_type3.IsEnum(), false);
  EXPECT_EQ(enum_type3.IsPointer(), false);
  EXPECT_EQ(enum_type3.GetTypeName(), std::string(""));

  const Type& enum_type4 = Type::CreateType<const TestEnum[3]>();
  EXPECT_EQ(enum_type4.IsValid(), true);
  EXPECT_EQ(enum_type4.IsRegistered(), false);
  EXPECT_EQ(enum_type4.GetSize(), sizeof(const TestEnum[3]));
  EXPECT_EQ(enum_type4.HaveDestructor(), true);
  EXPECT_EQ(enum_type4.IsArray(), true);
  EXPECT_EQ(enum_type4.IsConst(), true);
  EXPECT_EQ(enum_type4.IsReference(), false);
  EXPECT_EQ(enum_type4.IsEnum(), false);
  EXPECT_EQ(enum_type4.IsPointer(), false);
  EXPECT_EQ(enum_type4.GetTypeName(), std::string(""));

  const Type& enum_type5 = Type::CreateType<TestEnum*>();
  EXPECT_EQ(enum_type5.IsValid(), true);
  EXPECT_EQ(enum_type5.IsRegistered(), false);
  EXPECT_EQ(enum_type5.GetSize(), sizeof(TestEnum*));
  EXPECT_EQ(enum_type5.HaveDestructor(), true);
  EXPECT_EQ(enum_type5.IsArray(), false);
  EXPECT_EQ(enum_type5.IsConst(), false);
  EXPECT_EQ(enum_type5.IsReference(), false);
  EXPECT_EQ(enum_type5.IsEnum(), false);
  EXPECT_EQ(enum_type5.IsPointer(), true);
  EXPECT_EQ(enum_type5.GetTypeName(), std::string(""));

  const Type& enum_type6 = Type::CreateType<const TestEnum*>();
  EXPECT_EQ(enum_type6.IsValid(), true);
  EXPECT_EQ(enum_type6.IsRegistered(), false);
  EXPECT_EQ(enum_type6.GetSize(), sizeof(TestEnum*));
  EXPECT_EQ(enum_type6.HaveDestructor(), true);
  EXPECT_EQ(enum_type6.IsArray(), false);
  EXPECT_EQ(enum_type6.IsConst(), false);
  EXPECT_EQ(enum_type6.IsReference(), false);
  EXPECT_EQ(enum_type6.IsEnum(), false);
  EXPECT_EQ(enum_type6.IsPointer(), true);
  EXPECT_EQ(enum_type6.GetTypeName(), std::string(""));

  const Type& enum_type7 = Type::CreateType<TestEnum* const>();
  EXPECT_EQ(enum_type7.IsValid(), true);
  EXPECT_EQ(enum_type7.IsRegistered(), false);
  EXPECT_EQ(enum_type7.GetSize(), sizeof(TestEnum*));
  EXPECT_EQ(enum_type7.HaveDestructor(), true);
  EXPECT_EQ(enum_type7.IsArray(), false);
  EXPECT_EQ(enum_type7.IsConst(), true);
  EXPECT_EQ(enum_type7.IsReference(), false);
  EXPECT_EQ(enum_type7.IsEnum(), false);
  EXPECT_EQ(enum_type7.IsPointer(), true);
  EXPECT_EQ(enum_type7.GetTypeName(), std::string(""));
}
