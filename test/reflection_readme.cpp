#include <cassert>
#include <gtest/gtest.h>

#include <registration.h>
using namespace MM::Reflection;

struct MyStruct { MyStruct() {}; void Func(double) {}; int data_; };

MM_REGISTER {
  Class<MyStruct>{"MyStruct"}
    .Constructor<>("Init")
    .Method("Func", &MyStruct::Func)
    .Property("data_", &MyStruct::data_);
}

TEST (reflection, readme) {
  const Meta* meta_data = GetMetaDatabase().at(typeid(MyStruct).hash_code());
  for (const Property* prop : meta_data->GetAllProperty()) {
      std::cout << "name: " << prop->GetPropertyName();
  }

  for (const Method* method : meta_data->GetAllMethod()) {
      std::cout << "name: " << method->GetMethodName();
  }

  Variable var = meta_data->CreateInstance("Init");    // will invoke the previously registered ctor

  const Constructor* ctor = meta_data->GetConstructor("Init");  // 2nd way with the constructor class
  var = ctor->Invoke();
  std::cout << var.GetType()->GetTypeName(); // prints 'MyStruct'

  Variable data_variable = var.GetPropertyVariable("data_");
  assert(data_variable.IsRefrenceVariable() == true);
  assert(data_variable.IsPropertyVariable() == true);

  data_variable = Variable::CreateVariable(23); // set property
  std::cout << data_variable.GetValueCast<int>() << std::endl; // get property

  const Method* method = meta_data->GetMethod("Func");
  Variable arg_var = Variable::CreateVariable(42.0);
  method->Invoke(var, arg_var); // 1st way

  var.Invoke("Func", arg_var); // 2nd way
}



