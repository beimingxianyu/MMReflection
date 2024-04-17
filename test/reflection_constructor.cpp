#include <gtest/gtest.h>

#include "reflection.h"

using namespace MM::Reflection;

struct TestProperty {
  int property1_{1};
  float property2_{2.0};
  double property3_{3.0};

  friend bool operator==(const TestProperty& lhs, const TestProperty& rhs) {
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ && lhs.property3_ == rhs.property3_;
  }
  friend bool operator!=(const TestProperty& lhs, const TestProperty& rhs) {
    return !(lhs == rhs);
  }

  TestProperty(const int property1, const float property2,
               const double property3)
      : property1_(property1), property2_(property2), property3_(property3) {}
  TestProperty(const TestProperty& other)
      : property1_(other.property1_),
        property2_(other.property2_),
        property3_(other.property3_) {}

  TestProperty(TestProperty&& other) noexcept
      : property1_(other.property1_),
        property2_(other.property2_),
        property3_(other.property3_) {}

  TestProperty& operator=(const TestProperty& other) {
    if (this == &other) return *this;
    property1_ = other.property1_;
    property2_ = other.property2_;
    property3_ = other.property3_;
    return *this;
  }

  TestProperty& operator=(TestProperty&& other) noexcept {
    if (this == &other) return *this;
    property1_ = other.property1_;
    property2_ = other.property2_;
    property3_ = other.property3_;
    return *this;
  }
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

  friend bool operator==(const ConstructorTestClass& lhs,
                         const ConstructorTestClass& rhs) {
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_ &&
           lhs.property4_ == rhs.property4_ &&
           lhs.property5_ == rhs.property5_ &&
           lhs.property6_ == rhs.property6_ && lhs.property7_ == rhs.property7_;
  }
  friend bool operator!=(const ConstructorTestClass& lhs,
                         const ConstructorTestClass& rhs) {
    return !(lhs == rhs);
  }

  bool Equal(const ConstructorTestClass& other) const {
   return *this == other;
  }

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
    .Property("property2_", &ConstructorTestClass::property2_)
    .Method("Equal", &ConstructorTestClass::Equal);
}

TEST(reflection, constructor) {
  Meta* meta_data = GetMetaDatabase().at(GetNameToTypeHashDatabase().at("ConstructorTestClass"));

  Variable arg1 = Variable::CreateVariable(10),
           arg2 = Variable::CreateVariable(11.0f),
           arg3 = Variable::CreateVariable(12.0),
           arg4 = Variable::CreateVariable(13),
           arg5 = Variable::CreateVariable(14.0f),
           arg6 = Variable::CreateVariable(15.0),
           arg7 = Variable::EmplaceVariable<TestProperty>(16, 17.0f, 18.0);
  void *placement_address0 =
           malloc(GetVariableWrpperSize(sizeof(ConstructorTestClass))),
       *placement_address1 =
           malloc(GetVariableWrpperSize(sizeof(ConstructorTestClass))),
       *placement_address2 =
           malloc(GetVariableWrpperSize(sizeof(ConstructorTestClass))),
       *placement_address3 =
           malloc(GetVariableWrpperSize(sizeof(ConstructorTestClass))),
       *placement_address4 =
           malloc(GetVariableWrpperSize(sizeof(ConstructorTestClass))),
       *placement_address5 =
           malloc(GetVariableWrpperSize(sizeof(ConstructorTestClass))),
       *placement_address6 =
           malloc(GetVariableWrpperSize(sizeof(ConstructorTestClass))),
       *placement_address7 =
           malloc(GetVariableWrpperSize(sizeof(ConstructorTestClass)));
  EXPECT_NE(placement_address0, nullptr);
  EXPECT_NE(placement_address1, nullptr);
  EXPECT_NE(placement_address2, nullptr);
  EXPECT_NE(placement_address3, nullptr);
  EXPECT_NE(placement_address4, nullptr);
  EXPECT_NE(placement_address5, nullptr);
  EXPECT_NE(placement_address6, nullptr);
  EXPECT_NE(placement_address7, nullptr);

  ConstructorTestClass equal_variable{};

  const Constructor* default_constructor = meta_data->GetConstructor("Default");
  EXPECT_NE(default_constructor, nullptr);
  EXPECT_EQ(default_constructor->IsValid(), true);
  Variable default_variable = default_constructor->Invoke();
  EXPECT_EQ(default_variable.IsValid(), true);
  EXPECT_EQ(default_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
  EXPECT_EQ(default_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
  {
    Variable placement_default_variable = default_constructor->Invoke(placement_address0);
    EXPECT_EQ(placement_default_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
    EXPECT_EQ(placement_default_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
    EXPECT_EQ(placement_default_variable.GetValue(), reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(placement_address0) + sizeof(void*)));
  }

  equal_variable.property1_ = arg1.GetValueCast<int>();
  const Constructor* init1_constructor = meta_data->GetConstructor("Init1");
  EXPECT_NE(init1_constructor, nullptr);
  EXPECT_EQ(init1_constructor->IsValid(), true);
  Variable init1_variable = init1_constructor->Invoke(arg1);
  EXPECT_EQ(init1_variable.IsValid(), true);
  EXPECT_EQ(init1_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
  EXPECT_EQ(init1_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
  {
    Variable placement_init1_variable = init1_constructor->Invoke(placement_address1, arg1);
    EXPECT_EQ(placement_init1_variable.IsValid(), true);
    EXPECT_EQ(placement_init1_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
    EXPECT_EQ(placement_init1_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
    EXPECT_EQ(placement_init1_variable.GetValue(), reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(placement_address1) + sizeof(void*)));
  }

  equal_variable.property2_ = arg2.GetValueCast<float>();
  const Constructor* init2_constructor = meta_data->GetConstructor("Init2");
  EXPECT_NE(init2_constructor, nullptr);
  EXPECT_EQ(init2_constructor->IsValid(), true);
  Variable init2_variable = init2_constructor->Invoke(arg1, arg2);
  EXPECT_EQ(init2_variable.IsValid(), true);
  EXPECT_EQ(init2_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
  EXPECT_EQ(init2_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
  {
    Variable placement_init2_variable = init2_constructor->Invoke(placement_address2, arg1, arg2);
    EXPECT_EQ(placement_init2_variable.IsValid(), true);
    EXPECT_EQ(placement_init2_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
    EXPECT_EQ(placement_init2_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
    EXPECT_EQ(placement_init2_variable.GetValue(), reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(placement_address2) + sizeof(void*)));
  }

  equal_variable.property3_ = arg3.GetValueCast<double>();
  const Constructor* init3_constructor = meta_data->GetConstructor("Init3");
  EXPECT_NE(init3_constructor, nullptr);
  EXPECT_EQ(init3_constructor->IsValid(), true);
  Variable init3_variable = init3_constructor->Invoke(arg1, arg2, arg3);
  EXPECT_EQ(init3_variable.IsValid(), true);
  EXPECT_EQ(init3_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
  EXPECT_EQ(init3_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
  {
    Variable placement_init3_variable = init3_constructor->Invoke(placement_address3, arg1, arg2, arg3);
    EXPECT_EQ(placement_init3_variable.IsValid(), true);
    EXPECT_EQ(placement_init3_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
    EXPECT_EQ(placement_init3_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
    EXPECT_EQ(placement_init3_variable.GetValue(), reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(placement_address3) + sizeof(void*)));
  }

  equal_variable.property4_ = arg4.GetValueCast<int>();
  const Constructor* init4_constructor = meta_data->GetConstructor("Init4");
  EXPECT_NE(init4_constructor, nullptr);
  EXPECT_EQ(init4_constructor->IsValid(), true);
  Variable init4_variable = init4_constructor->Invoke(arg1, arg2, arg3, arg4);
  EXPECT_EQ(init4_variable.IsValid(), true);
  EXPECT_EQ(init4_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
  EXPECT_EQ(init4_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
  {
    Variable placement_init4_variable = init4_constructor->Invoke(placement_address4, arg1, arg2, arg3, arg4);
    EXPECT_EQ(placement_init4_variable.IsValid(), true);
    EXPECT_EQ(placement_init4_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
    EXPECT_EQ(placement_init4_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
    EXPECT_EQ(placement_init4_variable.GetValue(), reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(placement_address4) + sizeof(void*)));
  }

  equal_variable.property5_ = arg5.GetValueCast<float>();
  const Constructor* init5_constructor = meta_data->GetConstructor("Init5");
  EXPECT_NE(init5_constructor, nullptr);
  EXPECT_EQ(init5_constructor->IsValid(), true);
  Variable init5_variable = init5_constructor->Invoke(arg1, arg2, arg3, arg4, arg5);
  EXPECT_EQ(init5_variable.IsValid(), true);
  EXPECT_EQ(init5_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
  EXPECT_EQ(init5_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
  {
    Variable placement_init5_variable = init5_constructor->Invoke(placement_address5, arg1, arg2, arg3, arg4, arg5);
    EXPECT_EQ(placement_init5_variable.IsValid(), true);
    EXPECT_EQ(placement_init5_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
    EXPECT_EQ(placement_init5_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
    EXPECT_EQ(placement_init5_variable.GetValue(), reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(placement_address5) + sizeof(void*)));
  }

  equal_variable.property6_ = arg6.GetValueCast<double>();
  const Constructor* init6_constructor = meta_data->GetConstructor("Init6");
  EXPECT_NE(init6_constructor, nullptr);
  EXPECT_EQ(init6_constructor->IsValid(), true);
  Variable init6_variable = init6_constructor->Invoke(arg1, arg2, arg3, arg4, arg5, arg6);
  EXPECT_EQ(init6_variable.IsValid(), true);
  EXPECT_EQ(init6_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
  EXPECT_EQ(init6_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
  {
    Variable placement_init6_variable = init6_constructor->Invoke(placement_address6, arg1, arg2, arg3, arg4, arg5, arg6);
    EXPECT_EQ(placement_init6_variable.IsValid(), true);
    EXPECT_EQ(placement_init6_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
    EXPECT_EQ(placement_init6_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
    EXPECT_EQ(placement_init6_variable.GetValue(), reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(placement_address6) + sizeof(void*)));
  }

  equal_variable.property7_ = arg7.GetValueCast<TestProperty>();
  const Constructor* init7_constructor = meta_data->GetConstructor("Init7");
  EXPECT_NE(init7_constructor, nullptr);
  EXPECT_EQ(init7_constructor->IsValid(), true);
  std::vector<Variable*> args{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7};
  Variable init7_variable = init7_constructor->Invoke(args);
  EXPECT_EQ(init7_variable.IsValid(), true);
  EXPECT_EQ(init7_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
  EXPECT_EQ(init7_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
  {
    Variable placement_init7_variable = init7_constructor->Invoke(placement_address7, args);
    EXPECT_EQ(placement_init7_variable.IsValid(), true);
    EXPECT_EQ(placement_init7_variable.GetType()->GetTypeHashCode(), typeid(ConstructorTestClass).hash_code());
    EXPECT_EQ(placement_init7_variable.GetValueCast<ConstructorTestClass>(), equal_variable);
    EXPECT_EQ(placement_init7_variable.GetValue(), reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(placement_address7) + sizeof(void*)));
  }

  free(placement_address0);
  free(placement_address1);
  free(placement_address2);
  free(placement_address3);
  free(placement_address4);
  free(placement_address5);
  free(placement_address6);
  free(placement_address7);
}
