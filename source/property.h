#pragma once

#include <cassert>
#include <cstddef>
#include <memory>

#include "type.h"
#include "variable.h"

namespace MM {
namespace Reflection {
class PropertyWrapperBase {
  friend class Property;

 public:
  PropertyWrapperBase(const std::string& property_name, std::uint64_t offset, std::uint64_t size) : property_name_(property_name), offset_(offset), size_(size) {}
  virtual ~PropertyWrapperBase() = default;
  PropertyWrapperBase(const PropertyWrapperBase& other) = delete;
  PropertyWrapperBase(PropertyWrapperBase&& other) = delete; 
  PropertyWrapperBase& operator=(const PropertyWrapperBase& other) = delete;
  PropertyWrapperBase& operator=(PropertyWrapperBase&&) = delete;

 public:
  const std::string& GetPropertyName() const {return property_name_;}

  std::uint64_t GetPropertyOffset() const {return offset_;}

  std::uint64_t GetPropertySize() const {return size_;}

  virtual const void* GetStaticPropertyAddress() const {return nullptr;}

  virtual void* GetStaticPropertyAddress() {return nullptr;}

  virtual bool IsValid() const {
    return size_ != 0;
  }

  virtual bool IsStatic() const {return false;}

  /**
   * \brief Get the hashcode of this class.
   * \return The hashcode of this class.
   * \remark The value of hashcode depends on the type of the property, the type
   * of the class to which the property belongs, and the name of the property.
   */
  virtual std::size_t HashCode() const {return 0;}

  /**
   * \brief Get the MM::Reflection::Type of the class that holds the property.
   * \return The MM::Reflection::Type of the object held by this object.
   */
  virtual const MM::Reflection::Type* GetType() const {return nullptr;}

  /**
   * \brief Get the MM::Reflection::Type of the class held by this property.
   * \return The MM::Reflection::TypeWrapper of the class held by this property.
   */
  virtual const MM::Reflection::Type* GetClassType() const {return nullptr;}

  /**
   * \brief Get meta data.
   * \return Returns the metadata of the property represented by this object.
   * \remark If the type is not registered, the nullptr will be returned.
   */
  virtual const Meta* GetMeta() const {return nullptr;}

  /**
   * \brief Get class meta data.
   * \return Returns the metadata of the class that holds the property.
   * \remark If the type is not registered, the containing nullptr will be returned.
   */
  virtual const Meta* GetClassMeta() const {return nullptr;}

 private:
  /**
   * \brief Gets the variable that the attribute refers to.
   * \param class_variable The class variable of this property.
   * \return A MM::Reflection::Variable that holds the this property contained
   * in \ref class_variable.
   */
  virtual Variable GetPropertyVariable(Variable& class_variable) {return Variable{};}

  /**
   * \brief Gets the variable that the attribute refers to.
   * \param class_variable The class variable of this property.
   * \return A MM::Reflection::Variable that holds the this property contained
   * in \ref class_variable.
   */
  virtual Variable GetPropertyVariable(const Variable& class_variable) const {return Variable{};}

  private:
    std::string property_name_{};
    std::uint64_t offset_{0};
    std::uint64_t size_{0};
};

template <typename ClassType_, typename PropertyType_>
class CommonPropertyWrapper
    : public PropertyWrapperBase {
  friend class Property;

 public:
  using Type = PropertyType_;
  using OriginalType = typename Utils::GetOriginalType<PropertyType_>::Type;
  using ClassType = ClassType_;

 public:
  CommonPropertyWrapper(const std::string& property_name, std::uint64_t offset, std::uint64_t size) : PropertyWrapperBase(property_name, offset, size) {}
  ~CommonPropertyWrapper() override = default;
  CommonPropertyWrapper(const CommonPropertyWrapper& other) = delete;
  CommonPropertyWrapper(CommonPropertyWrapper&& other) noexcept = delete;
  CommonPropertyWrapper& operator=(const CommonPropertyWrapper& other) = delete;
  CommonPropertyWrapper& operator=(CommonPropertyWrapper&& other) noexcept = delete;

 public:
  /**
   * \brief Get the hashcode of this class.
   * \return The hashcode of this class.
   * \remark The value of hashcode depends on the type of the property, the type
   * of the class to which the property belongs, and the name of the property.
   */
  std::uint64_t HashCode() const override {
    return Utils::AllTypeHashCode<PropertyType_, ClassType_>::HashCode() ^ std::hash<std::string>{}(GetPropertyName());
  }

  /**
   * \brief Get the MM::Reflection::Type of the object held by this property.
   * \return The MM::Reflection::TypeWrapper of the object held by this
   * property.
   */
  const MM::Reflection::Type* GetType() const override {
    const MM::Reflection::Type& type = MM::Reflection::Type::CreateType<PropertyType_>();
    return &type;
  }

  /**
   * \brief Get the MM::Reflection::Type of the class held by this property.
   * \return The MM::Reflection::TypeWrapper of the class held by this property.
   */
  const MM::Reflection::Type* GetClassType() const override {
    const MM::Reflection::Type& type = MM::Reflection::Type::CreateType<ClassType_>();
    return &type;
  }

  /**
   * \brief Get meta data.
   * \return Returns the metadata of the property represented by this object.
   * \remark If the type is not registered, the nullptr will be returned.
   */
  const Meta* GetMeta() const override {
    const MM::Reflection::Type* PropertyType = GetType();
    assert(PropertyType != nullptr);
    return PropertyType->GetMate();
  }

  /**
   * \brief Get class meta data.
   * \return Returns the metadata of the class that holds the property.
   * \remark If the type is not registered, the nullptr will be returned.
   */
  const Meta* GetClassMeta() const override {
    const MM::Reflection::Type* ClassType = GetClassType();
    assert(ClassType != nullptr);
    return ClassType->GetMate();
  }
};

template <typename ClassType_, typename PropertyType_>
class PropertyWrapper
    : public CommonPropertyWrapper<ClassType_, PropertyType_> {

  public:
   PropertyWrapper(const std::string& property_name, std::uint64_t offset,
                   std::uint64_t size)
       : CommonPropertyWrapper<ClassType_, PropertyType_>(property_name, offset,
                                                          size) {}

  public:
    bool IsStatic() const override {
      return false;
    }

 private:
  /**
   * \brief Gets the variable that the attribute refers to.
   * \param class_variable The class variable of this property.
   * \return A MM::Reflection::Variable that holds the this property contained
   * in \ref class_variable.
   */
  Variable GetPropertyVariable(Variable& class_variable) override {
    PropertyType_* property_ptr =reinterpret_cast<PropertyType_*>(static_cast<char*>(class_variable.GetValue()) + PropertyWrapperBase::GetPropertyOffset());
    return Variable{new PropertyVariableWrapper<PropertyType_>(*property_ptr)};
  }

  /**
   * \brief Gets the variable that the attribute refers to.
   * \param class_variable The class variable of this property.
   * \return A MM::Reflection::Variable that holds the this property contained
   * in \ref class_variable.
   */
  Variable GetPropertyVariable(const Variable& class_variable) const override {
    const PropertyType_* property_ptr =reinterpret_cast<const PropertyType_*>(static_cast<const char*>(class_variable.GetValue()) + PropertyWrapperBase::GetPropertyOffset());
    return Variable{new PropertyVariableWrapper<const PropertyType_>(*property_ptr)};
  }
};

template <typename ClassType_, typename PropertyType_>
class StaticPropertyWrapper
    : public CommonPropertyWrapper<ClassType_, PropertyType_> {
  public:
    using StaticPropertyType = PropertyType_ *;

  public:
   StaticPropertyWrapper(const std::string& property_name,
                         StaticPropertyType address, std::uint64_t size)
       : CommonPropertyWrapper<ClassType_, PropertyType_>(property_name, 0,
                                                          size),
         static_property_address_(address) {}
   ~StaticPropertyWrapper() override = default;

  public:
    bool IsValid() const override {
      return PropertyWrapperBase::IsValid() && static_property_address_ != nullptr;
    }

    bool IsStatic() const override {
      return true;
    }

    const void* GetStaticPropertyAddress() const override { 
      return static_property_address_;
    }

    void* GetStaticPropertyAddress() override { 
      return static_property_address_;
    }

 private:
  /**
   * \brief Gets the variable that the attribute refers to.
   * \return A MM::Reflection::Variable that holds the this property contained
   * in \ref class_variable.
   */
  Variable GetPropertyVariable(Variable&) override {
    return Variable{new PropertyVariableWrapper<PropertyType_>(*static_property_address_)};
  }

  /**
   * \brief Gets the variable that the attribute refers to.
   * \return A MM::Reflection::Variable that holds the this property contained
   * in \ref class_variable.
   */
  Variable GetPropertyVariable(const Variable&) const override {
    return Variable{new PropertyVariableWrapper<const PropertyType_>(*static_property_address_)};
  }

  private:
    StaticPropertyType static_property_address_{nullptr};
};


class Property {
  friend class Meta;
  friend class Variable;

 public:
  Property() = default;
  ~Property() = default;
  Property(const Property& other) = delete;
  Property(Property&& other) noexcept = default;
  Property& operator=(const Property& other) = delete;
  Property& operator=(Property&& other) noexcept = default;

 public:
  template<typename ClassType_, typename PropertyType_>
  static Property CreateProperty(const std::string& property_name, std::uint64_t offset, std::uint64_t size) {
    return Property{std::make_unique<PropertyWrapper<ClassType_, PropertyType_>>(property_name, offset, size)};
  }

  template<typename ClassType_, typename PropertyType_>
  static Property CreateStaticProperty(const std::string& property_name, PropertyType_* static_property_address, std::uint64_t size) {
    // using StaticPropertyType = PropertyType_ ClassType_::*;
    return Property{std::make_unique<StaticPropertyWrapper<ClassType_, PropertyType_>>(property_name, static_property_address, size)};
  }

 public:
  std::size_t GetPropertySize() const;

  const std::string& GetPropertyName() const;

  std::size_t GetPropertyOffset() const;

  void* GetStaticPropertyAddress() const;

  bool IsStatic() const;

  /**
   * \brief Get the hashcode of this class.
   * \return The hashcode of this class.
   * \remark The value of hashcode depends on the type of the property, the type
   * of the class to which the property belongs, and the name of the property.
   */
  std::size_t HashCode() const;

  /**
   * \brief Judge whether the object is a valid object.
   * \return Returns true if the object is a valid object, otherwise returns
   * false.
   */
  bool IsValid() const;

  /**
   * \brief Get the MM::Reflection::Type of the object held by this object.
   * \return The MM::Reflection::TypeWrapper of the object held by this object.
   * \remark Return nullptr if this object is invalid.
   */
  const MM::Reflection::Type* GetType() const;

  /**
   * \brief Get the MM::Reflection::Type of the class held by this property.
   * \return The MM::Reflection::TypeWrapper of the class held by this property.
   * \remark Return nullptr if this object is invalid.
   */
  const MM::Reflection::Type* GetClassType() const;

  /**
   * \brief Get meta data.
   * \return Returns weak_ptr containing metadata.
   * \remark If the type is not registered or this object is invalid, the empty
   * std::weak_ptr will be returned.
   */
  const Meta* GetMate() const;

  /**
   * \brief Get class meta data.
   * \return Returns the metadata of the class that holds the property.
   * \remark If the type is not registered, the containing nullptr will be returned.
   */
  const Meta* GetClassMeta() const;

 private:
  /**
   * \brief Gets the variable that the attribute refers to.
   * \param class_variable The class variable of this property.
   * \return A MM::Reflection::Variable that holds the this property contained
   * in \ref class_variable.
   */
  Variable GetPropertyVariable(Variable& class_variable) const;

  /**
   * \brief Gets the variable that the attribute refers to.
   * \param class_variable The class variable of this property.
   * \return A MM::Reflection::Variable that holds the this property contained
   * in \ref class_variable.
   */
  Variable GetPropertyVariable(const Variable& class_variable) const;

  explicit Property(std::unique_ptr<PropertyWrapperBase>&& property_wrapper) : property_wrapper_(std::move(property_wrapper)) {}

 private:
  std::unique_ptr<PropertyWrapperBase> property_wrapper_{nullptr};
};
}  // namespace Reflection
}  // namespace MM