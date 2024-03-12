#include "property.h"

std::size_t MM::Reflection::Property::GetPropertySize() const {
  return property_wrapper_->GetPropertySize();
}
const std::string& MM::Reflection::Property::GetPropertyName() const {
  return property_wrapper_->GetPropertyName();
}
std::size_t MM::Reflection::Property::GetPropertyOffset() const {
  return property_wrapper_->GetPropertyOffset();
}
void* MM::Reflection::Property::GetStaticPropertyAddress() const {
  return property_wrapper_->GetStaticPropertyAddress();
}
bool MM::Reflection::Property::IsStatic() const {
  return property_wrapper_->IsStatic();
}
std::size_t MM::Reflection::Property::HashCode() const {
  return property_wrapper_->HashCode();
}
bool MM::Reflection::Property::IsValid() const {
  return property_wrapper_ != nullptr && property_wrapper_->IsValid();
}
const MM::Reflection::Type* MM::Reflection::Property::GetType() const {
  if (!IsValid()) {
    return nullptr;
  }
  return property_wrapper_->GetType();
}
const MM::Reflection::Type* MM::Reflection::Property::GetClassType() const {
  if (!IsValid()) {
    return nullptr;
  }

  return property_wrapper_->GetClassType();
}
const MM::Reflection::Meta* MM::Reflection::Property::GetMate() const {
  if (!IsValid()) {
    return nullptr;
  }

  return property_wrapper_->GetMeta();
}
const MM::Reflection::Meta* MM::Reflection::Property::GetClassMeta() const {
  if (!IsValid()) {
    return nullptr;
  }

  return property_wrapper_->GetClassMeta();
}
MM::Reflection::Variable MM::Reflection::Property::GetPropertyVariable(
    const Variable& class_variable) const {
  if (!IsValid()) {
    return Variable{};
  }
  const Type* class_type = class_variable.GetType();
  if (class_type == nullptr) {
    return Variable{};
  }
  if (class_type->GetTypeHashCode() ==
      property_wrapper_->GetClassType()->GetTypeHashCode()) {
    return property_wrapper_->GetPropertyVariable(class_variable);
  }
  return Variable{};
}

MM::Reflection::Variable MM::Reflection::Property::GetPropertyVariable(
    Variable& class_variable) const {
  if (!IsValid()) {
    return Variable{};
  }
  const Type* class_type = class_variable.GetType();
  if (class_type == nullptr) {
    return Variable{};
  }
  if (class_type->GetTypeHashCode() ==
      property_wrapper_->GetClassType()->GetOriginalTypeHashCode()) {
    return property_wrapper_->GetPropertyVariable(class_variable);
  }
  return Variable{};
}
