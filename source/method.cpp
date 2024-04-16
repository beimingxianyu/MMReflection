#include "method.h"

bool MM::Reflection::AllTypeIsArgTypeFromVector(const MM::Reflection::MethodWrapperBase& method, const std::vector<MM::Reflection::Variable*>& args) {
  if (!method.IsValid()) {
    return false;
  }
  if (args.size() != method.GetArgumentNumber()) {
    return false;
  }
  for (const auto& arg: args) {
    if (arg == nullptr || !(arg->IsValid())) {
      return false;
    }
  }
  for (std::uint32_t arg_index = 0; arg_index != method.GetArgumentNumber(); ++arg_index) {
    if (args[arg_index]->GetType()->GetTypeHashCode() != method.GetArgumentType(arg_index)->GetTypeHashCode()) {
      return false;
    }
  }

  return true;
}

bool MM::Reflection::VariableInstaceIsValid(const MM::Reflection::MethodWrapperBase& method, MM::Reflection::Variable& instance) {
    const Type* instance_type = instance.GetType();
    const Type* class_type = method.GetClassType();
    if (instance_type == nullptr || class_type == nullptr) {
      return false;
    }
    return (instance_type->GetTypeHashCode() == class_type->GetTypeHashCode()) && (instance_type->IsConst() == class_type->IsConst());
}

MM::Reflection::MethodWrapperBase::MethodWrapperBase(
    const std::string& method_name)
    : method_name_(method_name) {}
const std::string& MM::Reflection::MethodWrapperBase::GetMethodName() const {
  return method_name_;
}

std::size_t MM::Reflection::MethodWrapperBase::HashCode() const {
  return std::hash<std::string>{}(method_name_);
}

bool MM::Reflection::MethodWrapperBase::IsStatic() const { return false; }

bool MM::Reflection::MethodWrapperBase::IsConst() const { return false; }

bool MM::Reflection::MethodWrapperBase::IsVolatile() const { return false; }

std::uint32_t MM::Reflection::MethodWrapperBase::GetArgumentNumber() const {
  return 0;
}

const MM::Reflection::Type* MM::Reflection::MethodWrapperBase::GetReturnType()
    const {
  return nullptr;
}

const MM::Reflection::Type* MM::Reflection::MethodWrapperBase::GetArgumentType(
    std::uint32_t argument_index) const {
  return nullptr;
}

const MM::Reflection::Type* MM::Reflection::MethodWrapperBase::GetClassType()
    const {
  return nullptr;
}

const MM::Reflection::Meta* MM::Reflection::MethodWrapperBase::GetReturnMeta()
    const {
  return nullptr;
}

const MM::Reflection::Meta* MM::Reflection::MethodWrapperBase::GetArgumentMeta(
    std::uint32_t argument_index) const {
  return nullptr;
}

const MM::Reflection::Meta* MM::Reflection::MethodWrapperBase::GetClassMeta()
    const {
  return nullptr;
}

MM::Reflection::Variable MM::Reflection::MethodWrapperBase::Invoke(
    Variable& instance) const {
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::MethodWrapperBase::Invoke(
    Variable& instance, Variable& arg1) const {
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::MethodWrapperBase::Invoke(
    Variable& instance, Variable& arg1, Variable& arg2) const {
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::MethodWrapperBase::Invoke(
    Variable& instance, Variable& arg1, Variable& arg2, Variable& arg3) const {
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::MethodWrapperBase::Invoke(
    Variable& instance, Variable& arg1, Variable& arg2, Variable& arg3,
    Variable& arg4) const {
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::MethodWrapperBase::Invoke(
    Variable& instance, Variable& arg1, Variable& arg2, Variable& arg3,
    Variable& arg4, Variable& arg5) const {
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::MethodWrapperBase::Invoke(
    Variable& instance, Variable& arg1, Variable& arg2, Variable& arg3,
    Variable& arg4, Variable& arg5, Variable& arg6) const {
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::MethodWrapperBase::Invoke(
    Variable& instance, std::vector<Variable*>& args) const {
  return Variable{};
}

bool MM::Reflection::MethodWrapperBase::IsValid() const {
  return !method_name_.empty();
}

const std::string& MM::Reflection::Method::GetMethodName() const {
  if (!IsValid()) {
    return GetEmptyString();
  }

  return method_wrapper_->GetMethodName();
}

std::size_t MM::Reflection::Method::HashCode() const {
  if (!IsValid()) {
    return 0;
  }

  return method_wrapper_->HashCode();
}

bool MM::Reflection::Method::IsValid() const {
  return method_wrapper_ != nullptr && method_wrapper_->IsValid();
}

bool MM::Reflection::Method::IsStatic() const {
  if (!IsValid()) {
    return false;
  }

  return method_wrapper_->IsStatic();
}

std::size_t MM::Reflection::Method::GetArgumentNumber() const {
  if (!IsValid()) {
    return 0;
  }

  return method_wrapper_->GetArgumentNumber();
}

const MM::Reflection::Type* MM::Reflection::Method::GetClassType() const {
  if (!IsValid()) {
    return nullptr;
  }

  return method_wrapper_->GetClassType();
}

const MM::Reflection::Type* MM::Reflection::Method::GetArgumentType(
    std::uint32_t argument_index) const {
  if (!IsValid()) {
    return nullptr;
  }

  return method_wrapper_->GetArgumentType(argument_index);
}

const MM::Reflection::Type* MM::Reflection::Method::GetReturnType() const {
  if (!IsValid()) {
    return nullptr;
  }

  return method_wrapper_->GetReturnType();
}

const MM::Reflection::Meta* MM::Reflection::Method::GetClassMeta() const {
  const Type* class_type = method_wrapper_->GetClassType();
  if (class_type == nullptr) {
    return nullptr;
  }

  return class_type->GetMeta();
}

const MM::Reflection::Meta* MM::Reflection::Method::GetArgumentMeta(
    std::uint32_t argument_index) const {
  const Type* argument_type = method_wrapper_->GetArgumentType(argument_index);
  if (argument_type == nullptr) {
    return nullptr;
  }

  return argument_type->GetMeta();
}

const MM::Reflection::Meta* MM::Reflection::Method::GetReturnMeta() const {
  const Type* return_type = method_wrapper_->GetReturnType();
  if (return_type == nullptr) {
    return nullptr;
  }

  return return_type->GetMeta();
}

MM::Reflection::Variable MM::Reflection::Method::Invoke(
    Variable& instance) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(instance);
}

MM::Reflection::Variable MM::Reflection::Method::Invoke(Variable& instance,
                                                        Variable& arg1) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(instance, arg1);
}

MM::Reflection::Variable MM::Reflection::Method::Invoke(Variable& instance,
                                                        Variable& arg1,
                                                        Variable& arg2) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(instance, arg1, arg2);
}

MM::Reflection::Variable MM::Reflection::Method::Invoke(Variable& instance,
                                                        Variable& arg1,
                                                        Variable& arg2,
                                                        Variable& arg3) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(instance, arg1, arg2, arg3);
}

MM::Reflection::Variable MM::Reflection::Method::Invoke(Variable& instance,
                                                        Variable& arg1,
                                                        Variable& arg2,
                                                        Variable& arg3,
                                                        Variable& arg4) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(instance, arg1, arg2, arg3, arg4);
}

MM::Reflection::Variable MM::Reflection::Method::Invoke(
    Variable& instance, Variable& arg1, Variable& arg2, Variable& arg3,
    Variable& arg4, Variable& arg5) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(instance, arg1, arg2, arg3, arg4, arg5);
}

MM::Reflection::Variable MM::Reflection::Method::Invoke(
    Variable& instance, Variable& arg1, Variable& arg2, Variable& arg3,
    Variable& arg4, Variable& arg5, Variable& arg6) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(instance, arg1, arg2, arg3, arg4, arg5, arg6);
}

MM::Reflection::Variable MM::Reflection::Method::Invoke(
    Variable& instance, std::vector<Variable*>& args) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(instance, args);
}

MM::Reflection::Variable MM::Reflection::Method::Invoke(
    Variable& instance, std::vector<Variable*>&& args) const {
  return Invoke(instance, args);
}
