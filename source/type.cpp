#include "type.h"

#include "meta.h"

bool MM::Reflection::TypeWrapper<void>::IsVoid() const { return true; }

bool MM::Reflection::TypeWrapper<void>::IsRegistered() const { return true; }

bool MM::Reflection::TypeWrapper<void>::IsConst() const { return false; }

bool MM::Reflection::TypeWrapper<void>::IsReference() const { return false; }

bool MM::Reflection::TypeWrapper<void>::IsArray() const { return false; }

bool MM::Reflection::TypeWrapper<void>::IsPointer() const { return false; }

bool MM::Reflection::TypeWrapper<void>::IsEnum() const { return false; }

bool MM::Reflection::TypeWrapper<void>::HaveDefaultConstructor() const { return false; }

bool MM::Reflection::TypeWrapper<void>::HaveDestructor() const { return false; }

bool MM::Reflection::TypeWrapper<void>::HaveCopyConstructor() const { return false; }

bool MM::Reflection::TypeWrapper<void>::HaveMoveConstructor() const { return false; }

bool MM::Reflection::TypeWrapper<void>::HaveCopyAssign() const { return false; }

bool MM::Reflection::TypeWrapper<void>::HaveMoveAssign() const { return false; }

std::size_t MM::Reflection::TypeWrapper<void>::GetSize() const { return 0; }

std::size_t MM::Reflection::TypeWrapper<void>::GetTypeHashCode() const { return typeid(void).hash_code(); }

std::size_t MM::Reflection::TypeWrapper<void>::GetOriginalTypeHashCode() const { return GetTypeHashCode(); }

std::string MM::Reflection::TypeWrapper<void>::GetTypeName(const std::string& orignal_type_name) const {
  assert(orignal_type_name == "void");
  return orignal_type_name;
}

const MM::Reflection::Meta* MM::Reflection::TypeWrapper<void>::GetMeta() const {
  if (!IsRegistered()) {
    return nullptr;
  }

  return GetMetaDatabase().at(GetOriginalTypeHashCode());
}

MM::Reflection::Type::Type() : type_wrapper_{nullptr} {}

MM::Reflection::Type::Type(std::unique_ptr<TypeWrapperBase>&& other) noexcept
    : type_wrapper_(std::move(other)) {}

bool MM::Reflection::Type::operator==(const Type& other) const { return IsEqual(other); }

MM::Reflection::Type::operator bool() const { return type_wrapper_ != nullptr; }

bool MM::Reflection::Type::IsRegistered() const {
  if (!IsValid()) {
    return false;
  }

  return type_wrapper_->IsRegistered();
}

bool MM::Reflection::Type::IsVoid() const {
  if (!IsValid()) {
    return false;
  }

  return type_wrapper_->IsVoid();
}

bool MM::Reflection::Type::IsValid() const { return type_wrapper_ != nullptr; }

bool MM::Reflection::Type::IsEqual(const Type& other) const {
  if (IsValid() && other.IsValid()) {
    return GetTypeHashCode() == other.GetTypeHashCode();
  }
  return false;
}

bool MM::Reflection::Type::IsConst() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->IsConst();
}

bool MM::Reflection::Type::IsReference() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->IsReference();
}

bool MM::Reflection::Type::IsArray() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->IsArray();
}

bool MM::Reflection::Type::IsPointer() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->IsPointer();
}

bool MM::Reflection::Type::IsEnum() const { return type_wrapper_->IsEnum(); }

bool MM::Reflection::Type::IsTrivial() const {
  if (!IsValid()) {
    return false;
  }

  return type_wrapper_->IsTrivial();
}

bool MM::Reflection::Type::HaveDefaultConstructor() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->HaveDefaultConstructor();
}

bool MM::Reflection::Type::HaveDestructor() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->HaveDestructor();
}

bool MM::Reflection::Type::HaveCopyConstructor() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->HaveCopyConstructor();
}

bool MM::Reflection::Type::HaveMoveConstructor() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->HaveMoveConstructor();
}

bool MM::Reflection::Type::HaveCopyAssign() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->HaveCopyAssign();
}

bool MM::Reflection::Type::HaveMoveAssign() const {
  if (!IsValid()) {
    return false;
  }
  return type_wrapper_->HaveMoveAssign();
}

auto MM::Reflection::Type::OriginalTypeIsEqual(
    const Type& other) const -> bool {
  if (IsValid() && other.IsValid()) {
    return GetOriginalTypeHashCode() == other.GetOriginalTypeHashCode();
  }
  return false;
}

void MM::Reflection::Type::Swap(MM::Reflection::Type& other) {
  if (this != &other) {
    std::swap(type_wrapper_, other.type_wrapper_);
  }
}

std::size_t MM::Reflection::Type::GetSize() const {
  if (!IsValid()) {
    return 0;
  }
  return type_wrapper_->GetSize();
}

std::size_t MM::Reflection::Type::GetTypeHashCode() const {
  if (!IsValid()) {
    return 0;
  }
  return type_wrapper_->GetTypeHashCode();
}

std::size_t MM::Reflection::Type::GetOriginalTypeHashCode() const {
  if (!IsValid()) {
    return 0;
  }
  return type_wrapper_->GetOriginalTypeHashCode();
}

std::string MM::Reflection::Type::GetTypeName() const {
  if (!IsValid()) {
    return std::string();
  }

  return type_wrapper_->GetTypeName(GetOriginalTypeName());
}

std::string MM::Reflection::Type::GetOriginalTypeName() const {
  if (!IsValid()) {
    return std::string();
  }
  return GetMeta()->GetTypeName();
}

const MM::Reflection::Meta* MM::Reflection::Type::GetMeta() const {
  if (!IsValid()) {
    return nullptr;
  }
  return type_wrapper_->GetMeta();
}

MM::Reflection::TypeID MM::Reflection::Type::GetTypeID() const {
  if (!IsValid()) {
    return TypeID{};
  }

  return type_wrapper_->GetTypeID();
}
