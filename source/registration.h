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
ConstructorClassType ConstructorFunction(Args ...args) {
  return ConstructorClassType{((Args)args)...};
}

template<typename ConstructorClassType, typename ...Args>
void PlacementConstructorFunction(void* placement_address, Args ...args) {
  new (placement_address)VariableWrapper<ConstructorClassType>{((Args)args)...};
}

template <typename ClassType_>
class Class {
public:
  Class() = delete;
  ~Class() {
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
      assert(add_result);
    } else {
      static_assert(std::is_same_v<typename PropertyMeta::ClassType, ClassType_>, "This property does not belong to this class.");
      const intptr_t property_offset = (intptr_t)(&((ClassType_*)(nullptr)->*property_ptr));
      const bool add_result = meta_.AddProperty(Property::CreateProperty<typename PropertyMeta::ClassType,
                                   typename PropertyMeta::PropertyType>(property_name, property_offset, sizeof(typename PropertyMeta::PropertyType)));
      assert(add_result);
    }

    return *this;
  }

  template<typename ...Args>
  Class& Constructor(const std::string& constructor_name) {
    const bool add_result = meta_.AddConstructor(Constructor::CreateConstructor<ClassType_, Args...>(constructor_name, &ConstructorFunction<ClassType_, Args...>, &PlacementConstructorFunction<ClassType_, Args...>));
    assert(add_result);

    return *this;
  }

private:
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
      assert(meta_find_result != nullptr);
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

      if (meta_.serializer_name_ != old_meta->serializer_name_) {
        old_meta->serializer_name_ = meta_.serializer_name_;
      }

      return;
    }

    // Register a new type.
    auto name_to_hash_emplace_result = name_to_type_database.emplace(std::pair{meta_.GetTypeName(), meta_.GetType().GetTypeHashCode()});
    assert(name_to_hash_emplace_result.second);
    auto meta_data_emplace_result = meta_database.emplace(std::pair{meta_.GetType().GetTypeHashCode(), new Meta{std::move(meta_)}});
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

private:
  Meta meta_;
};
}  // namespace Reflection
}  // namespace MM

