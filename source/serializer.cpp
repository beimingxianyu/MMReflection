#include "serializer.h"

#include <iostream>

const std::string& MM::Reflection::SerializerBase::GetSerializerName() const {
  return SerializerBase::GetSerializerNameStatic();
}

std::uint32_t MM::Reflection::SerializerBase::GetVersion() const { return 1; }

const std::string& MM::Reflection::SerializerBase::GetSerializerNameStatic() {
  static std::string serializer_name{};
  return serializer_name;
}

void MM::Reflection::SerializerBase::WriteDescriptor(
    DataBuffer& data_buffer, const Variable& variable, const void* custom_data,
    const std::uint32_t custome_data_size) const {
  bool is_refrence = variable.IsPropertyVariable()
                         ? (variable.GetPropertyRealType()->IsReference() ||
                            variable.GetPropertyRealType()->IsPointer())
                         : variable.IsRefrenceVariable();

  const SerializerDescriptor descriptor{
      GetVersion(),
      static_cast<std::uint32_t>(variable.GetMeta()->GetTypeName().size() + 1),
      custome_data_size, is_refrence};
  data_buffer.AddData(&descriptor, sizeof(SerializerDescriptor));
  data_buffer.AddData(variable.GetType()->GetOriginalTypeName().c_str(),
                      descriptor.c_style_type_name_size_);
  if (custom_data != nullptr) {
    assert(custome_data_size != 0);
    data_buffer.AddData(custom_data, custome_data_size);
  }
}

void* MM::Reflection::SerializerBase::GetVariableValuePtr(Variable& variable) {
  if (bool is_pointer = variable.IsPropertyVariable()
                            ? variable.GetPropertyRealType()->IsPointer()
                            : variable.GetType()->IsPointer()) {
    return variable.GetValueCast<void*>();
  } else {
    return variable.GetValue();
  }
}

MM::Reflection::SerializerDescriptor
MM::Reflection::SerializerBase::ReadDescriptor(const DataBuffer& data_buffer) {
  SerializerDescriptor result{};
  data_buffer.ReadData(&result, sizeof(result));

  return result;
}

std::string MM::Reflection::SerializerBase::ReadTypeName(
    const DataBuffer& data_buffer, std::uint64_t size) {
  std::string result{};
  result.resize(size - 1);
  data_buffer.ReadData(result.data(), size);

  return result;
}

void MM::Reflection::SerializerBase::ReadCustomData(
    const DataBuffer& data_buffer, void* data_to, std::uint64_t size) {
  data_buffer.ReadData(data_to, size);
}

void MM::Reflection::SerializerBase::PreProcessDescriptor(
    const Meta& meta, Variable& invalid_variable_refrence,
    const DeserializerInfo& deserializer_info) {
  assert(meta.HaveSerializer() && meta.HaveEmptyObject());
  assert(!invalid_variable_refrence.IsValid());

  Variable& empty_variable = meta.GetEmptyVariable();
  Variable& empty_refrence_variable = meta.GetEmptyVariableRefrence();
  void* empty_object_vptr =
      *reinterpret_cast<void**>(empty_variable.GetWrapperBasePtr());
  void* empty_refrence_object_vptr = *reinterpret_cast<void**>(
      empty_refrence_variable.GetWrapperBasePtr());

  if (deserializer_info.is_refrence_) {
    if (deserializer_info.placement_address_) {
      if (deserializer_info.need_vptr_) {
        void* refrence_ptr = malloc(meta.GetType().GetSize());

        auto* wrapper_ptr = static_cast<Variable::WrapperObject*>(
            deserializer_info.placement_address_);
        wrapper_ptr->SetPtr1(empty_refrence_object_vptr);
        wrapper_ptr->SetPtr2(refrence_ptr);

        invalid_variable_refrence.wrapper_.placement_wrapper_ =
            static_cast<VariableWrapperBase*>(
                deserializer_info.placement_address_);
        invalid_variable_refrence.variable_type_ =
            Variable::VariableType::PLACMENT_OBJECT;
      } else {
        Variable::WrapperObject* wrapper_ptr =
            invalid_variable_refrence.GetWrapperObjectAddress();
        wrapper_ptr->SetPtr1(empty_refrence_object_vptr);
        wrapper_ptr->SetPtr2(deserializer_info.placement_address_);

        invalid_variable_refrence.variable_type_ =
            Variable::VariableType::SMALL_OBJECT;
      }
    } else {
      void* refrence_ptr = malloc(meta.GetType().GetSize());

      Variable::WrapperObject* wrapper_ptr =
          invalid_variable_refrence.GetWrapperObjectAddress();
      wrapper_ptr->SetPtr1(empty_refrence_object_vptr);
      wrapper_ptr->SetPtr2(refrence_ptr);

      invalid_variable_refrence.variable_type_ =
          Variable::VariableType::SMALL_OBJECT;
    }
  } else {
    if (deserializer_info.placement_address_) {
      if (deserializer_info.need_vptr_) {
        *static_cast<void**>(deserializer_info.placement_address_) =
            empty_object_vptr;

        invalid_variable_refrence.wrapper_.placement_wrapper_ =
            static_cast<VariableWrapperBase*>(
                deserializer_info.placement_address_);
        invalid_variable_refrence.variable_type_ =
            Variable::VariableType::PLACMENT_OBJECT;
      }
    } else {
      // use small object optimize
      std::uint64_t type_size = meta.GetType().GetSize();
      if (type_size <= sizeof(void*)) {
        invalid_variable_refrence.wrapper_.small_wrapper_.ptr1 = empty_object_vptr;
        invalid_variable_refrence.variable_type_ = Variable::VariableType::COMMON_OBJECT;
      } else {
        VariableWrapperBase* wrapper_ptr = static_cast<VariableWrapperBase*>(
            malloc(GetVariableWrpperSize(type_size)));
        *reinterpret_cast<void**>(wrapper_ptr) = empty_object_vptr;
        invalid_variable_refrence.wrapper_.common_wrapper_ = wrapper_ptr;
        invalid_variable_refrence.variable_type_ = Variable::VariableType::COMMON_OBJECT;
      }
    }
  }

  if (!invalid_variable_refrence.IsValid()) {
    std::cerr << "[Error] [MMReflection] Error descriptor of is_refrence:"
              << std::boolalpha << deserializer_info.is_refrence_
              << " | need_vptr_:" << std::boolalpha
              << deserializer_info.need_vptr_
              << " | placement_address:" << std::hex
              << deserializer_info.placement_address_ << '\n';
    assert(invalid_variable_refrence.IsValid());
  }
}

bool MM::Reflection::TrivialSerializer::Check(const Meta& meta) const {
  if (!meta.HaveEmptyObject()) {
    std::cerr << "[Error] [MMReflection] The type named " << meta.GetTypeName()
              << " is not registered empty object to metadata,so the " << GetSerializerNameStatic()
              << " serializer cannot be used.\n";
    return false;
  }
  if (!meta.GetType().IsTrivial()) {
    std::cerr << "[Error] [MMReflection] The type named " << meta.GetTypeName()
              << " is not a trivial type, so the " << GetSerializerNameStatic()
              << " serializer cannot be used.\n";
    return false;
  }

  return true;
}

MM::Reflection::DataBuffer& MM::Reflection::TrivialSerializer::Serialize(
    DataBuffer& data_buffer, Variable& variable) const {
  assert(GetSerializerName() == variable.GetMeta()->GetSerializerName());

  WriteDescriptor(data_buffer, variable, nullptr, 0);

  data_buffer.AddData(GetVariableValuePtr(variable),
                      variable.GetMeta()->GetType().GetSize());

  return data_buffer;
}

MM::Reflection::Variable MM::Reflection::TrivialSerializer::Deserialize(
    const DataBuffer& data_buffer, const Meta& meta, const DeserializerInfo& deserializer_info) const {
  assert(GetSerializerName() == meta.GetSerializerName());

  Variable variable{};
  PreProcessDescriptor(meta, variable, deserializer_info);

  data_buffer.ReadData(variable.GetValue(), meta.GetType().GetSize());

  return variable;
}

const std::string& MM::Reflection::TrivialSerializer::GetSerializerName()
    const {
  return TrivialSerializer::GetSerializerNameStatic();
}

const std::string&
MM::Reflection::TrivialSerializer::GetSerializerNameStatic() {
  static std::string serializer_name{"Trivial"};
  return serializer_name;
}

bool MM::Reflection::UnsefeRecursionSerializer::Check(const Meta& meta) const {
  if (!meta.HaveEmptyObject()) {
    std::cerr << "[Error] [MMReflection] The type named " << meta.GetTypeName()
              << " is not registered empty object to metadata,so the "
              << GetSerializerNameStatic() << " serializer cannot be used.\n";
    return false;
  }

  for (const auto* property_ptr : meta.GetAllProperty()) {
    assert(property_ptr != nullptr);
    if (property_ptr->IsStatic()) {
      continue;
    }

    const Meta* property_meta = property_ptr->GetMeta();
    if (property_meta == nullptr) {
      std::cerr << "[Error] [MMReflection] The property "
                << property_ptr->GetPropertyName() << " of type "
                << meta.GetTypeName() << " is not registered,so the "
                << GetSerializerNameStatic() << " serializer cannot be used.\n";

      return false;
    }

    if (!property_meta->HaveSerializer()) {
      std::cerr << "[Error] [MMReflection] The property "
                << property_ptr->GetPropertyName() << "(property type is "
                << property_meta->GetTypeName() << ")"
                << " of type " << meta.GetTypeName()
                << " is not registered serializer to metadata,so the "
                << GetSerializerNameStatic() << " serializer cannot be used.\n";
      return false;
    }
  }

  return true;
}

MM::Reflection::DataBuffer& MM::Reflection::UnsefeRecursionSerializer::Serialize(
    DataBuffer& data_buffer, Variable& variable) const {
  assert(GetSerializerName() == variable.GetMeta()->GetSerializerName());

  WriteDescriptor(data_buffer, variable, nullptr, 0);

  auto& serializer_database = GetSerializerDatabase();
  for (const auto* property : variable.GetMeta()->GetAllProperty()) {
    if (property->IsStatic()) {
      continue;
    }
    Variable property_variable =
        variable.GetPropertyVariable(property->GetPropertyName());
    assert(property_variable.GetMeta()->HaveSerializer());
    auto serializer_iter = serializer_database.find(
        property_variable.GetMeta()->GetSerializerName());
    assert(serializer_iter != nullptr);
    const SerializerBase* serializer = serializer_iter->second;
    serializer->Serialize(data_buffer, property_variable);
  }

  return data_buffer;
}

MM::Reflection::Variable MM::Reflection::UnsefeRecursionSerializer::Deserialize(
    const DataBuffer& data_buffer, const Meta& meta,
    const DeserializerInfo& deserializer_info) const {
  assert(GetSerializerName() == meta.GetSerializerName());

  Variable variable{};
  PreProcessDescriptor(meta, variable, deserializer_info);

  memcpy(variable.GetValue(), meta.GetEmptyVariable().GetValue(), meta.GetType().GetSize());

  ReadAllPropertyData(data_buffer, meta, variable);

  return variable;
}

const std::string& MM::Reflection::UnsefeRecursionSerializer::GetSerializerName()
    const {
  return GetSerializerNameStatic();
}

const std::string&
MM::Reflection::UnsefeRecursionSerializer::GetSerializerNameStatic() {
  static std::string serializer_name{"UnsafeRecursion"};

  return serializer_name;
}

void MM::Reflection::UnsefeRecursionSerializer::ReadAllPropertyData(
    const DataBuffer& data_buffer, const Meta& meta, Variable& variable) {
  for (const Property* property : meta.GetAllProperty()) {
    if (property->IsStatic()) {
      continue;
    }

    const SerializerDescriptor serializer_descriptor =
        ReadDescriptor(data_buffer);
    const std::string type_name = ReadTypeName(
        data_buffer, serializer_descriptor.c_style_type_name_size_);
    auto type_hash_code_iter = GetNameToTypeHashDatabase().find(type_name);
    assert(type_hash_code_iter != nullptr);
    auto meta_iter = GetMetaDatabase().find(type_hash_code_iter->second);
    assert(meta_iter != nullptr);
    const Meta* property_meta = meta_iter->second;
    assert(property_meta->HaveSerializer());
    auto serializer_iter =
        GetSerializerDatabase().find(property_meta->GetSerializerName());
    assert(serializer_iter != nullptr);
    const SerializerBase* serializer = serializer_iter->second;

    assert(serializer_descriptor.is_refrence_ ==
           property->GetType()->IsReference());
    DeserializerInfo property_deserializer_info{
        static_cast<char*>(variable.GetValue()) + property->GetPropertyOffset(),
        serializer_descriptor.is_refrence_, false};
    Variable property_variable =
        variable.GetPropertyVariable(property->GetPropertyName());
    // discard return value
    serializer->Deserialize(data_buffer, *property_meta,
                            property_deserializer_info);
  }
}

bool MM::Reflection::RecursionSerializer::Check(const Meta& meta) const {
  if (UnsefeRecursionSerializer::Check(meta)) {
    return false;
  }

  for (const auto* property_ptr : meta.GetAllProperty()) {
    assert(property_ptr != nullptr);
    if (property_ptr->IsStatic()) {
      continue;
    }

    if (property_ptr->GetType()->IsReference()) {
      std::cerr << "[Error] [MMReflection] The property "
                << property_ptr->GetPropertyName() << " of type "
                << meta.GetTypeName() << " is a refrence property,so the "
                << GetSerializerNameStatic() << " serializer cannot be used.\n";

      return false;
    }
  }

  return true;
}

const std::string& MM::Reflection::RecursionSerializer::GetSerializerName()
    const {
  return GetSerializerNameStatic();
}

const std::string&
MM::Reflection::RecursionSerializer::GetSerializerNameStatic() {
  static std::string serializer_name{"UnsafeRecursion"};

  return serializer_name;
}

MM::Reflection::DataBuffer& MM::Reflection::Serialize(DataBuffer& data_buffer,
                                                      Variable& variable) {
  const Meta* meta = variable.GetMeta();
  assert(meta != nullptr);
  assert(meta->HaveSerializer());

  const auto serializer_iter =
      GetSerializerDatabase().find(meta->GetSerializerName());
  assert(serializer_iter != nullptr);
  const SerializerBase* serializer = serializer_iter->second;
  return serializer->Serialize(data_buffer, variable);
}

MM::Reflection::Variable MM::Reflection::Deserialize(
    const DataBuffer& data_buffer) {
  const SerializerDescriptor serializer_descriptor =
      SerializerBase::ReadDescriptor(data_buffer);
  const std::string type_name = SerializerBase::ReadTypeName(
      data_buffer, serializer_descriptor.c_style_type_name_size_);
  auto type_hash_code_iter = GetNameToTypeHashDatabase().find(type_name);
  assert(type_hash_code_iter != nullptr);
  auto meta_iter = GetMetaDatabase().find(type_hash_code_iter->second);
  assert(meta_iter != nullptr);
  const Meta* meta = meta_iter->second;
  assert(meta->HaveSerializer());
  auto serializer_iter =
      GetSerializerDatabase().find(meta->GetSerializerName());
  assert(serializer_iter != nullptr);
  const SerializerBase* serializer = serializer_iter->second;

  DeserializerInfo deserializer_info{nullptr,
                                     serializer_descriptor.is_refrence_, false};
  return serializer->Deserialize(data_buffer, *meta, deserializer_info);
}
