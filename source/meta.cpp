#include "meta.h"

MM::Reflection::Meta::Meta(
    const std::string& type_name, const Type& type,
    std::unordered_map<std::string, Constructor>&& constructors,
    std::unordered_map<std::string, Method>&& methods,
    std::unordered_map<std::string, Property>&& properties)
    : type_name_(type_name),
      type_(&type),
      constructors_(std::move(constructors)),
      methods_(std::move(methods)),
      properties_(std::move(properties)) {}

MM::Reflection::Meta::Meta(Meta&& other) noexcept
    : type_name_(std::move(other.type_name_)),
      type_(std::move(other.type_)),
      constructors_(std::move(other.constructors_)),
      methods_(std::move(other.methods_)),
      properties_(std::move(other.properties_)),
      serializer_name_(std::move(other.serializer_name_)){
  empty_variable_ = std::move(other.empty_variable_);
  empty_variable_refrence_ = std::move(other.empty_variable_refrence_);
  empty_variable_const_refrence_ =
      std::move(other.empty_variable_const_refrence_);
  // If the referenced object is a small object, the reference address needs to be changed.
  if (empty_variable_.variable_type_ == Variable::VariableType::SMALL_OBJECT) {
    empty_variable_refrence_.wrapper_.small_wrapper_.ptr2 = empty_variable_.GetValue();
    empty_variable_const_refrence_.wrapper_.small_wrapper_.ptr2 = empty_variable_.GetValue();
  }
}

const std::string& MM::Reflection::Meta::GetTypeName() const {
  return type_name_;
}

const MM::Reflection::Type& MM::Reflection::Meta::GetType() const {
  assert(type_ != nullptr && type_->IsValid());
  return *type_;
}

bool MM::Reflection::Meta::HaveConstructor(
    const std::string& constructor_name) const {
  return constructors_.find(constructor_name) != constructors_.end();
}

const MM::Reflection::Constructor* MM::Reflection::Meta::GetConstructor(
    const std::string& constructor_name) const {
  if (const auto& find_result = constructors_.find(constructor_name);
      find_result != constructors_.end()) {
    return &(find_result->second);
  }

  return nullptr;
}

std::vector<const MM::Reflection::Constructor*> MM::Reflection::Meta::GetAllConstructor() const {
  if (constructors_.empty()) {
    return std::vector<const Constructor*>{};
  }

  std::vector<const Constructor*> result{};
  result.reserve(constructors_.size());
  for (const std::pair<const std::string, Constructor>& one_method : constructors_) {
    result.push_back(&(one_method.second));
  }

  return result;
}

bool MM::Reflection::Meta::HaveMethod(const std::string& method_name) const {
  return methods_.find(method_name) != methods_.end();
}

const MM::Reflection::Method* MM::Reflection::Meta::GetMethod(
    const std::string& method_name) const {
  if (const auto& find_result = methods_.find(method_name);
      find_result != methods_.end()) {
    return &(find_result->second);
  }

  return nullptr;
}

std::vector<const MM::Reflection::Method*> MM::Reflection::Meta::GetAllMethod() const {
  if (methods_.empty()) {
    return std::vector<const Method*>{};
  }

  std::vector<const Method*> result{};
  result.reserve(methods_.size());
  for (const std::pair<const std::string, Method>& one_method : methods_) {
    result.push_back(&(one_method.second));
  }

  return result;
}

bool MM::Reflection::Meta::HaveProperty(
    const std::string& property_name) const {
  return properties_.find(property_name) != properties_.end();
}

const MM::Reflection::Property* MM::Reflection::Meta::GetProperty(
    const std::string& property_name) const {
  if (const auto& find_result = properties_.find(property_name);
      find_result != properties_.end()) {
    return &(find_result->second);
  }

  return nullptr;
}

std::vector<const MM::Reflection::Property*> MM::Reflection::Meta::GetAllProperty() const {
  if (properties_.empty()) {
    return std::vector<const Property*>{};
  }

  std::vector<const Property*> result{};
  result.reserve(properties_.size());
  for (const std::pair<const std::string, Property>& one_properties :
       properties_) {
    result.push_back(&(one_properties.second));
  }

  return result;
}

bool MM::Reflection::Meta::AddConstructor(Constructor&& constuctor) {
  if (!constuctor.IsValid()) {
    return false;
  }

  constructors_[constuctor.GetConstructorName()] = std::move(constuctor);

  return true;
}

void MM::Reflection::Meta::RemoveConstructor(
    const std::string& constructor_name) {
  constructors_.erase(constructor_name);
}

bool MM::Reflection::Meta::AddMethod(Method&& method) {
  if (!method.IsValid()) {
    return false;
  }

  methods_[method.GetMethodName()] = std::move(method);

  return true;
}

void MM::Reflection::Meta::RemoveMethod(const std::string& method_name) {
  methods_.erase(method_name);
}

bool MM::Reflection::Meta::AddProperty(Property&& property) {
  if (!property.IsValid()) {
    return false;
  }

  if (HaveProperty(property.GetPropertyName())) {
    return false;
  }

  properties_.emplace(property.GetPropertyName(), std::move(property));

  return true;
}

void MM::Reflection::Meta::RemoveProperty(const std::string& property_name) {
  properties_.erase(property_name);
}

bool MM::Reflection::Meta::SetSerializerName(
    const std::string& serializer_name) {
  const std::unordered_map<std::string, SerializerBase*>& data_base = GetSerializerDatabase();
  const auto find_rseult = data_base.find(serializer_name);
  if (find_rseult == nullptr) {
    return false;
  }
  if (!find_rseult->second->Check(*this)) {
    return false;
  }

  serializer_name_ = serializer_name;

  return true;
}

void MM::Reflection::Meta::RemoveSerializerName() { serializer_name_.clear(); }

MM::Reflection::Variable MM::Reflection::Meta::CreateInstance(
    const std::string& constructor_name) const {
  const Constructor* constructor = GetConstructor(constructor_name);
  if (constructor == nullptr) {
    return Variable{};
  }

  return constructor->Invoke();
}

MM::Reflection::Variable MM::Reflection::Meta::CreateInstance(
    const std::string& constructor_name, Variable& arg1) const {
  const Constructor* constructor = GetConstructor(constructor_name);
  if (constructor == nullptr) {
    return Variable{};
  }

  return constructor->Invoke(arg1);
}

MM::Reflection::Variable MM::Reflection::Meta::CreateInstance(
    const std::string& constructor_name, Variable& arg1, Variable& arg2) const {
  const Constructor* constructor = GetConstructor(constructor_name);
  if (constructor == nullptr) {
    return Variable{};
  }

  return constructor->Invoke(arg1, arg2);
}

MM::Reflection::Variable MM::Reflection::Meta::CreateInstance(
    const std::string& constructor_name, Variable& arg1, Variable& arg2,
    Variable& arg3) const {
  const Constructor* constructor = GetConstructor(constructor_name);
  if (constructor == nullptr) {
    return Variable{};
  }

  return constructor->Invoke(arg1, arg2, arg3);
}

MM::Reflection::Variable MM::Reflection::Meta::CreateInstance(
    const std::string& constructor_name, Variable& arg1, Variable& arg2,
    Variable& arg3, Variable& arg4) const {
  const Constructor* constructor = GetConstructor(constructor_name);
  if (constructor == nullptr) {
    return Variable{};
  }

  return constructor->Invoke(arg1, arg2, arg3, arg4);
}

MM::Reflection::Variable MM::Reflection::Meta::CreateInstance(
    const std::string& constructor_name, Variable& arg1, Variable& arg2,
    Variable& arg3, Variable& arg4, Variable& arg5) const {
  const Constructor* constructor = GetConstructor(constructor_name);
  if (constructor == nullptr) {
    return Variable{};
  }

  return constructor->Invoke(arg1, arg2, arg3, arg4, arg5);
}

MM::Reflection::Variable MM::Reflection::Meta::CreateInstance(
    const std::string& constructor_name, Variable& arg1, Variable& arg2,
    Variable& arg3, Variable& arg4, Variable& arg5, Variable& arg6) const {
  const Constructor* constructor = GetConstructor(constructor_name);
  if (constructor == nullptr) {
    return Variable{};
  }

  return constructor->Invoke(arg1, arg2, arg3, arg4, arg5, arg6);
}

MM::Reflection::Variable MM::Reflection::Meta::CreateInstance(
    const std::string& constructor_name, std::vector<Variable*>& args) const {
  const Constructor* constructor = GetConstructor(constructor_name);
  if (constructor == nullptr) {
    return Variable{};
  }

  return constructor->Invoke(args);
}

const std::string& MM::Reflection::Meta::GetEmptyObjectMethodName() {
  static std::string empyt_object_method_name = "GetEmptyObject";

  return empyt_object_method_name;
}

bool MM::Reflection::Meta::HaveEmptyObject() const {
  return empty_variable_.IsValid();
}

MM::Reflection::Variable& MM::Reflection::Meta::GetEmptyVariable() const {
  return empty_variable_;
}

MM::Reflection::Variable& MM::Reflection::Meta::GetEmptyVariableRefrence()
    const {
  return empty_variable_refrence_;
}

MM::Reflection::Variable& MM::Reflection::Meta::GetEmptyVariableConstRefrence()
    const {
  return empty_variable_const_refrence_;
}

const std::string& MM::Reflection::Meta::GetSerializerName() const {
  return serializer_name_;
}

bool MM::Reflection::Meta::HaveSerializer() const {
  return !GetSerializerName().empty();
}
