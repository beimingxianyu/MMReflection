#include <gtest/gtest.h>

#include "reflection.h"

using namespace MM::Reflection;

struct TestProperty {
  int property1_{1};
  float property2_{2.0};
  double property3_{3.0};
};

class ConstructorTestClass {
  MM_GENERATE_REFLECTION_BODY()

public:
  ConstructorTestClass() = default;
 explicit ConstructorTestClass(const int& property1) : property1_(property1) {}
  ConstructorTestClass(int property1, const float& property2)
      : property1_(property1), property2_(property2) {}
  ConstructorTestClass(int property1, const float& property2, double property3)
      : property1_(property1), property2_(property2), property3_(property3) {}
  ConstructorTestClass(int property1, const float& property2, double property3,
                       int property4)
      : property1_(property1),
        property2_(property2),
        property3_(property3),
        property4_(property4) {}
  ConstructorTestClass(int property1, float property2, double property3,
                       int property4, float property5)
      : property1_(property1),
        property2_(property2),
        property3_(property3),
        property4_(property4),
        property5_(property5) {}
  ConstructorTestClass(int property1, float property2, double property3,
                       int property4, float property5, double property6)
      : property1_(property1),
        property2_(property2),
        property3_(property3),
        property4_(property4),
        property5_(property5),
        property6_(property6) {}
  ConstructorTestClass(const int property1, const float property2,
                       const double property3, const int property4,
                       const float property5, const double property6,
                       const TestProperty& property7)
      : property1_(property1),
        property2_(property2),
        property3_(property3),
        property4_(property4),
        property5_(property5),
        property6_(property6),
        property7_(property7) {}
  // ConstructorTestClass(const int property1, const float property2,
  //                      const double property3, const int property4,
  //                      const float property5, const double property6,
  //                      const int& property7)
  //     : property1_(property1),
  //       property2_(property2),
  //       property3_(property3),
  //       property4_(property4),
  //       property5_(property5),
  //       property6_(property6),
  //       property7_(property7) {}

  int property1_{1};
  float property2_{2.0};
  double property3_{3.0};
  int property4_{4};
  float property5_{5.0};
  double property6_{6.0};
  TestProperty property7_{7, 8.0, 9.0};
  // int property7_{7};
};

MM_REGISTER {
  Class<ConstructorTestClass>{"ConstructorTestClass"}
    .Constructor<>("Default")
    .Constructor<const int&>("Init1")
    .Constructor<int, const float&>("Init2")
    .Constructor<int, const float&, double>("Init3")
    .Constructor<int, const float&, double, int>("Init4")
    .Constructor<int, float, double, int, float>("Init5")
    .Constructor<int, float, double, int, float, double>("Init6")
    .Constructor<const int, const float, const double, const int, const float, const double, const TestProperty&>("Init7")
    .Property("property1_", &ConstructorTestClass::property1_)
    .Property("property2_", &ConstructorTestClass::property2_);
}

TEST(reflection, constructor) {
  Meta* meta_data = GetMetaDatabase().at(GetNameToTypeHashDatabase().at("ConstructorTestClass"));

  // Variable arg1 =;

  const Constructor* default_constructor = meta_data->GetConstructor("Default");
  EXPECT_NE(default_constructor, nullptr);
  Variable variable = default_constructor->Invoke();
  EXPECT_EQ(default_constructor->IsValid(), true);


  const Constructor* init_constructor = meta_data->GetConstructor("Init");

}
