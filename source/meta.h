#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "constructor.h"
#include "enum.h"
#include "method.h"
#include "property.h"

namespace MM {
namespace Reflection {
class Meta {
template <typename ClassType_> friend class Class;
template <typename EnumType_, EnumValue DefaultValue> friend class Enum;

public:
  Meta() = delete;
  ~Meta() = default;
  Meta(const std::string& type_name, const Type& type,
       std::unordered_map<std::string, Constructor>&& constructors =
           std::unordered_map<std::string, Constructor>{},
       std::unordered_map<std::string, Method>&& methods =
           std::unordered_map<std::string, Method>{},
       std::unordered_map<std::string, Property>&& properties =
           std::unordered_map<std::string, Property>{});
  Meta(const std::string& type_name, const Type& type, std::unordered_map<std::string, EnumPair>&& enums);
  Meta(const Meta& other) = delete;
  Meta(Meta&& other) noexcept;
  Meta& operator=(const Meta& other) = delete;
  Meta& operator=(Meta&& other) noexcept = delete;

public:
  /**
   * \brief Get type name of this meta.
   * \return The type name of this meta.
   */
 const std::string& GetTypeName() const;

 /**
  * \brief Get the \ref MM::Reflection::Type of this object.
  * \return The \ref MM::Reflection::Type of this object.
  */
 const Type& GetType() const;

 /**
  * \brief Determine whether the constuctor specified by \ref method_name
  * exists. \param constructor_name The constructor name; \return If the
  * specified constructor exists, return true; otherwise, return false.
  */
 bool HaveConstructor(const std::string& constructor_name) const;

 /**
  * \brief Get \ref MM::Reflection::Method with a name.
  * \param constructor_name The name of constructor.
  * \return The \ref MM::Reflection::Method.
  */
 const Constructor* GetConstructor(const std::string& constructor_name) const;

 /**
  * \brief Get all \ref MM::Reflection::Method of this meta.
  * \return All \ref MM::Reflection::Method of this meta.
  */
 std::vector<const Constructor*> GetAllConstructor() const;

 /**
  * \brief Get all \ref MM::Reflection::EnumPair of this meta.
  * \return All \ref MM::Reflection::EnumPair of this meta.
  */
 std::vector<const EnumPair*> GetAllEnums() const;

 /**
  * \brief Determine whether the method specified by \ref method_name exists.
  * \param method_name The method name;
  * \return If the specified method exists, return true; otherwise, return
  * false.
  */
 bool HaveMethod(const std::string& method_name) const;

 /**
  * \brief Get \ref MM::Reflection::Method with a name.
  * \param method_name The name of method.
  * \return The \ref MM::Reflection::Method.
  */
 const Method* GetMethod(const std::string& method_name) const;

 /**
  * \brief Get all \ref MM::Reflection::Method of this meta.
  * \return All \ref MM::Reflection::Method of this meta.
  */
 std::vector<const Method*> GetAllMethod() const;

 /**
  * \brief Determine whether the property specified by \ref property_name
  * exists. \param property_name The property name; \return If the specified
  * property exists, return true; otherwise, return false.
  */
 bool HaveProperty(const std::string& property_name) const;

 bool HaveEnum(const std::string& enum_name) const;

 /**
  * \brief Get \ref MM::Reflection::Property with a name.
  * \param property_name The name of property name.
  * \return The \ref MM::Reflection::Property.
  */
 const Property* GetProperty(const std::string& property_name) const;

 /**
  * \brief Get all \ref MM::Reflection::Property of this meta.
  * \return All \ref MM::Reflection::Property of this meta.
  */
 std::vector<const Property*> GetAllProperty() const;

 const EnumPair* GetEnum(const std::string& enum_name) const;

 const EnumPair* GetEnum(EnumValue enum_value) const;

 /**
  * \brief Add new constructor.
  * \param constructor The constructor method.
  * \return Returns true if the operation is successful, otherwise returns
  * false. \remark For the same meta, the name of the constructor must be
  * unique. If the action constructor has the same name as the old constructor,
  * the new constructor replaces the old constructor.
  */
 bool AddConstructor(Constructor&& constructor);

 void RemoveConstructor(const std::string& constructor_name);

 /**
  * \brief Register method to metadata.
  * \param method The method.
  * \return Returns true if the operation is successful, otherwise returns
  * false. \remark For the same meta, the name of the method must be unique. If
  * the action method has the same name as the old method, the new method
  * replaces the old method.
  */
 bool AddMethod(Method&& method);

 void RemoveMethod(const std::string& method_name);

 /**
  * \brief Register property to metadata.
  * \param property The property.
  * \return Returns true if the operation is successful, otherwise returns
  * false. \remark For the same meta, the name of the property must be unique.
  * \remark If a property with the same name is added to the meta, the operation
  * will not succeed and false will be returned.
  */
 bool AddProperty(Property&& property);

 void RemoveProperty(const std::string& property_name);

 bool AddEnum(EnumPair&& enum_pair);

 static const std::string& GetEnumConstructName();

 EnumItem CreateEnum(const std::string& enum_name) const;

 Variable CreateEnumVariable(const std::string& enum_name) const;

 /**
  * \brief Create instance with 0 arguments.
  * \param constructor_name The name of the constructor used.
  * \return \ref MM::Reflection::Variable containing the return value of this
  * function.
  * \remark If the number or type of incoming argument is different
  * from the argument required by the constructor held by this meta, the
  * function held by this object will not be called and return an empty \ref
  * MM::Reflection::Variable.
  */
 Variable CreateInstance(const std::string& constructor_name) const;

 /**
  * \brief Create instance with 1 arguments.
  * \param constructor_name The name of the constructor used.
  * \param arg1 1st argument.
  * \return \ref MM::Reflection::Variable containing the return value of this
  * function.
  * \remark If the number or type of incoming argument is different
  * from the argument required by the constructor held by this meta, the
  * function held by this object will not be called and return an empty \ref
  * MM::Reflection::Variable.
  */
 Variable CreateInstance(const std::string& constructor_name,
                         Variable& arg1) const;

 /**
  * \brief Create instance with 2 arguments.
  * \param constructor_name The name of the constructor used.
  * \param arg1 1st argument.
  * \param arg2 2ed argument
  * \return \ref MM::Reflection::Variable containing the return value of this
  * function.
  * \remark If the number or type of incoming argument is different
  * from the argument required by the constructor held by this meta, the
  * function held by this object will not be called and return an empty \ref
  * MM::Reflection::Variable.
  */
 Variable CreateInstance(const std::string& constructor_name, Variable& arg1,
                         Variable& arg2) const;

 /**
  * \brief Create instance with 3 arguments.
  * \param constructor_name The name of the constructor used.
  * \param arg1 1st argument.
  * \param arg2 2ed argument
  * \param arg3 3rd argument
  * \return \ref MM::Reflection::Variable containing the return value of this
  * function.
  * \remark If the number or type of incoming argument is different
  * from the argument required by the constructor held by this meta, the
  * function held by this object will not be called and return an empty \ref
  * MM::Reflection::Variable.
  */
 Variable CreateInstance(const std::string& constructor_name, Variable& arg1,
                         Variable& arg2, Variable& arg3) const;

 /**
  * \brief Create with 4 arguments.
  * \param constructor_name The name of the constructor used.
  * \param arg1 1st argument.
  * \param arg2 2ed argument.
  * \param arg3 3rd argument.
  * \param arg4 4th argument.
  * \return \ref MM::Reflection::Variable containing the return value of this
  * function.
  * \remark If the number or type of incoming argument is different
  * from the argument required by the constructor held by this meta, the
  * function held by this object will not be called and return an empty \ref
  * MM::Reflection::Variable.
  */
 Variable CreateInstance(const std::string& constructor_name, Variable& arg1,
                         Variable& arg2, Variable& arg3, Variable& arg4) const;

 /**
  * \brief Create instance with 5 arguments.
  * \param constructor_name The name of the constructor used.
  * \param arg1 1st argument.
  * \param arg2 2ed argument.
  * \param arg3 3rd argument.
  * \param arg4 4th argument.
  * \param arg5 5th argument.
  * \return \ref MM::Reflection::Variable containing the return value of this
  * function.
  * \remark If the number or type of incoming argument is different
  * from the argument required by the constructor held by this meta, the
  * function held by this object will not be called and return an empty \ref
  * MM::Reflection::Variable.
  */
 Variable CreateInstance(const std::string& constructor_name, Variable& arg1,
                         Variable& arg2, Variable& arg3, Variable& arg4,
                         Variable& arg5) const;

 /**
  * \brief Create instance with 6 arguments.
  * \param constructor_name The name of the constructor used.
  * \param arg1 1st argument.
  * \param arg2 2ed argument.
  * \param arg3 3rd argument.
  * \param arg4 4th argument.
  * \param arg5 5th argument.
  * \param arg6 6th argument.
  * \return \ref MM::Reflection::Variable containing the return value of this
  * function.
  * \remark If the number or type of incoming argument is different
  * from the argument required by the constructor held by this meta, the
  * function held by this object will not be called and return an empty \ref
  * MM::Reflection::Variable.
  */
 Variable CreateInstance(const std::string& constructor_name, Variable& arg1,
                         Variable& arg2, Variable& arg3, Variable& arg4,
                         Variable& arg5, Variable& arg6) const;

 /**
  * \brief Create instance with any number of parameters.
  * \param constructor_name The name of the constructor used.
  * \param args The list of arguments,
  * \return \ref MM::Reflection::Variable containing the return value of this
  * function.
  * \remark If the number or type of incoming argument is different
  * from the argument required by the constructor held by this meta, the
  * function held by this object will not be called and return an empty \ref
  * MM::Reflection::Variable.
  */
 Variable CreateInstance(const std::string& constructor_name,
                         std::vector<Variable*>& args) const;

 static const std::string& GetEmptyObjectMethodName();

 bool HaveEmptyObject() const;

 /**
  * \brief Get a reference to the stored empty object.
  * \note When registering metadata, if a static function of "GetEmptyObject"
  * without any parameters is registered, it will be called and an empty object
  * will be obtained, which will be captured and stored in the metadata. If the
  * "GetEmptyObject" function is not registered, only an invalid MM::
  * Reflection:: Variable object will be stored.
  * \return The stored empty
  * object.
  */
 Variable& GetEmptyVariable() const;

 /**
  * \brief Get the reference of "refrence Variable"(IsRefrenceVariable() result
  * is true) of the stored empty object.
  * \note When registering metadata, if a static function of "GetEmptyObject"
  * without any parameters is registered, it will be called and an empty object
  * will be obtained, which will be captured and stored in the metadata. If the
  * "GetEmptyObject" function is not registered, only an invalid MM::
  * Reflection:: Variable object will be stored.
  * \return The reference of "refrence Variable"(IsRefrenceVariable()
  * result is true) of the stored empty object.
  */
 Variable& GetEmptyVariableRefrence() const;

 /**
  * \brief Get the reference of "const refrence Variable"(IsRefrenceVariable()
  * && GetType()->IsConst() is true) of the stored empty object.
  * \note When registering metadata, if a static function of "GetEmptyObject"
  * without any parameters is registered, it will be called and an empty object
  * will be obtained, which will be captured and stored in the metadata. If the
  * "GetEmptyObject" function is not registered, only an invalid MM::
  * Reflection:: Variable object will be stored.
  * \return The reference of "const refrence Variable"(IsRefrenceVariable() &&
  * GetType()->IsConst() is true) of the stored empty object.
  */
 Variable& GetEmptyVariableConstRefrence() const;

 bool SetSerializerName(const std::string& serializer_name);

 void RemoveSerializerName();

 const std::string& GetSerializerName() const;

 bool HaveSerializer() const;

private:
 std::string type_name_{};

 /**
  * \brief The \ref MM::Reflection::Type of this object.
  */
 const Type* type_;

 /**
  * \brief Constructor map.
  */
 std::unordered_map<std::string, Constructor> constructors_;

 /**
  * \brief Method map.
  */
 std::unordered_map<std::string, Method> methods_;

 /**
  * \brief Property map.
  */
 std::unordered_map<std::string, Property> properties_;

 std::unordered_map<std::string, EnumPair> enums_;

 std::unordered_map<EnumValue, std::string> enum_value_to_name_;

 mutable Variable empty_variable_{};

 mutable Variable empty_variable_refrence_{};

 mutable Variable empty_variable_const_refrence_{};

 std::string serializer_name_{};
};
}
}
