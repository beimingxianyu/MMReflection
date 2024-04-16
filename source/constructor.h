#pragma once

#include "method.h"

namespace MM {
namespace Reflection {
class Constructor {
  friend class Meta;

 public:
  Constructor() = default;
  ~Constructor() = default;
  Constructor(const Constructor& other) = delete;
  Constructor(Constructor&& other) noexcept = default;
  Constructor& operator=(const Constructor& other) = delete;
  Constructor& operator=(Constructor&& other) noexcept = default;

 public:
  template<typename InstanceType, typename ...Args>
  static Constructor CreateConstructor(
      const std::string& constructor_name,
      Utils::GetFunctionPointTypeT<InstanceType, InstanceType, true, false, false, Args...> common_constructor_ptr,
      Utils::GetFunctionPointTypeT<void, InstanceType, true, false, false, void*, Args...> placement_constructor_ptr) {
    std::unique_ptr<MethodWrapperBase> common_constructor_wrapper = std::make_unique<MethodWrapper<InstanceType, InstanceType, true, false, false, Args...>>(constructor_name, common_constructor_ptr);
    std::unique_ptr<MethodWrapperBase> placement_constructor_wrapper = std::make_unique<MethodWrapper<void, InstanceType, true, false, false, void*, Args...>>(constructor_name, placement_constructor_ptr);
    return Constructor{std::move(common_constructor_wrapper), std::move(placement_constructor_wrapper)};
  }

 public:
  /**
   * \brief Get the constructor  name.
   * \return The constructor name.
   */
  const std::string& GetConstructorName() const;

  /**
   * \brief Get the hashcode of this constructor.
   * \return The hashcode of this constructor.
   * \remark The hash value depends on the constructor name, the type of all
   * parameters, the location of the parameters, the type of the class to which
   * it belongs.
   */
  std::size_t HashCode() const;

  /**
   * \brief Judge whether the object is a valid object.
   * \return Returns true if the object is a valid object, otherwise returns
   * false.
   */
  bool IsValid() const;

  /**
   * \brief Returns the number of arguments to the function.
   * \return The number of arguments to the function.
   * \remark Return 0 if the object is invalid.
   */
  std::size_t GetArgumentNumber() const;

  /**
   * \brief Get the \ref MM::Reflection::Type of class type.
   * \return The \ref MM::Reflection::Type of class type.
   */
  const Type* GetClassType() const;

  /**
   * \brief Get the \ref MM::Reflection::Type of result type.
   * \param argument_index The index of argument.
   * \return The \ref MM::Reflection::Type of target argument type.
   */
  const Type* GetArgumentType(std::uint32_t argument_index) const;

  /**
   * \brief Get instance meta data.
   * \return Returns metadata.
   * \remark If the type is not registered or this object is invalid, the
   * nullptr will be returned.
   */
  const Meta* GetClassMeta() const;

  const Meta* GetArgumentMeta(std::uint32_t argument_index) const;

  /**
   * \brief Invoke the function with 0 arguments.
   * \return \ref MM::Reflection::Variable containing the constructed value of
   * this function. \remark If this object is invalid ,empty \ref
   * MM::Reflection::Variable will be return. \remark If the number or type of
   * incoming argument is different from the argument required by the function
   * held by this object, the function held by this object will not be called
   * and return an empty \ref MM::Reflection::Variable.
   */
  Variable Invoke() const;

  /**
   * \brief Invoke the placement constructor with 0 arguments.
   * \param placement_address The location of the newly constructed
   * VariableWrapper<OriginalType>.
   * \remark The constructed object is MM::
   * Reflection::VariableWrapper<OriginalType>, not the original type, so the
   * reserved space size is not the same as the original type. It is recommended
   * to use MM::Reflection::GetVariableWrapperSize(std::uint64_t) to
   * calculate the required reserved space size.
   */
  Variable Invoke(void* placement_address) const;

  /**
   * \brief Invoke the function with 1 arguments.
   * \param arg1 1st argument.
   * \return \ref MM::Reflection::Variable containing the constructed value of
   * this function. \remark If this object is invalid ,empty \ref
   * MM::Reflection::Variable will be return. \remark If the number or type of
   * incoming argument is different from the argument required by the function
   * held by this object, the function held by this object will not be called
   * and return an empty \ref MM::Reflection::Variable.
   */
  Variable Invoke(Variable& arg1) const;

  /**
   * \brief Invoke the placement constructor with 0 arguments.
   * \param placement_address The location of the newly constructed
   * VariableWrapper<OriginalType>.
   * \remark The constructed object is MM::
   * Reflection::VariableWrapper<OriginalType>, not the original type, so the
   * reserved space size is not the same as the original type. It is recommended
   * to use MM::Reflection::GetVariableWrapperSize(std::uint64_t) to
   * calculate the required reserved space size.
   */
  Variable Invoke(void* placement_address, Variable& arg1) const;

  /**
   * \brief Invoke the function with 2 arguments.
   * \param arg1 1st argument.
   * \param arg2 2ed argument.
   * \return \ref MM::Reflection::Variable containing the constructed value of
   * this function. \remark If this object is invalid ,empty \ref
   * MM::Reflection::Variable will be return. \remark If the number or type of
   * incoming argument is different from the argument required by the function
   * held by this object, the function held by this object will not be called
   * and return an empty \ref MM::Reflection::Variable.
   */
  Variable Invoke(Variable& arg1, Variable& arg2) const;

  /**
   * \brief Invoke the placement constructor with 0 arguments.
   * \param placement_address The location of the newly constructed
   * VariableWrapper<OriginalType>.
   * \remark The constructed object is MM::
   * Reflection::VariableWrapper<OriginalType>, not the original type, so the
   * reserved space size is not the same as the original type. It is recommended
   * to use MM::Reflection::GetVariableWrapperSize(std::uint64_t) to
   * calculate the required reserved space size.
   */
  Variable Invoke(void* placement_address, Variable& arg1,
                  Variable& arg2) const;

  /**
   * \brief Invoke the function with 3 arguments.
   * \param arg1 1st argument.
   * \param arg2 2ed argument.
   * \param arg3 3rd argument.
   * \return \ref MM::Reflection::Variable containing the constructed value of
   * this function. \remark If this object is invalid ,empty \ref
   * MM::Reflection::Variable will be return. \remark If the number or type of
   * incoming argument is different from the argument required by the function
   * held by this object, the function held by this object will not be called
   * and return an empty \ref MM::Reflection::Variable.
   */
  Variable Invoke(Variable& arg1, Variable& arg2, Variable& arg3) const;

  /**
   * \brief Invoke the placement constructor with 0 arguments.
   * \param placement_address The location of the newly constructed
   * VariableWrapper<OriginalType>.
   * \remark The constructed object is MM::
   * Reflection::VariableWrapper<OriginalType>, not the original type, so the
   * reserved space size is not the same as the original type. It is recommended
   * to use MM::Reflection::GetVariableWrapperSize(std::uint64_t) to
   * calculate the required reserved space size.
   */
  Variable Invoke(void* placement_address, Variable& arg1, Variable& arg2,
                  Variable& arg3) const;

  /**
   * \brief Invoke the function with 4 arguments.
   * \param arg1 1st argument.
   * \param arg2 2ed argument.
   * \param arg3 3rd argument.
   * \param arg4 4th argument.
   * \return \ref MM::Reflection::Variable containing the constructed value of
   * this function. \remark If this object is invalid ,empty \ref
   * MM::Reflection::Variable will be return. \remark If the number or type of
   * incoming argument is different from the argument required by the function
   * held by this object, the function held by this object will not be called
   * and return an empty \ref MM::Reflection::Variable.
   */
  Variable Invoke(Variable& arg1, Variable& arg2, Variable& arg3,
                  Variable& arg4) const;

  /**
   * \brief Invoke the placement constructor with 0 arguments.
   * \param placement_address The location of the newly constructed
   * VariableWrapper<OriginalType>.
   * \remark The constructed object is MM::
   * Reflection::VariableWrapper<OriginalType>, not the original type, so the
   * reserved space size is not the same as the original type. It is recommended
   * to use MM::Reflection::GetVariableWrapperSize(std::uint64_t) to
   * calculate the required reserved space size.
   */
  Variable Invoke(void* placement_address, Variable& arg1, Variable& arg2,
                  Variable& arg3, Variable& arg4) const;

  /**
   * \brief Invoke the function with 5 arguments.
   * \param arg1 1st argument.
   * \param arg2 2ed argument.
   * \param arg3 3rd argument.
   * \param arg4 4th argument.
   * \param arg5 5th argument.
   * \return \ref MM::Reflection::Variable containing the constructed value of
   * this function. \remark If this object is invalid ,empty \ref
   * MM::Reflection::Variable will be return. \remark If the number or type of
   * incoming argument is different from the argument required by the function
   * held by this object, the function held by this object will not be called
   * and return an empty \ref MM::Reflection::Variable.
   */
  Variable Invoke(Variable& arg1, Variable& arg2, Variable& arg3,
                  Variable& arg4, Variable& arg5) const;

  /**
   * \brief Invoke the placement constructor with 0 arguments.
   * \param placement_address The location of the newly constructed
   * VariableWrapper<OriginalType>.
   * \remark The constructed object is MM::
   * Reflection::VariableWrapper<OriginalType>, not the original type, so the
   * reserved space size is not the same as the original type. It is recommended
   * to use MM::Reflection::GetVariableWrapperSize(std::uint64_t) to
   * calculate the required reserved space size.
   */
  Variable Invoke(void* placement_address, Variable& arg1, Variable& arg2,
                  Variable& arg3, Variable& arg4, Variable& arg5) const;

  /**
   * \brief Invoke the function with 6 arguments.
   * \param arg1 1st argument.
   * \param arg2 2ed argument.
   * \param arg3 3rd argument.
   * \param arg4 4th argument.
   * \param arg5 5th argument.
   * \param arg6 6th argument.
   * \return \ref MM::Reflection::Variable containing the constructed value of
   * this function. \remark If this object is invalid ,empty \ref
   * MM::Reflection::Variable will be return. \remark If the number or type of
   * incoming argument is different from the argument required by the function
   * held by this object, the function held by this object will not be called
   * and return an empty \ref MM::Reflection::Variable.
   */
  Variable Invoke(Variable& arg1, Variable& arg2, Variable& arg3,
                  Variable& arg4, Variable& arg5, Variable& arg6) const;

  /**
   * \brief Invoke the placement constructor with 0 arguments.
   * \param placement_address The location of the newly constructed
   * VariableWrapper<OriginalType>.
   * \remark The constructed object is MM::
   * Reflection::VariableWrapper<OriginalType>, not the original type, so the
   * reserved space size is not the same as the original type. It is recommended
   * to use MM::Reflection::GetVariableWrapperSize(std::uint64_t) to
   * calculate the required reserved space size.
   */
  Variable Invoke(void* placement_address, Variable& arg1, Variable& arg2,
                  Variable& arg3, Variable& arg4, Variable& arg5,
                  Variable& arg6) const;

  /**
   * \brief Call the function with any number of parameters.
   * \param args The list of arguments,
   * \return \ref MM::Reflection::Variable containing the return value of this
   * function.
   * \remark If this object is invalid ,empty \ref MM::Reflection::Variable will
   * be return. \remark If the number or type of incoming argument is different
   * from the argument required by the function held by this object, the
   * function held by this object will not be called and return an empty \ref
   * MM::Reflection::Variable.
   */
  Variable Invoke(std::vector<Variable*>& args) const;

  /**
   * \brief Invoke the placement constructor with 0 arguments.
   * \param placement_address The location of the newly constructed
   * VariableWrapper<OriginalType>.
   * \remark The constructed object is MM::
   * Reflection::VariableWrapper<OriginalType>, not the original type, so the
   * reserved space size is not the same as the original type. It is recommended
   * to use MM::Reflection::GetVariableWrapperSize(std::uint64_t) to
   * calculate the required reserved space size.
   */
  Variable Invoke(void* placement_address, std::vector<Variable*>& args) const;

  /**
   * \brief Call the function with any number of parameters.
   * \param args The list of arguments,
   * \return \ref MM::Reflection::Variable containing the return value of this
   * function.
   * \remark If this object is invalid ,empty \ref MM::Reflection::Variable will
   * be return. \remark If the number or type of incoming argument is different
   * from the argument required by the function held by this object, the
   * function held by this object will not be called and return an empty \ref
   * MM::Reflection::Variable.
   */
  Variable Invoke(std::vector<Variable*>&& args) const;

  /**
   * \brief Invoke the placement constructor with 0 arguments.
   * \param placement_address The location of the newly constructed
   * VariableWrapper<OriginalType>.
   * \remark The constructed object is MM::
   * Reflection::VariableWrapper<OriginalType>, not the original type, so the
   * reserved space size is not the same as the original type. It is recommended
   * to use MM::Reflection::GetVariableWrapperSize(std::uint64_t) to
   * calculate the required reserved space size.
   */
  Variable Invoke(void* placement_address, std::vector<Variable*>&& args) const;

 private:
  Constructor(std::unique_ptr<MethodWrapperBase>&& common_constructor_wrapper, std::unique_ptr<MethodWrapperBase>&& placement_constructor_wrapper);

 private:
  static Variable empty_variable;

  private:
    std::unique_ptr<MethodWrapperBase> common_constructor_wrapper_{};
    std::unique_ptr<MethodWrapperBase> placement_constructor_wrapper_{};
};
}
}