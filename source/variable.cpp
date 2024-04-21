#include "variable.h"

#include <cstring>
#include <memory>

#include "meta.h"
#include "method.h"
#include "type.h"

MM::Reflection::Type MM::Reflection::Variable::EmptyType{};

MM::Reflection::Variable::~Variable() { Destroy(); };

MM::Reflection::Variable::Variable(
    const Variable& other) {
  if (!other.IsValid()) {
    return;
  }

  const VariableWrapperBase* wrapper_base_ptr = other.GetWrapperBasePtr();
  assert(wrapper_base_ptr != nullptr);
  if (other.variable_type_ == VariableType::SMALL_OBJECT) {
    variable_type_ = VariableType::SMALL_OBJECT;
    // GetWrapperBasePtr depend on variable_type_
    VariableWrapperBase* new_ptr = wrapper_base_ptr->CopyToBasePointer(GetWrapperBasePtr());
  } else {
  VariableWrapperBase* new_ptr = wrapper_base_ptr->CopyToBasePointer(nullptr);

  wrapper_.common_wrapper_ = new_ptr;
  variable_type_ = VariableType::COMMON_OBJECT;
  }
}

MM::Reflection::Variable::Variable(
    Variable&& other) noexcept {
  if (!other.IsValid()) {
    return;
  }

  VariableWrapperBase* wrapper_base_ptr = other.GetWrapperBasePtr();
  assert(wrapper_base_ptr != nullptr);
  if (other.variable_type_ == VariableType::SMALL_OBJECT) {
    variable_type_ = VariableType::SMALL_OBJECT;
    // GetWrapperBasePtr depend on variable_type_
    if (other.IsRefrenceVariable()) {
      VariableWrapperBase* new_ptr = wrapper_base_ptr->CopyToBasePointer(GetWrapperBasePtr());
    } else {
      VariableWrapperBase* new_ptr = wrapper_base_ptr->MoveToBasePointer(GetWrapperBasePtr());
    }
  } else {
    VariableWrapperBase* new_ptr = wrapper_base_ptr->MoveToBasePointer(nullptr);

    wrapper_.common_wrapper_ = new_ptr;
    variable_type_ = VariableType::COMMON_OBJECT;
  }
}

MM::Reflection::Variable& MM::Reflection::Variable::operator=(
    const Variable& other) {
  if (this == std::addressof(other)) {
    return *this;
  }
  if (!IsValid() || !other.IsValid()) {
    return *this;
  }
  if (GetType()->GetTypeHashCode() != other.GetType()->GetTypeHashCode()) {
    return *this;
  }

  CopyValue(other.GetValue());

  return *this;
}

MM::Reflection::Variable& MM::Reflection::Variable::operator=(
    Variable&& other) noexcept {
  if (this == std::addressof(other)) {
    return *this;
  }
  if (!IsValid() && other.IsValid()) {
    memcpy(&wrapper_, &other.wrapper_, sizeof(other.wrapper_));
    variable_type_ = other.variable_type_;

    other.variable_type_ = VariableType::INVALID;

    return *this;
  }

  if (!IsValid() || !other.IsValid()) {
    return *this;
  }
  if (GetType()->GetTypeHashCode() !=
      other.GetType()->GetTypeHashCode()) {
    return *this;
  }

  MoveValue(other.GetValue());

  return *this;
}

MM::Reflection::Variable::Variable(
    VariableWrapperBase* variable_wrapper,
    const bool placement /*= false*/)
    : wrapper_(), variable_type_(VariableType::COMMON_OBJECT) {
  wrapper_.common_wrapper_ = variable_wrapper;
  if (placement) {
    variable_type_ = VariableType::PLACMENT_OBJECT;
  }
}

MM::Reflection::Variable::operator bool() const { return IsValid();}

bool MM::Reflection::Variable::IsValid() const {
  return variable_type_ != VariableType::INVALID;
}

const void* MM::Reflection::Variable::GetValue() const {
  if (!IsValid()) {
    return nullptr;
  }

  const VariableWrapperBase* wrapper_base_ptr = GetWrapperBasePtr();
  assert(wrapper_base_ptr != nullptr);
  return wrapper_base_ptr->GetValue();
}

void* MM::Reflection::Variable::GetValue() {
  if (!IsValid()) {
    return nullptr;
  }

  VariableWrapperBase* wrapper_base_ptr = GetWrapperBasePtr();
  assert(wrapper_base_ptr != nullptr);
  return wrapper_base_ptr->GetValue();
}

bool MM::Reflection::Variable::CopyValue(const void* other) {
  if (!IsValid()) {
    return false;
  }

  VariableWrapperBase* wrapper_base_ptr = GetWrapperBasePtr();
  if (wrapper_base_ptr == nullptr) {
    return false;
  }

  return wrapper_base_ptr->CopyValue(other);
}

bool MM::Reflection::Variable::MoveValue(void* other) {
  if (!IsValid()) {
    return false;
  }
  VariableWrapperBase* wrapper_base_ptr = GetWrapperBasePtr();
  if (wrapper_base_ptr == nullptr) {
    return false;
  }

  return wrapper_base_ptr->MoveValue(other);
}

const MM::Reflection::Type* MM::Reflection::Variable::GetType() const {
  if (!IsValid()) {
    return &EmptyType;
  }

  const VariableWrapperBase* wrapper_base_ptr = GetWrapperBasePtr();
  assert(wrapper_base_ptr != nullptr);
  return wrapper_base_ptr->GetType();
}

const MM::Reflection::Meta* MM::Reflection::Variable::
GetMeta() const {
  if (!IsValid()) {
    return nullptr;
  }
  const VariableWrapperBase* wrapper_base_ptr = GetWrapperBasePtr();
  assert(wrapper_base_ptr != nullptr);
  return wrapper_base_ptr->GetMeta();
}

MM::Reflection::Variable MM::Reflection::Variable::GetPropertyVariable(
    const std::string& property_name) {
  const Meta* metadata = GetMeta();
    if (metadata != nullptr) {
      const Property* target_property = metadata->GetProperty(property_name);
      if (target_property != nullptr) {
        assert(target_property->IsValid());
        return target_property->GetPropertyVariable(*this);
      }
    }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::GetPropertyVariable(
    const std::string& property_name) const {
  const Meta* metadata = GetMeta();
    if (metadata != nullptr) {
      const Property* target_property = metadata->GetProperty(property_name);
      if (target_property != nullptr) {
        assert(target_property->IsValid());
        return target_property->GetPropertyVariable(*this);
      }
    }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::Invoke(
    const std::string& method_name) {
  if (IsValid()) {
      const Meta* metadata = GetMeta();
      if (metadata != nullptr) {
        const Method* method{metadata->GetMethod(method_name)};
        if (method != nullptr) {
          assert(method->IsValid());
          return method->Invoke(*this);
        }
        return Variable{};
      }
  }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::Invoke(
    const std::string& method_name, Variable& arg1) {
  if (IsValid()) {
      const Meta* metadata = GetMeta();
      if (metadata != nullptr) {
        const Method* method{metadata->GetMethod(method_name)};
        if (method != nullptr) {
          assert(method->IsValid());
          return method->Invoke(*this, arg1);
        }
        return Variable{};
      }
  }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::Invoke(
    const std::string& method_name, Variable& arg1, Variable& arg2) {
  if (IsValid()) {
      const Meta* metadata = GetMeta();
      if (metadata != nullptr) {
        const Method* method{metadata->GetMethod(method_name)};
        if (method != nullptr) {
          assert(method->IsValid());
          return method->Invoke(*this, arg1, arg2);
        }
        return Variable{};
      }
  }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::Invoke(
    const std::string& method_name, Variable& arg1, Variable& arg2,
    Variable& arg3) {
  if (IsValid()) {
      const Meta* metadata = GetMeta();
      if (metadata != nullptr) {
        const Method* method{metadata->GetMethod(method_name)};
        if (method != nullptr) {
          assert(method->IsValid());
          return method->Invoke(*this, arg1, arg2, arg3);
        }
        return Variable{};
      }
  }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::Invoke(
    const std::string& method_name, Variable& arg1, Variable& arg2,
    Variable& arg3, Variable& arg4) {
  if (IsValid()) {
      const Meta* metadata = GetMeta();
      if (metadata != nullptr) {
        const Method* method{metadata->GetMethod(method_name)};
        if (method != nullptr) {
          assert(method->IsValid());
          return method->Invoke(*this, arg1, arg2, arg3, arg4);
        }
        return Variable{};
      }
  }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::Invoke(
    const std::string& method_name, Variable& arg1, Variable& arg2,
    Variable& arg3, Variable& arg4, Variable& arg5) {
  if (IsValid()) {
      const Meta* metadata = GetMeta();
      if (metadata != nullptr) {
        const Method* method{metadata->GetMethod(method_name)};
        if (method != nullptr) {
          assert(method->IsValid());
          return method->Invoke(*this, arg1, arg2, arg3, arg4, arg5);
        }
        return Variable{};
      }
  }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::Invoke(
    const std::string& method_name, Variable& arg1, Variable& arg2,
    Variable& arg3, Variable& arg4, Variable& arg5, Variable& arg6) {
  if (IsValid()) {
      const Meta* metadata = GetMeta();
      if (metadata != nullptr) {
        const Method* method{metadata->GetMethod(method_name)};
        if (method != nullptr) {
          assert(method->IsValid());
          return method->Invoke(*this, arg1, arg2, arg3, arg4, arg5, arg6);
        }
        return Variable{};
      }
  }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::Invoke(
    const std::string& method_name, std::vector<Variable*>& args) {
  if (IsValid()) {
      const Meta* metadata = GetMeta();
      if (metadata != nullptr) {
        const Method* method{metadata->GetMethod(method_name)};
        if (method != nullptr) {
          assert(method->IsValid());
          return method->Invoke(*this, args);
        }
        return Variable{};
      }
  }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Variable::Invoke(
    const std::string& method_name, std::vector<Variable*>&& args) {
  if (IsValid()) {
      const Meta* metadata = GetMeta();
      if (metadata != nullptr) {
        const Method* method{metadata->GetMethod(method_name)};
        if (method != nullptr) {
          assert(method->IsValid());
          return method->Invoke(*this, args);
        }
        return Variable{};
      }
  }
  return Variable{};
}

void* MM::Reflection::Variable::ReleaseOwnership() {
  void* result = nullptr;
  switch (variable_type_) {
    case VariableType::COMMON_OBJECT:
    case VariableType::PLACMENT_OBJECT:
      result = GetWrapperBasePtr();
      wrapper_.common_wrapper_ = nullptr;
      variable_type_ = VariableType::INVALID;
      return result;
    default:
      return nullptr;
  }
}

void MM::Reflection::Variable::Destroy() {
  switch (variable_type_) {
    case VariableType::SMALL_OBJECT:
      wrapper_.small_wrapper_.GetWrpperBasePtr()->Destroy();
      wrapper_.small_wrapper_ = SmallObject{};
      variable_type_ = VariableType::INVALID;
      break;
    case VariableType::COMMON_OBJECT:
      delete wrapper_.common_wrapper_;
      wrapper_.common_wrapper_ = nullptr;
      variable_type_ = VariableType::INVALID;
      break;
    case VariableType::PLACMENT_OBJECT:
      wrapper_.placement_wrapper_->Destroy();
      wrapper_.placement_wrapper_ = nullptr;
      variable_type_ = VariableType::INVALID;
      break;
    default:
      break;
  }
}

MM::Reflection::Variable::WrapperObject* MM::Reflection::Variable::GetWrapperObjectAddress() { return &wrapper_.small_wrapper_; }

bool MM::Reflection::VariableWrapperBase::IsVoid() const { return false; }

bool MM::Reflection::VariableWrapperBase::IsRefrenceVariable() const {
  return false;
}

void MM::Reflection::VariableWrapperBase::Destroy() {}

std::uint64_t MM::Reflection::VariableWrapperBase::GetWrapperSize() const {
  return 0;
}

bool MM::Reflection::VariableWrapperBase::IsPropertyVariable() const {
  return false;
}

MM::Reflection::VariableWrapperBase*
MM::Reflection::VariableWrapperBase::CopyToBasePointer(void* placement_address) const {
  return nullptr;
}

MM::Reflection::VariableWrapperBase*
MM::Reflection::VariableWrapperBase::MoveToBasePointer(void* placement_address) {
  return nullptr;
}

const void* MM::Reflection::VariableWrapperBase::GetValue() const {
  return nullptr;
}

void* MM::Reflection::VariableWrapperBase::GetValue() { return nullptr; }

bool MM::Reflection::VariableWrapperBase::CopyValue(const void*) {
  return false;
}

bool MM::Reflection::VariableWrapperBase::MoveValue(void*) {
  return false;
}

MM::Reflection::Variable
MM::Reflection::VariableWrapperBase::GetPropertyVariable(
    const std::string&) const {
  return Variable{};
}

const MM::Reflection::Type* MM::Reflection::VariableWrapperBase::GetType()
    const {
  return nullptr;
}

const MM::Reflection::Type*
MM::Reflection::VariableWrapperBase::GetPropertyRealType() const {
  return GetType();
}

const MM::Reflection::Meta* MM::Reflection::VariableWrapperBase::GetMeta()
    const {
  return nullptr;
}

std::uint64_t MM::Reflection::VoidVariable::GetWrapperSize() const {
  return sizeof(VoidVariable);
}

bool MM::Reflection::VoidVariable::IsVoid() const { return true; }

MM::Reflection::VariableWrapperBase*
MM::Reflection::VoidVariable::CopyToBasePointer(void* placement_addresss) const {
  if (placement_addresss != nullptr) {
    return new (placement_addresss)VoidVariable{};
  } else {
    return new VoidVariable{};
  }
}

MM::Reflection::VariableWrapperBase*
MM::Reflection::VoidVariable::MoveToBasePointer(void* placement_addresss) {
  if (placement_addresss != nullptr) {
    return new (placement_addresss)VoidVariable{};
  } else {
    return new VoidVariable{};
  }
}

const MM::Reflection::Type* MM::Reflection::VoidVariable::GetType() const {
  const MM::Reflection::Type& Result = MM::Reflection::Type::CreateType<void>();
  return &Result;
}
const MM::Reflection::Meta* MM::Reflection::VoidVariable::GetMeta() const {
  return GetType()->GetMeta();
}

bool MM::Reflection::Variable::IsVoid() const {
  if (!IsValid()) {
      return false;
  }

  const VariableWrapperBase* wrapper_base_ptr = GetWrapperBasePtr();
  assert(wrapper_base_ptr != nullptr);

  return wrapper_base_ptr->IsVoid();
}

bool MM::Reflection::Variable::IsRefrenceVariable() const {
  if (!IsValid()) {
    return false;
  }

  const VariableWrapperBase* wrapper_base_ptr = GetWrapperBasePtr();
  assert(wrapper_base_ptr != nullptr);

  return wrapper_base_ptr->IsRefrenceVariable();
}

bool MM::Reflection::Variable::IsPropertyVariable() const {
  if (!IsValid()) {
    return false;
  }

  const VariableWrapperBase* wrapper_base_ptr = GetWrapperBasePtr();
  assert(wrapper_base_ptr != nullptr);

  return wrapper_base_ptr->IsPropertyVariable();
}

MM::Reflection::Variable MM::Reflection::Variable::CreateVoidVariable() {
  static_assert(sizeof(VoidVariable) < sizeof(SmallObject));
  Variable variable{};
  variable.variable_type_ = VariableType::SMALL_OBJECT;
  void* small_object_address = &variable.wrapper_.small_wrapper_;
  new (small_object_address) VoidVariable{};
  return variable;
}
