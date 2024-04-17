//
// Created by lsj on 24-3-9.
//

#include "constructor.h"

MM::Reflection::Variable MM::Reflection::Constructor::empty_variable{};

const std::string& MM::Reflection::Constructor::GetConstructorName() const {
  if (!IsValid()) {
    return GetEmptyString();
  }

  return common_constructor_wrapper_->GetMethodName();
}

std::size_t MM::Reflection::Constructor::HashCode() const {
  if (!IsValid()) {
    return 0;
  }

  return common_constructor_wrapper_->HashCode();
}

bool MM::Reflection::Constructor::IsValid() const {
  return common_constructor_wrapper_ != nullptr && common_constructor_wrapper_->IsValid();
}

std::size_t MM::Reflection::Constructor::GetArgumentNumber() const {
  if (!IsValid()) {
    return 0;
  }

  return common_constructor_wrapper_->GetArgumentNumber();
}

const MM::Reflection::Type* MM::Reflection::Constructor::GetClassType() const {
  if (!IsValid()) {
    return nullptr;
  }

  return common_constructor_wrapper_->GetClassType();
}

const MM::Reflection::Type* MM::Reflection::Constructor::GetArgumentType(
    std::uint32_t argument_index) const {
  if (!IsValid()) {
    return nullptr;
  }

  return common_constructor_wrapper_->GetArgumentType(argument_index);
}

const MM::Reflection::Meta* MM::Reflection::Constructor::GetClassMeta() const {
  const Type* class_type = common_constructor_wrapper_->GetClassType();
  if (class_type == nullptr) {
    return nullptr;
  }

  return class_type->GetMeta();
}

const MM::Reflection::Meta* MM::Reflection::Constructor::GetArgumentMeta(
    std::uint32_t argument_index) const {
  const Type* argument_type = common_constructor_wrapper_->GetArgumentType(argument_index);
  if (argument_type == nullptr) {
    return nullptr;
  }

  return argument_type->GetMeta();
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke() const {
  if (!IsValid()) {
    return Variable{};
  }

  return common_constructor_wrapper_->Invoke(empty_variable);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    void* placement_address) const {
  if (!IsValid()) {
    return Variable{};
  }

  Variable address_variable = Variable::CreateVariable(placement_address);
  placement_constructor_wrapper_->Invoke(empty_variable, address_variable);
  return Variable{static_cast<VariableWrapperBase*>(placement_address), true};
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1) const {
  if (!IsValid()) {
    return Variable{};
  }

  return common_constructor_wrapper_->Invoke(empty_variable, arg1);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    void* placement_address, Variable& arg1) const {
  if (!IsValid()) {
    return Variable{};
  }

  Variable address_variable = Variable::CreateVariable(placement_address);
  placement_constructor_wrapper_->Invoke(empty_variable, address_variable,
                                         arg1);
  return Variable{static_cast<VariableWrapperBase*>(placement_address), true};
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2) const {
  if (!IsValid()) {
    return Variable{};
  }

  return common_constructor_wrapper_->Invoke(empty_variable, arg1, arg2);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    void* placement_address, Variable& arg1, Variable& arg2) const {
  if (!IsValid()) {
    return Variable{};
  }

  Variable address_variable = Variable::CreateVariable(placement_address);
  placement_constructor_wrapper_->Invoke(empty_variable, address_variable, arg1,
                                         arg2);
  return Variable{static_cast<VariableWrapperBase*>(placement_address), true};
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2, Variable& arg3) const {
  if (!IsValid()) {
    return Variable{};
  }

  return common_constructor_wrapper_->Invoke(empty_variable, arg1, arg2, arg3);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    void* placement_address, Variable& arg1, Variable& arg2,
    Variable& arg3) const {
  if (!IsValid()) {
    return Variable{};
  }

  Variable address_variable = Variable::CreateVariable(placement_address);
  placement_constructor_wrapper_->Invoke(empty_variable, address_variable, arg1,
                                         arg2, arg3);
  return Variable{static_cast<VariableWrapperBase*>(placement_address), true};
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2, Variable& arg3, Variable& arg4) const {
  if (!IsValid()) {
    return Variable{};
  }

  return common_constructor_wrapper_->Invoke(empty_variable, arg1, arg2, arg3,
                                             arg4);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    void* placement_address, Variable& arg1, Variable& arg2, Variable& arg3,
    Variable& arg4) const {
  if (!IsValid()) {
    return Variable{};
  }

  Variable address_variable = Variable::CreateVariable(placement_address);
  placement_constructor_wrapper_->Invoke(empty_variable, address_variable, arg1,
                                         arg2, arg3, arg4);
  return Variable{static_cast<VariableWrapperBase*>(placement_address), true};
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2, Variable& arg3, Variable& arg4,
    Variable& arg5) const {
  if (!IsValid()) {
    return Variable{};
  }

  return common_constructor_wrapper_->Invoke(empty_variable, arg1, arg2, arg3,
                                             arg4, arg5);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    void* placement_address, Variable& arg1, Variable& arg2, Variable& arg3,
    Variable& arg4, Variable& arg5) const {
  if (!IsValid()) {
    return Variable{};
  }

  Variable address_variable = Variable::CreateVariable(placement_address);
  placement_constructor_wrapper_->Invoke(
      empty_variable, address_variable, arg1, arg2, arg3, arg4, arg5);
  return Variable{static_cast<VariableWrapperBase*>(placement_address), true};
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    Variable& arg1, Variable& arg2, Variable& arg3, Variable& arg4,
    Variable& arg5, Variable& arg6) const {
  if (!IsValid()) {
    return Variable{};
  }

  return common_constructor_wrapper_->Invoke(empty_variable, arg1, arg2, arg3,
                                             arg4, arg5, arg6);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    void* placement_address, Variable& arg1, Variable& arg2, Variable& arg3,
    Variable& arg4, Variable& arg5, Variable& arg6) const {
  if (!IsValid()) {
    return Variable{};
  }

  Variable address_variable = Variable::CreateVariable(placement_address);
  std::vector<Variable*> placement_args = {
      &address_variable, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6};
  placement_constructor_wrapper_->Invoke(empty_variable, placement_args);
  return Variable{static_cast<VariableWrapperBase*>(placement_address), true};
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    std::vector<Variable*>& args) const {
  if (!IsValid()) {
    return Variable{};
  }

  return common_constructor_wrapper_->Invoke(empty_variable, args);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    void* placement_address, std::vector<Variable*>& args) const {
  if (!IsValid()) {
    return Variable{};
  }

  Variable address_variable = Variable::CreateVariable(placement_address);
  std::vector<Variable*> placement_args(1 + args.size());
  placement_args[0] = &address_variable;
  for (std::uint32_t i = 1; i != placement_args.size(); ++i) {
    placement_args[i] = args[i - 1];
  }
  placement_constructor_wrapper_->Invoke(empty_variable, placement_args);
  return Variable{static_cast<VariableWrapperBase*>(placement_address), true};
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    std::vector<Variable*>&& args) const {
  return Invoke(args);
}

MM::Reflection::Variable MM::Reflection::Constructor::Invoke(
    void* placement_address, std::vector<Variable*>&& args) const {
  return Invoke(placement_address, args);
}

MM::Reflection::Constructor::Constructor(
    std::unique_ptr<MethodWrapperBase>&& common_constructor_wrapper,
    std::unique_ptr<MethodWrapperBase>&& placement_constructor_wrapper)
    : common_constructor_wrapper_(std::move(common_constructor_wrapper)),
      placement_constructor_wrapper_(std::move(placement_constructor_wrapper)) {
}