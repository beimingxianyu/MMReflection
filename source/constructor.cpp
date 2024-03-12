//
// Created by lsj on 24-3-9.
//

#include "constructor.h"

MM::Reflection::Variable MM::Reflection::Constructor::empty_variable{};

const std::string& MM::Reflection::Constructor::GetConstructorName() const {
  if (!IsValid()) {
    return GetEmptyString();
  }

  return method_wrapper_->GetMethodName();
}

std::size_t MM::Reflection::Constructor::HashCode() const {
  if (!IsValid()) {
    return 0;
  }

  return method_wrapper_->HashCode();
}

bool MM::Reflection::Constructor::IsValid() const {
  return method_wrapper_ != nullptr && method_wrapper_->IsValid();
}

std::size_t MM::Reflection::Constructor::GetArgumentNumber() const {
  if (!IsValid()) {
    return 0;
  }

  return method_wrapper_->GetArgumentNumber();
}

const MM::Reflection::Type* MM::Reflection::Constructor::GetClassType() const {
  if (!IsValid()) {
    return nullptr;
  }

  return method_wrapper_->GetClassType();
}

const MM::Reflection::Type* MM::Reflection::Constructor::GetArgumentType(
    std::uint32_t argument_index) const {
  if (!IsValid()) {
    return nullptr;
  }

  return method_wrapper_->GetArgumentType(argument_index);
}

const MM::Reflection::Meta* MM::Reflection::Constructor::GetClassMeta() const {
  const Type* class_type = method_wrapper_->GetClassType();
  if (class_type == nullptr) {
    return nullptr;
  }

  return class_type->GetMate();
}

const MM::Reflection::Meta* MM::Reflection::Constructor::GetArgumentMeta(
    std::uint32_t argument_index) const {
  const Type* argument_type = method_wrapper_->GetArgumentType(argument_index);
  if (argument_type == nullptr) {
    return nullptr;
  }

  return argument_type->GetMate();
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke() const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(empty_variable);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(empty_variable, arg1);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(empty_variable, arg1, arg2);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2, Variable& arg3) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(empty_variable, arg1, arg2, arg3);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2, Variable& arg3, Variable& arg4) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(empty_variable, arg1, arg2, arg3, arg4);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2, Variable& arg3, Variable& arg4,
    Variable& arg5) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(empty_variable, arg1, arg2, arg3, arg4, arg5);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2, Variable& arg3, Variable& arg4,
    Variable& arg5, Variable& arg6) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(empty_variable, arg1, arg2, arg3, arg4, arg5,
                                 arg6);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    std::vector<Variable*>& args) const {
  if (!IsValid()) {
    return Variable{};
  }

  return method_wrapper_->Invoke(empty_variable, args);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    std::vector<Variable*>&& args) const {
  return Invoke(args);
}

MM::Reflection::Constructor::Constructor(
    std::unique_ptr<MethodWrapperBase>&& method_wrapper)
    : method_wrapper_(std::move(method_wrapper)) {}