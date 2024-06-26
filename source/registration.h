#pragma once

#include <cassert>
#include <iostream>
#include <filesystem>

#include "constructor.h"
#include "method.h"
#include "property.h"
#include "meta.h"
#include "type_utils.h"
#include "marco.h"

static void MMAutoRegistionFunction();

#define MM_REGISTER                                                        \
  namespace {                                                              \
  struct MMAutoRegistionStruct {                                           \
    MMAutoRegistionStruct() { MMAutoRegistionFunction(); }                 \
  };                                                                       \
  }                                                                        \
  static const MMAutoRegistionStruct MM_CAT(mm_auto_register__, __LINE__); \
  static void MMAutoRegistionFunction()

#define MM_GENERATE_REFLECTION_BODY() \
  friend void ::MMAutoRegistionFunction();

namespace MM {
namespace Reflection {
template<std::uint64_t RegisterID>
struct MMAutoRegisterStruct;

template<typename ConstructorClassType, typename ...Args>
ConstructorClassType ConstructorFunction(Utils::GetFunctionArgTypeT<Args> ...args) {
  return ConstructorClassType{((Args)args)...};
}

template<typename ConstructorClassType, typename ...Args>
void PlacementConstructorFunction(void* placement_address, Utils::GetFunctionArgTypeT<Args> ...args) {
  new (placement_address)VariableWrapper<ConstructorClassType>{((Args)args)...};
}

template <typename ClassType_>
class Class {
public:
  Class() = delete;
  virtual ~Class() {
    Register();
  }
  Class(const Class& other) = delete;
  Class(Class&& other) noexcept = delete;
  Class& operator=(const Class& other) = delete;
  Class& operator=(Class& other) noexcept = delete;

public:
  explicit Class(const std::string& type_name)
    : meta_(type_name, Type::CreateType<ClassType_>()) {
    static_assert((std::is_void_v<ClassType_>) || (std::is_same_v<ClassType_, Utils::GetOriginalTypeT<ClassType_>>), "");
  }

  template<typename F_>
  Class& Method(const std::string& method_name, F_ method_ptr) {
    using SignatureType = Utils::FunctionSignature<F_>;
    static_assert(
        (!SignatureType::IsStatic) ||
            (std::is_same_v<ClassType_, Utils::GetOriginalTypeT<ClassType_>>),
        "This method does not belong to this class.");
    if constexpr (SignatureType::IsStatic) {
      CreateStaticMethodHelp(method_name, method_ptr);
    } else {
      if constexpr (SignatureType::IsConst) {
        CreateConstMethodHelp(method_name, method_ptr);
      } else {
        CreateMethodHelp(method_name, method_ptr);
      }
    }

    return *this;
  }

  template<typename PropertySignature>
  Class& Property(const std::string& property_name, PropertySignature property_ptr) {
    using PropertyMeta = Utils::PropertyMetaData<PropertySignature>;
    if constexpr (PropertyMeta::IsStatic) {
      const bool add_result = meta_.AddProperty(Property::CreateStaticProperty<ClassType_, typename PropertyMeta::PropertyType>(property_name, property_ptr, sizeof(typename PropertyMeta::PropertyType)));
      if (!add_result) {
        std::cerr << "[Error] [MMReflecion] Class " << meta_.GetTypeName() << " registers property " << property_name << " repeatedly.";
        abort();
      }
    } else {
      static_assert(std::is_same_v<typename PropertyMeta::ClassType, ClassType_>, "This property does not belong to this class.");
      const intptr_t property_offset = (intptr_t)(&((ClassType_*)(nullptr)->*property_ptr));
      const bool add_result = meta_.AddProperty(Property::CreateProperty<typename PropertyMeta::ClassType,
                                   typename PropertyMeta::PropertyType>(property_name, property_offset, sizeof(typename PropertyMeta::PropertyType)));
      if (!add_result) {
        std::cerr << "[Error] [MMReflecion] Class " << meta_.GetTypeName() << " registers property " << property_name << " repeatedly.";
        abort();
      }
    }

    return *this;
  }

  template<typename PropertyType>
  Class& Property(const std::string& property_name, RefrencePropertyDescriptor<PropertyType> descriptor ) {
    const bool add_result = meta_.AddProperty(Property::CreateRefrenceProperty<ClassType_, PropertyType>(property_name, descriptor.offset_));
    if (!add_result) {
      std::cerr << "[Error] [MMReflecion] Class " << meta_.GetTypeName() << " registers property " << property_name << " repeatedly.";
      abort();
    }

    return *this;
  }

  template<typename PropertyType>
  Class& Property(const std::string& property_name, StaticRefrencePropertyDescriptor<PropertyType> descriptor ) {
    const bool add_result = meta_.AddProperty(Property::CreateStaticRefrenceProperty<ClassType_, PropertyType>(property_name, descriptor.address_));
    if (!add_result) {
      std::cerr << "[Error] [MMReflecion] Class " << meta_.GetTypeName() << " registers property " << property_name << " repeatedly.";
      abort();
    }

    return *this;
  }

  template<typename ...Args>
  Class& Constructor(const std::string& constructor_name) {
    const bool add_result = meta_.AddConstructor(
        Constructor::CreateConstructor<ClassType_, Args...>(
            constructor_name,
            &ConstructorFunction<ClassType_, Args...>,
            &PlacementConstructorFunction<ClassType_, Args...>));
    if (!add_result) {
      std::cerr << "[Error] [MMReflecion] Class " << meta_.GetTypeName() << " registers constructor " << constructor_name << " repeatedly.";
      abort();
    }

    return *this;
  }

  Class& SetSerializerName(const std::string& serializer_name) {
    serializer_name_ = serializer_name;

    return *this;
  }

protected:
  static void SetEmptyObject(Meta& meta) {
      if constexpr (!std::is_same_v<ClassType_, void>) {
        if (!meta.empty_variable_.IsValid()) {
          assert(!meta.empty_variable_refrence_.IsValid() && !meta.empty_variable_const_refrence_.IsValid());

          const Reflection::Method* get_empty_object_method = meta.GetMethod(meta.GetEmptyObjectMethodName());
          if (get_empty_object_method != nullptr &&
              get_empty_object_method->IsValid() &&
              get_empty_object_method->GetArgumentNumber() == 0 &&
              get_empty_object_method->IsStatic() &&
              meta.GetType().GetTypeHashCode() ==
                  get_empty_object_method->GetReturnType()->GetTypeHashCode()) {
            static Variable empty_variable = Variable::CreateVoidVariable();
            meta.empty_variable_ = get_empty_object_method->Invoke(empty_variable);
            meta.empty_variable_refrence_ = Variable::CreateVariable<ClassType_&>(meta.empty_variable_.GetValueCast<ClassType_>());
            meta.empty_variable_const_refrence_ = Variable::CreateVariable<const ClassType_&>(meta.empty_variable_.GetValueCast<ClassType_>());
          }
        }
      }
  }

  void Register() {
    auto& name_to_type_database = GetNameToTypeHashDatabase();
    auto& meta_database = GetMetaDatabase();

    // TODO Write unit tests for this section.
    // Incremental registration.
    if (auto find_result = name_to_type_database.find(meta_.GetTypeName()); find_result != name_to_type_database.end()) {
      if (find_result->second != meta_.GetType().GetTypeHashCode()) {
        std::cerr << "[Error] [MMReflecion] Different types use the same reflection type name, which is not allowed.(same reflection name:" << meta_.GetTypeName() << ")." << std::endl;
        abort();
      }

      auto meta_find_result = meta_database.find(find_result->second);
      assert(meta_find_result != meta_database.end());
      Meta* old_meta = meta_find_result->second;

      auto& old_meta_constructors = old_meta->constructors_;
      for (auto& construtor: meta_.constructors_) {
        if (old_meta_constructors.count(construtor.first) == 0) {
          old_meta_constructors.emplace(construtor.first, std::move(construtor.second));
        }
      }
      auto& old_meta_methods = old_meta->methods_;
      for (auto& method: meta_.methods_) {
        if (old_meta_methods.count(method.first) == 0) {
          old_meta_methods.emplace(method.first, std::move(method.second));
        }
      }
      auto& old_meta_properties = old_meta->properties_;
      for (auto& property: meta_.properties_) {
        if (old_meta_properties.count(property.first) == 0) {
          old_meta_properties.emplace(property.first, std::move(property.second));
        }
      }

      SetEmptyObject(*old_meta);
      if (old_meta->GetSerializerName() != serializer_name_) {
        old_meta->SetSerializerName(serializer_name_);
      }

      return;
    }

    // Register a new type.
    SetEmptyObject(meta_);
    meta_.SetSerializerName(serializer_name_);

    auto name_to_hash_emplace_result = name_to_type_database.emplace(std::pair{meta_.GetTypeName(), meta_.GetType().GetTypeHashCode()});
    assert(name_to_hash_emplace_result.second);
    auto type_hash_code = meta_.GetType().GetTypeHashCode();
    auto meta_data_emplace_result = meta_database.emplace(std::pair{type_hash_code, new Meta{std::move(meta_)}});
    assert(meta_data_emplace_result.second);
  }

private:

  template<typename T>
  struct TypePack {
    using Type = T;
  };

  template<typename ReturnType, typename ClassType, typename ...Args>
  void CreateMethodHelp(const std::string& method_name, ReturnType(ClassType::*method_ptr)(Args...)) {
    const bool add_result = meta_.AddMethod(Method::CreateMethod<ReturnType, ClassType_, false, false, false,
                                             Args...>(method_name, method_ptr));
    assert(add_result);
  }

  template<typename ReturnType, typename ClassType, typename ...Args>
  void CreateConstMethodHelp(const std::string& method_name, ReturnType(ClassType::*method_ptr)(Args...) const) {
    const bool add_result = meta_.AddMethod(Method::CreateMethod<ReturnType, ClassType_, false, true, false,
                                             Args...>(method_name, method_ptr));
    assert(add_result);
  }

  template<typename ReturnType, typename ...Args>
  void CreateStaticMethodHelp(const std::string& method_name, ReturnType(*method_ptr)(Args...)) {
    const bool add_result = meta_.AddMethod(Method::CreateMethod<ReturnType, ClassType_, true, false, false,
                                             Args...>(method_name, method_ptr));
    assert(add_result);
  }

public:
  Meta meta_;

  std::string serializer_name_;
};

template <typename EnumType, EnumValue DefaultValue = 0>
class Enum : private Class<EnumType> {
public:
  Enum() = delete;
  ~Enum() override {
    EnumRegister();
  }
  explicit Enum(const std::string& type_name)
    : Class<EnumType>(type_name) {
    static_assert((std::is_enum_v<EnumType>) && (std::is_same_v<EnumType, Utils::GetOriginalTypeT<EnumType>>), "");
  }
  Enum(const Enum& other) = delete;
  Enum(Enum&& other) noexcept = delete;
  Enum& operator=(const Enum& other) = delete;
  Enum& operator=(Enum& other) noexcept = delete;

public:
  Enum& Add(const std::string& enum_name, EnumType value) {
    const bool add_result = Class<EnumType>::meta_.AddEnum(EnumPair{enum_name, static_cast<EnumValue>(value)});
    if (!add_result) {
      std::cerr << "[Error] [MMReflecion] Enum " << Class<EnumType>::meta_.GetTypeName() << " registers enum " << enum_name << " repeatedly.";
      abort();
    }

    return *this;
  }

  Enum& SetSerializerName(const std::string& serializer_name) {
    Class<EnumType>::serializer_name_ = serializer_name;

    return *this;
  }

private:
  static EnumType GetEmptyObject() {
    return static_cast<EnumType>(DefaultValue);
  }

  static EnumType ConstructorFunction(EnumValue value) {
    return EnumType{static_cast<EnumType>(value)};
  }

  static void PlacementConstructorFunction(void* address, EnumValue value) {
    new (address) EnumType{static_cast<EnumType>(value)};
  }

  void EnumRegister() {
    if (Class<EnumType>::meta_.enums_.empty()) {
      std::cerr << "[Error] [MMReflection] Enum " << Class<EnumType>::meta_.GetTypeName() << "does not contain any enumeration.";
      abort();
    }

    ::MM::Reflection::Constructor constructor = ::MM::Reflection::Constructor::CreateConstructor<EnumType, EnumValue>(Meta::GetEnumConstructName(), ConstructorFunction, PlacementConstructorFunction);
    Class<EnumType>::meta_.AddConstructor(std::move(constructor));
    Class<EnumType>::Method(Class<EnumType>::meta_.GetEmptyObjectMethodName(), &GetEmptyObject);
  }
};

template<typename SerializerType>
void RegisterSerializer() {
  static_assert(std::is_base_of_v<SerializerBase, SerializerType>, "The registered serializer must inherit from MM::Reflection::SerializerBase.");

  auto& serializer_database = GetSerializerDatabase();
  SerializerType* new_serializer = new SerializerType{};
  if (serializer_database.count(new_serializer->GetSerializerName()) != 0) {
    return;
  }

  serializer_database.emplace(new_serializer->GetSerializerName(), new_serializer);
}
}  // namespace Reflection
}  // namespace MM

