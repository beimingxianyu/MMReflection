#include <gtest/gtest.h>

#include "reflection.h"

using namespace MM::Reflection;

class PropertyTestClass {
  MM_GENERATE_REFLECTION_BODY()

public:
  int property1_{10};
  float property2_{20.0};
  static std::string property3_;
};

std::string PropertyTestClass::property3_{"string"};

MM_REGISTER {
  Class<PropertyTestClass>{"PropertyTestClass"}
    .Property("property1_", &PropertyTestClass::property1_)
    .Property("property2_", &PropertyTestClass::property2_)
    .Property("property3_", &PropertyTestClass::property3_);
}

TEST(reflection, property) {
  const Meta* property_test_meta = GetMetaDatabase().at(typeid(PropertyTestClass).hash_code());
  EXPECT_NE(property_test_meta, nullptr);
  EXPECT_EQ(property_test_meta->GetAllProperty().size(), 3);
  EXPECT_EQ(property_test_meta->HaveProperty("falid_property"), false);
  EXPECT_EQ(property_test_meta->HaveProperty("property1_"), true);
  EXPECT_EQ(property_test_meta->HaveProperty("property2_"), true);
  EXPECT_EQ(property_test_meta->HaveProperty("property3_"), true);
  const Property* property1 = property_test_meta->GetProperty("property1_");
  const Property* property2 = property_test_meta->GetProperty("property2_");
  const Property* property3 = property_test_meta->GetProperty("property3_");
  EXPECT_NE(property1, nullptr);
  EXPECT_NE(property2, nullptr);
  EXPECT_NE(property3, nullptr);
  EXPECT_EQ(property1->IsValid(), true);
  EXPECT_EQ(property2->IsValid(), true);
  EXPECT_EQ(property3->IsValid(), true);
  EXPECT_EQ(property1->IsStatic(), false);
  EXPECT_EQ(property2->IsStatic(), false);
  EXPECT_EQ(property3->IsStatic(), true);
  EXPECT_EQ(property1->GetClassMeta(), property_test_meta);
  EXPECT_EQ(property2->GetClassMeta(), property_test_meta);
  EXPECT_EQ(property3->GetClassMeta(), property_test_meta);
  EXPECT_EQ(property1->GetType()->GetTypeHashCode(), typeid(int).hash_code());
  EXPECT_EQ(property2->GetType()->GetTypeHashCode(), typeid(float).hash_code());
  EXPECT_EQ(property3->GetType()->GetTypeHashCode(), typeid(std::string).hash_code());
  EXPECT_EQ(property1->GetStaticPropertyAddress(), nullptr);
  EXPECT_EQ(property2->GetStaticPropertyAddress(), nullptr);
  EXPECT_EQ(property3->GetStaticPropertyAddress(), &PropertyTestClass::property3_);
  EXPECT_EQ(property1->GetPropertyName(), std::string("property1_"));
  EXPECT_EQ(property2->GetPropertyName(), std::string("property2_"));
  EXPECT_EQ(property3->GetPropertyName(), std::string("property3_"));
  EXPECT_EQ(property1->GetPropertyOffset(), offsetof(PropertyTestClass, property1_));
  EXPECT_EQ(property2->GetPropertyOffset(), offsetof(PropertyTestClass, property2_));
  EXPECT_EQ(property3->GetPropertyOffset(), 0);
  EXPECT_EQ(property1->GetPropertySize(), sizeof(int));
  EXPECT_EQ(property2->GetPropertySize(), sizeof(float));
  EXPECT_EQ(property3->GetPropertySize(), sizeof(std::string));
}

