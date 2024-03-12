#include <gtest/gtest.h>

#include "reflection.h"

using namespace MM::Reflection;

class VariableTestClass {
  MM_GENERATE_REFLECTION_BODY()

public:
  VariableTestClass(int property1, const float& property2) : property1_(property1), property2_(property2) {}

public:
  int property1_{10};
  float property2_{20.0};
  static std::string property3_;

  int GetProperty1() const {
    return property1_;
  }

  float GetProperty2() const {
    return property2_;
  }

  const float& GetProperty2Refrence() const {
    return property2_;
  }

  void SetProperty1(const int& property1) {
    property1_ = property1;
  }

  void SetProperty2(float property2) {
    property2_ = property2;
  }
};

std::string VariableTestClass::property3_{"string"};

MM_REGISTER {
  Class<VariableTestClass>{"VariableTestClass"}
    .Constructor<int, const float&>("Init")
    .Property("property1_", &VariableTestClass::property1_)
    .Property("property2_", &VariableTestClass::property2_)
    .Property("property3_", &VariableTestClass::property3_)
    .Method("GetProperty1", &VariableTestClass::GetProperty1)
    .Method("GetProperty2", &VariableTestClass::GetProperty2)
    .Method("GetProperty2Refrence", &VariableTestClass::GetProperty2Refrence)
    .Method("SetProperty1", &VariableTestClass::SetProperty1)
    .Method("SetProperty2", &VariableTestClass::SetProperty2);
}

TEST(reflection, variable) {
  const Meta* variable_test_meta = GetMetaDatabase().at(typeid(VariableTestClass).hash_code());
  EXPECT_NE(variable_test_meta, nullptr);

  std::vector<const Constructor*> constructors = variable_test_meta->GetAllConstructor();
  std::vector<const Method*> methods = variable_test_meta->GetAllMethod();
  std::vector<const Property*> properties = variable_test_meta->GetAllProperty();

  EXPECT_EQ(constructors.size(), 1);
  EXPECT_EQ(properties.size(), 3);
  EXPECT_EQ(methods.size(), 5);

  EXPECT_EQ(variable_test_meta->HaveConstructor("Init"), true);
  EXPECT_EQ(variable_test_meta->HaveConstructor("falid_constructor"), false);
  EXPECT_EQ(variable_test_meta->HaveProperty("property1_"), true);
  EXPECT_EQ(variable_test_meta->HaveProperty("property2_"), true);
  EXPECT_EQ(variable_test_meta->HaveProperty("property3_"), true);
  EXPECT_EQ(variable_test_meta->HaveProperty("falid_property"), false);
  EXPECT_EQ(variable_test_meta->HaveMethod("GetProperty1"), true);
  EXPECT_EQ(variable_test_meta->HaveMethod("GetProperty2"), true);
  EXPECT_EQ(variable_test_meta->HaveMethod("GetProperty2Refrence"), true);
  EXPECT_EQ(variable_test_meta->HaveMethod("SetProperty1"), true);
  EXPECT_EQ(variable_test_meta->HaveMethod("SetProperty2"), true);
  EXPECT_EQ(variable_test_meta->HaveMethod("SetProperty2"), true);
  EXPECT_EQ(variable_test_meta->HaveMethod("falid_method"), false);

  Variable empty_variable{};
  Variable constructor_arg1 = Variable::EmplaceVariable<int>(20), constructor_arg2 = Variable::EmplaceVariable<float>(10.0f);
  Variable invalid_variable = constructors[0]->Invoke(empty_variable, empty_variable, constructor_arg2);
  EXPECT_EQ(invalid_variable.IsValid(), false);
  Variable new_variable = constructors[0]->Invoke(constructor_arg1, constructor_arg2);
  EXPECT_EQ(new_variable.IsValid(), true);
  VariableTestClass* variable_object = static_cast<VariableTestClass*>(new_variable.GetValue());
  EXPECT_EQ(variable_object->property1_, 20);
  EXPECT_EQ(variable_object->property2_, 10.0);

  EXPECT_EQ(variable_test_meta->GetMethod("GetProperty1")->Invoke(new_variable).GetValueCast<int>(), 20);
  EXPECT_EQ(variable_test_meta->GetMethod("GetProperty2")->Invoke(new_variable).GetValueCast<float>(), 10.0);
  EXPECT_EQ(variable_test_meta->GetMethod("GetProperty2Refrence")->Invoke(new_variable).GetValue(), &(variable_object->property2_));
  Variable new_int_variable = Variable::EmplaceVariable<int>(10), new_float_variable = Variable::EmplaceVariable<float>(20.0f);
  EXPECT_EQ(variable_test_meta->GetMethod("SetProperty1")->Invoke(new_variable, new_int_variable).IsVoid(), true);
  EXPECT_EQ(variable_test_meta->GetMethod("SetProperty2")->Invoke(new_variable, new_float_variable).IsVoid(), true);
  EXPECT_EQ(variable_test_meta->GetMethod("GetProperty1")->Invoke(new_variable).GetValueCast<int>(), 10);
  EXPECT_EQ(variable_test_meta->GetMethod("GetProperty2")->Invoke(new_variable).GetValueCast<float>(), 20.0);

  Variable property1_variable = new_variable.GetPropertyVariable("property1_");
  Variable property2_variable = new_variable.GetPropertyVariable("property2_");
  Variable invalid_property_variable = new_variable.GetPropertyVariable("invalid_property");
  EXPECT_EQ(property1_variable.IsValid(), true);
  EXPECT_EQ(property2_variable.IsValid(), true);
  EXPECT_EQ(invalid_property_variable .IsValid(), false);
  EXPECT_EQ(property1_variable.IsRefrenceVariable(), true);
  EXPECT_EQ(property2_variable.IsRefrenceVariable(), true);
  EXPECT_EQ(property1_variable.IsPropertyVariable(), true);
  EXPECT_EQ(property2_variable.IsPropertyVariable(), true);
  EXPECT_EQ(property1_variable.GetValueCast<int>(), 10);
  EXPECT_EQ(property2_variable.GetValueCast<float>(), 20.0f);
  property1_variable = Variable::CreateVariable<int>(20);
  property2_variable = Variable::CreateVariable<float>(10.0f);
  EXPECT_EQ(variable_object->property1_, 20);
  EXPECT_EQ(variable_object->property2_, 10.0f);
}

