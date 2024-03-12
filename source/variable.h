#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include "type.h"
#include "type_utils.h"

namespace MM
{
    namespace Reflection
    {
        class Property;
        class Variable;

        class VariableWrapperBase
        {
            friend class Variable;

        public:
            VariableWrapperBase() = default;
            virtual ~VariableWrapperBase() = default;
            VariableWrapperBase(const VariableWrapperBase&) = default;
            VariableWrapperBase(VariableWrapperBase&&) = default;
            VariableWrapperBase& operator=(const VariableWrapperBase&) = default;
            VariableWrapperBase& operator=(VariableWrapperBase&&) = default;

        public:
            virtual bool IsValid() const;

            virtual bool IsPropertyVariable() const;

            virtual bool IsVoid() const;

            virtual bool IsRefrenceVariable() const;

            /**
             * \brief Get a base pointer of copy.
             * \return The base pointer of copy.
             */
            virtual std::unique_ptr<VariableWrapperBase> CopyToBasePointer() const;

            /**
             * \brief Get a base pointer of move.
             * \return The base pointer of move.
             */
            virtual std::unique_ptr<VariableWrapperBase> MoveToBasePointer();

            /**
             * \brief Get the value of the object held by this object.
             * \return The value pointer of the object held by this object.
             */
            virtual const void* GetValue() const;

            /**
             * \brief Get the value of the object held by this object.
             * \return The value pointer of the object held by this object.
             */
            virtual void* GetValue();

            /**
             * \brief Set the value of the object held by this object.
             * \param object A void * pointer to the new value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            virtual bool SetValue(const void* object);

            /**
             * \brief Copy the new value to the object held by the pair, and this function will call the copy assignment function.
             * \param object A void * pointer to the new value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            virtual bool CopyValue(const void* object);

            /**
             * \brief Move the new value to the object held by the pair, and this function will call the move assignment function.
             * \param object A void * pointer to the new value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            virtual bool MoveValue(void* object);

            /**
             * \brief Gets the properties of the object held by this object.
             * \param property_name The name of property.
             * \return A MM::Reflection::Variable that holds the specific property.
             */
            virtual Variable GetPropertyVariable(const std::string& property_name) const;

            /**
             * \brief Get the MM::Reflection::Type of the object held by this object.
             * \return The MM::Reflection::TypeWrapper of the object held by this object.
             */
            virtual const MM::Reflection::Type* GetType() const;

            /**
             * \brief Get meta data.
             * \return Returns unique_ptr containing metadata.
             * \remark If the type is not registered, the unique_ptr containing nullptr
             * will be returned.
             */
            virtual const Meta* GetMeta() const;

            /**
             * \brief Returns a pointer to the managed object and releases the ownership.
             * \return The pointer to the managed object.
             */
            virtual void* ReleaseOwnership();
        };


        template <typename VariableType_>
        class VariableWrapper final : public VariableWrapperBase
        {
            template <typename TargetType_, typename DestructorType_>
            friend class DestructorWrapper;

            friend class Variable;

        public:
            using Type = VariableType_;
            // using OriginalType = typename TypeWrapper<VariableType_>::OriginalType;
            using OriginalType = Utils::GetOriginalTypeT<VariableType_>;

        public:
            ~VariableWrapper() override = default;

            /**
             * \brief Copy constructor.
             * \param other Objects to be copied.
             */
            VariableWrapper(const VariableWrapper& other)
                : value_(nullptr)
            {
                if (!other.IsValid())
                {
                    return;
                }

                value_.reset(std::make_unique<VariableType_>(*other.value_).release());
            }

            /**
             * \brief Move constructor.
             * \param other Objects to be moved.
             */
            VariableWrapper(VariableWrapper&& other) noexcept
                : value_(nullptr)
            {
                value_.reset(std::make_unique<VariableType_>(std::move(*other.value_)).release());
            }

            /**
             * \brief Copy assign.
             * \param other Objects to be copied.
             * \return New objects after copying.
             */
            VariableWrapper& operator=(const VariableWrapper& other)
            {
                if (std::addressof(other) == this)
                {
                    return *this;
                }
                //  The CopyValue() function will detect whether the object is a valid object.
                // if (!IsValid() || !other.IsValid()) {
                //   return *this;
                // }

                CopyValue(other.GetValue());

                return *this;
            }

            /**
             * \brief Move assign.
             * \tparam TestMove Determine whether move assign can be performed.
             * \param other Objects to be moved.
             * \return New objects after moving.
             */
            VariableWrapper& operator=(VariableWrapper&& other) noexcept
            {
                if (std::addressof(other) == this)
                {
                    return *this;
                }

                //  The CopyValue() function will detect whether the object is a valid object.
                // if (!IsValid() || !other.IsValid()) {
                //   return *this;
                // }

                MoveValue(other.GetValue());

                return *this;
            }

            explicit VariableWrapper(std::unique_ptr<VariableType_>&& variable_ptr)
                : value_(variable_ptr.release())
            {
            }

            explicit VariableWrapper(const VariableType_& other)
                : value_(std::make_unique<VariableType_>(other))
            {
            }

            explicit VariableWrapper(VariableType_&& other)
                : value_(std::make_unique<VariableType_>(std::move(other)))
            {
            }

            template <typename... Args>
            explicit VariableWrapper(Args... args)
                : value_(std::make_unique<VariableType_>(std::forward<Args>(args)...))
            {
            }

        public:
            bool IsPropertyVariable() const override
            {
                return false;
            }

            bool IsValid() const override
            {
                return GetValue() != nullptr;
            }

            std::unique_ptr<VariableWrapperBase> CopyToBasePointer() const override
            {
                if constexpr (std::is_copy_constructible_v<VariableType_>)
                {
                    return std::make_unique<VariableWrapperBase>(
                        VariableWrapper<VariableType_>(*this));
                }
                else
                {
                    return nullptr;
                }
            }

            std::unique_ptr<VariableWrapperBase> MoveToBasePointer() override
            {
                if constexpr (std::is_move_constructible_v<VariableType_>)
                {
                    return std::make_unique<VariableWrapperBase>(
                        VariableWrapper<VariableType_>(std::move(*this)));
                }
                else
                {
                    return nullptr;
                }
            }

            /**
             * \brief Get the MM::Reflection::Type of the object held by this object.
             * \return The "MM::Reflection::Type" of the object held by this object.
             */
            const MM::Reflection::Type* GetType() const override
            {
                const MM::Reflection::Type& Result = MM::Reflection::Type::CreateType<VariableType_>();
                return &Result;
            }

            /**
             * \brief Get meta data.
             * \return Returns unique_ptr containing metadata.
             * \remark If the type is not registered, the unique_ptr containing nullptr
             * will be returned.
             */
            const Meta* GetMeta() const override
            {
                return GetType()->GetMate();
            }

            /**
             * \brief Get the value of the object held by this object.
             * \return The value pointer of the object held by this object.
             */
            const void* GetValue() const override
            {
                return value_.get();
            }

            /**
             * \brief Get the value of the object held by this object.
             * \return The value pointer of the object held by this object.
             */
            void* GetValue() override
            {
                return (void*)(value_.get());
            }

            /**
             * \brief Set the value of the object held by this object.
             * \param object A void * pointer to the new value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            bool SetValue(const void* object) override
            {
                if constexpr (std::is_copy_constructible_v<VariableType_>)
                {
                    if (object == nullptr)
                    {
                        return false;
                    }
                    value_.reset(
                        std::make_unique<VariableType_>(*static_cast<VariableType_*>(const_cast<void*>(object))).
                        release());
                    return true;
                }

                return false;
            }

            /**
             * \brief Copy the new value to the object held by the pair, and this function will call the copy assignment function.
             * \param object A void * pointer to the new value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            bool CopyValue(const void* object) override
            {
                if constexpr (std::is_copy_assignable_v<VariableType_>)
                {
                    if (object == nullptr || !IsValid())
                    {
                        return false;
                    }
                    *value_ = *((VariableType_*)object);
                    return true;
                }

                return false;
            }

            /**
             * \brief Move the new value to the object held by the pair, and this function will call the move assignment function.
             * \param object A void * pointer to the new value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            bool MoveValue(void* object) override
            {
                if constexpr (std::is_move_assignable_v<VariableType_>)
                {
                    if (object == nullptr || !IsValid())
                    {
                        return false;
                    }
                    *value_ = std::move(*static_cast<VariableType_*>(object));
                    return true;
                }

                return false;
            }

            /**
             * \brief Returns a pointer to the managed object and releases the ownership.
             * \return The pointer to the managed object.
             */
            void* ReleaseOwnership() override
            {
                return (void*)(value_.release());
            }

        private:
            std::unique_ptr<VariableType_> value_ = nullptr;
        };

        template <typename VariableType_>
        class VariableRefrenceWrapper : public VariableWrapperBase
        {
        public:
            using VariableRefrenceType = std::add_lvalue_reference_t<std::remove_reference_t<VariableType_>>;

        public:
            ~VariableRefrenceWrapper() override = default;

            /**
             * \brief Copy constructor.
             * \param other Objects to be copied.
             */
            VariableRefrenceWrapper(const VariableRefrenceWrapper& other)
                : value_(other.value_)
            {
            }


            explicit VariableRefrenceWrapper(VariableRefrenceType other)
                : value_(other)
            {
            };

            /**
             * \brief Copy constructor.
             * \param other Objects to be copied.
             */
            VariableRefrenceWrapper(VariableRefrenceWrapper& other)
                : value_(other.value_)
            {
            }

            /**
             * \brief Copy assign.
             * \param other Objects to be copied.
             * \return New objects after copying.
             */
            VariableRefrenceWrapper& operator=(const VariableRefrenceWrapper& other)
            {
                if (std::addressof(other) == this)
                {
                    return *this;
                }

                value_ = other.value_;

                return *this;
            }

            /**
             * \brief Move assign.
             * \tparam TestMove Determine whether move assign can be performed.
             * \param other Objects to be moved.
             * \return New objects after moving.
             */
            VariableRefrenceWrapper& operator=(VariableRefrenceWrapper&& other) noexcept
            {
                if (std::addressof(other) == this)
                {
                    return *this;
                }

                other.value_ = std::move(other.value_);

                return *this;
            }

        public:
            bool IsPropertyVariable() const override
            {
                return false;
            }

            bool IsRefrenceVariable() const override
            {
                return true;
            }

            bool IsValid() const override
            {
                return true;
            }

            std::unique_ptr<VariableWrapperBase> CopyToBasePointer() const override
            {
                if constexpr (std::is_copy_constructible_v<VariableType_>)
                {
                    return std::make_unique<VariableWrapperBase>(
                        VariableWrapper<std::remove_reference_t<VariableType_>>(value_));
                }
                else
                {
                    return nullptr;
                }
            }

            std::unique_ptr<VariableWrapperBase> MoveToBasePointer() override
            {
                if constexpr (std::is_move_constructible_v<VariableType_>)
                {
                    return std::make_unique<VariableWrapperBase>(
                        VariableWrapper<std::remove_reference_t<VariableType_>>(std::move(value_)));
                }
                else
                {
                    return nullptr;
                }
            }

            /**
             * \brief Get the MM::Reflection::Type of the object held by this object.
             * \return The "MM::Reflection::Type" of the object held by this object.
             */
            const MM::Reflection::Type* GetType() const override
            {
                const MM::Reflection::Type& Result = MM::Reflection::Type::CreateType<VariableRefrenceType>();
                return &Result;
            }

            /**
             * \brief Get meta data.
             * \return Returns unique_ptr containing metadata.
             * \remark If the type is not registered, the unique_ptr containing nullptr
             * will be returned.
             */
            const Meta* GetMeta() const override
            {
                return GetType()->GetMate();
            }

            /**
             * \brief Get the value of the object held by this object.
             * \return The value pointer of the object held by this object.
             */
            const void* GetValue() const override
            {
                return &value_;
            }

            /**
             * \brief Get the value of the object held by this object.
             * \return The value pointer of the object held by this object.
             */
            void* GetValue() override
            {
                return (void*)(&value_);
            }

            /**
             * \brief Set the value of the object held by this object.
             * \param object A void * pointer to the new value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            bool SetValue(const void* object) override
            {
                if constexpr (std::is_copy_constructible_v<VariableType_>)
                {
                    if (object == nullptr)
                    {
                        return false;
                    }
                    // using CopyPtrType = std::add_pointer_t<std::remove_reference_t<std::remove_const_t<VariableType_>>>;
                    *const_cast<std::add_pointer_t<Utils::GetNotConstRefrenceT<VariableRefrenceType>>>(&value_) = *
                        static_cast<std::add_pointer_t<std::add_const_t<std::remove_reference_t<VariableRefrenceType>>>>
                        (object);
                    return true;
                }

                return false;
            }

            /**
             * \brief Copy the new value to the object held by the pair, and this function will call the copy assignment function.
             * \param object A void * pointer to the new value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            bool CopyValue(const void* object) override
            {
                if constexpr (std::is_copy_assignable_v<VariableType_>)
                {
                    if (object == nullptr)
                    {
                        return false;
                    }
                    *const_cast<std::add_pointer_t<Utils::GetNotConstRefrenceT<VariableRefrenceType>>>(&value_) = *
                        static_cast<std::add_pointer_t<std::add_const_t<std::remove_reference_t<VariableRefrenceType>>>>
                        (object);
                    return true;
                }

                return false;
            }

            /**
             * \brief Move the new value to the object held by the pair, and this function will call the move assignment function.
             * \param object A void * pointer to the new value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            bool MoveValue(void* object) override
            {
                if constexpr (std::is_move_assignable_v<VariableType_>)
                {
                    if (object == nullptr)
                    {
                        return false;
                    }
                    *const_cast<std::add_pointer_t<Utils::GetNotConstRefrenceT<VariableRefrenceType>>>(&value_) =
                        std::move(
                            *static_cast<std::add_pointer_t<std::add_const_t<std::remove_reference_t<
                                VariableRefrenceType>>>>(object));
                    return true;
                }

                return false;
            }

            /**
             * \brief Returns a pointer to the managed object and releases the ownership.
             * \return Always return nullptr.
             */
            void* ReleaseOwnership() override
            {
                return nullptr;
            }

        protected:
            VariableRefrenceType value_;
        };

        template <typename PropertyType_>
        class PropertyVariableWrapper : public VariableRefrenceWrapper<PropertyType_>
        {
        public:
            ~PropertyVariableWrapper() override = default;

            /**
             * \brief Copy constructor.
             * \param other Objects to be copied.
             */
            PropertyVariableWrapper(const PropertyVariableWrapper& other)
                : VariableRefrenceWrapper<PropertyType_>(other)
            {
            }

            /**
             * \brief Move constructor.
             * \param other Objects to be moved.
             */
            PropertyVariableWrapper(PropertyVariableWrapper&& other) noexcept
                : VariableRefrenceWrapper<PropertyType_>(std::move(other))
            {
            }

            /**
             * \brief Copy assign.
             * \param other Objects to be copied.
             * \return New objects after copying.
             */
            PropertyVariableWrapper& operator=(const PropertyVariableWrapper& other)
            {
                if (std::addressof(other) == this)
                {
                    return *this;
                }

                VariableRefrenceWrapper<PropertyType_>::value_ = other.VariableRefrenceWrapper<PropertyType_>::value_;

                return *this;
            }

            /**
             * \brief Move assign.
             * \tparam TestMove Determine whether move assign can be performed.
             * \param other Objects to be moved.
             * \return New objects after moving.
             */
            PropertyVariableWrapper& operator=(PropertyVariableWrapper&& other) noexcept
            {
                if (std::addressof(other) == this)
                {
                    return *this;
                }

                VariableRefrenceWrapper<PropertyType_>::value_ = std::move(
                    other.VariableRefrenceWrapper<PropertyType_>::value_);

                return *this;
            }

            explicit PropertyVariableWrapper(std::unique_ptr<PropertyType_>&& variable_ptr)
                : VariableRefrenceWrapper<PropertyType_>(std::move(variable_ptr))
            {
            }

            // explicit PropertyVariableWrapper(const PropertyType_& other)
            //   : VariableRefrenceWrapper<PropertyType_>(other) {}

            explicit PropertyVariableWrapper(PropertyType_& other)
                : VariableRefrenceWrapper<PropertyType_>(other)
            {
            }

        public:
            bool IsPropertyVariable() const override
            {
                return true;
            }
        };

        class VoidVariable : public VariableWrapperBase
        {
        public:
            VoidVariable() = default;
            ~VoidVariable() override = default;
            VoidVariable(const VoidVariable& other) = default;
            VoidVariable(VoidVariable&& other) noexcept = default;
            VoidVariable& operator=(const VoidVariable& other) = default;
            VoidVariable& operator=(VoidVariable&& other) noexcept = default;

        public:
            bool IsValid() const override;

            bool IsVoid() const override;

            /**
             * \brief Get a base pointer of copy.
             * \return The base pointer of copy.
             */
            std::unique_ptr<VariableWrapperBase> CopyToBasePointer() const override;

            /**
             * \brief Get a base pointer of move.
             * \return The base pointer of move.
             */
            std::unique_ptr<VariableWrapperBase> MoveToBasePointer() override;

            /**
             * \brief Get the MM::Reflection::Type of the object held by this object.
             * \return The MM::Reflection::TypeWrapper of the object held by this object.
             */
            const MM::Reflection::Type* GetType() const override;

            /**
             * \brief Get meta data.
             * \return Returns unique_ptr containing metadata.
             * \remark If the type is not registered, the unique_ptr containing nullptr
             * will be returned.
             */
            const Meta* GetMeta() const override;
        };


        class Variable
        {
            friend class VariableWrapperBase;
            template <typename VariableType>
            friend class VariableWrapper;
            template <typename VariableType>
            friend class VariableRefrenceWrapper;
            template <typename PropertyType, typename ClassType>
            friend class PropertyWrapper;
            template <typename TargetType_, typename DestructorType_>
            friend class DestructorWrapper;

        public:
            /**
             * \brief Get the value and perform type conversion.
             * \tparam VariableType The type of the value.
             * \return The value you want.
             */
            template <typename VariableType>
            const VariableType& GetValueCast() const
            {
                return *static_cast<const VariableType*>(GetValue());
            }

            /**
             * \brief Get the value and perform type conversion.
             * \tparam VariableType The type of the value.
             * \return The value you want.
             */
            template <typename VariableType>
            VariableType& GetValueCast()
            {
                return *static_cast<VariableType*>(GetValue());
            }

            /**
             * \brief Set new value.
             * \tparam TargetType The type of new value.
             * \tparam VariableType The type of value to be changed.
             * \tparam TestConvertible Determine whether "A" can be converted to "B".
             * \param other New value.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             */
            template <typename TargetType, typename VariableType,
                      typename TestConvertible =
                      std::enable_if_t<
                          Utils::Conversion<TargetType, VariableType>::value>>
            bool SetValueCast(TargetType&& other)
            {
                VariableType value = static_cast<VariableType>(GetValue());
                if (value == nullptr)
                {
                    return false;
                }
                value = other;

                return true;
            }

        public:
            /**
             * \brief Default initialization. The initialized object is invalid.
             */
            Variable() = default;

            /**
             * \brief Use the registered destructor to destruct. If the destructor is not registered actively, the destructor of each class itself will be used.
             */
            ~Variable();

            /**
             * \brief Create a \ref MM::Reflection::Variable from an rvalue.
             * \tparam VariableType VariableType The type of rvalue.
             * \param other One object.
             * \param is_refrence Is true, create refrence variable, otherwise create common variable.
             * \remark The new \ref MM::Reflection::Variable holds a value to other.
             */
            template <typename VariableType>
            static Variable CreateVariable(VariableType&& other, bool is_refrence = false)
            {
                if constexpr (std::is_same_v<VariableType, void>)
                {
                    return Variable::CreateVoidVariable();
                }
                if (is_refrence || std::is_lvalue_reference_v<VariableType>)
                {
                    return Variable{std::make_unique<VariableRefrenceWrapper<VariableType>>(other)};
                }

                return Variable{
                    std::make_unique<VariableWrapper<std::remove_reference_t<VariableType>>>(
                        std::forward<VariableType>(other))
                };
            }

            template <typename VariableType, typename... Args>
            static Variable EmplaceVariable(Args&&... args)
            {
                return Variable{
                    std::make_unique<VariableWrapper<std::remove_reference_t<VariableType>>>(
                        std::forward<Args>(args)...)
                };
            }

            static Variable CreateVoidVariable();

            /**
             * \brief Copy constructor.
             * \param other Other objects will be copied to this object.
             * \remark If the value held by this object has no copy constructor, this
             * function will do nothing. \remark If \ref other is an invalid object, this
             * object will also change to an invalid object.
             */
            Variable(const Variable& other);

            /**
             * \brief Move constructor.
             * \param other Other objects will be moved to this object.
             * \remark If the value held by this object has no move constructor, this
             * function will do nothing.
             * \remark If \ref other is an invalid object, this
             * object will also change to an invalid object.
             */
            Variable(Variable&& other) noexcept;

            /**
             * \brief Copy Assign.
             * \param other Other objects will be copied to this object.
             * \remark If the value held by this object has no copy assign, this
             * function will do nothing.
             * \remark If \ref other is an invalid object, this
             * object will also change to an invalid object.
             */
            Variable& operator=(const Variable& other);

            /**
             * \brief Move Assign.
             * \param other Other objects will be moved to this object.
             * \remark If the value held by this object has no move assign, this
             * function will do nothing.
             * \remark If \ref other is an invalid object, this object will also
             * change to an invalid object.
             */
            Variable& operator=(Variable&& other) noexcept;

            /**
             * \brief Construct an object from an std::unique_ptr<VariableWrapperBase>.
             * \param variable_wrapper std::unique_ptr<VariableWrapperBase> containing variable data.
             */
            explicit Variable(std::unique_ptr<VariableWrapperBase>&& variable_wrapper);

        public:
            /**
             * \brief bool conversion.Call \ref IsValid.
             */
            explicit operator bool() const;

            /**
             * \brief Judge whether the object is a valid object.
             * \return Returns true if the object is a valid object, otherwise returns
             * false.
             */
            bool IsValid() const;

            bool IsVoid() const;

            bool IsRefrenceVariable() const;

            bool IsPropertyVariable() const;

            /**
             * \brief Get the value of the object held by this object.
             * \return The value pointer of the object held by this object.
             * \remark It is not recommended for users and may cause runtime errors.
             * \remark If object is invalid, it will return nullptr.
             */
            const void* GetValue() const;

            /**
             * \brief Get the value of the object held by this object.
             * \return The value pointer of the object held by this object.
             * \remark It is not recommended for users and may cause runtime errors.
             * \remark If object is invalid, it will return nullptr.
             */
            void* GetValue();

            /**
             * \brief Set the value of the object held by this object.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             * \remark It is not recommended for users and may cause runtime errors.
             * \remark If object is invalid, it will do nothing and return false.
             */
            bool SetValue(const void* other);

            /**
             * \brief Copy the new value to the object held by the pair, and this function will call the copy assignment function.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             * \remark It is not recommended for users and may cause runtime errors.
             * \remark If object is not valid, it will do nothing and return false.
             */
            bool CopyValue(const void* other);

            /**
             * \brief Move the new value to the object held by the pair, and this function will call the move assignment function.
             * \return Returns true if the value is set successfully, otherwise returns
             * false.
             * \remark It is not recommended for users and may cause runtime errors.
             * \remark If object is not valid, it will do nothing and return false.
             */
            bool MoveValue(void* other);


            /**
             * \brief Get the MM::Reflection::Type of the object held by this object.
             * \return The "MM::Reflection::Type" of the object held by this object.
             * \remark If object is not valid, it will return the nullptr.
             */
            const Type* GetType() const;

            /**
             * \brief Get meta data.
             * \return Returns std::weak_ptr containing metadata.
             * \remark If the type is not registered or this object is invalid, the
             * nullptr will be returned.
             */
            const Meta* GetMeta() const;

            /**
             * \brief Gets the properties of the object held by this object.
             * \param property_name The name of property.
             * \return A MM::Reflection::Variable that holds the specific property.
             */
            Variable GetPropertyVariable(const std::string& property_name);

            /**
             * \brief Gets the properties of the object held by this object.
             * \param property_name The name of property.
             * \return A MM::Reflection::Variable that holds the specific property.
             */
            Variable GetPropertyVariable(const std::string& property_name) const;

            /**
             * \brief Invoke the function with 0 arguments.
             * \param method_name The name of a method that you want call.
             * \return \ref MM::Reflection::Variable containing the return value of this
             * function.
             * \remark If the number or type of incoming argument is different
             * from the argument required by the function held by this object or the target method not exist, the
             * function held by this object will not be called and return an empty \ref
             * MM::Reflection::Variable.
             */
            Variable Invoke(const std::string& method_name);

            /**
             * \brief Invoke the function with 0 arguments.
             * \param method_name The name of a method that you want call.
             * \param arg1 1st argument.
             * \return \ref MM::Reflection::Variable containing the return value of this
             * function.
             * \remark If the number or type of incoming argument is different
             * from the argument required by the function held by this object or the target method not exist, the
             * function held by this object will not be called and return an empty \ref
             * MM::Reflection::Variable.
             */
            Variable Invoke(const std::string& method_name, Variable& arg1);

            /**
             * \brief Invoke the function with 0 arguments.
             * \param method_name The name of a method that you want call.
             * \param arg1 1st argument.
             * \param arg2 2ed argument.
             * \return \ref MM::Reflection::Variable containing the return value of this
             * function.
             * \remark If the number or type of incoming argument is different
             * from the argument required by the function held by this object or the target method not exist, the
             * function held by this object will not be called and return an empty \ref
             * MM::Reflection::Variable.
             */
            Variable Invoke(const std::string& method_name, Variable& arg1, Variable& arg2);

            /**
             * \brief Invoke the function with 0 arguments.
             * \param method_name The name of a method that you want call.
             * \param arg1 1st argument.
             * \param arg2 2ed argument.
             * \param arg3 3rd argument.
             * \return \ref MM::Reflection::Variable containing the return value of this
             * function.
             * \remark If the number or type of incoming argument is different
             * from the argument required by the function held by this object or the target method not exist, the
             * function held by this object will not be called and return an empty \ref
             * MM::Reflection::Variable.
             */
            Variable Invoke(const std::string& method_name, Variable& arg1, Variable& arg2, Variable& arg3);

            /**
             * \brief Invoke the function with 0 arguments.
             * \param method_name The name of a method that you want call.
             * \param arg1 1st argument.
             * \param arg2 2ed argument.
             * \param arg3 3rd argument.
             * \param arg4 4th argument.
             * \return \ref MM::Reflection::Variable containing the return value of this
             * function.
             * \remark If the number or type of incoming argument is different
             * from the argument required by the function held by this object or the target method not exist, the
             * function held by this object will not be called and return an empty \ref
             * MM::Reflection::Variable.
             */
            Variable Invoke(const std::string& method_name, Variable& arg1, Variable& arg2, Variable& arg3,
                            Variable& arg4);

            /**
             * \brief Invoke the function with 0 arguments.
             * \param method_name The name of a method that you want call.
             * \param arg1 1st argument.
             * \param arg2 2ed argument.
             * \param arg3 3rd argument.
             * \param arg4 4th argument.
             * \param arg5 5th argument.
             * \return \ref MM::Reflection::Variable containing the return value of this
             * function.
             * \remark If the number or type of incoming argument is different
             * from the argument required by the function held by this object or the target method not exist, the
             * function held by this object will not be called and return an empty \ref
             * MM::Reflection::Variable.
             */
            Variable Invoke(const std::string& method_name, Variable& arg1, Variable& arg2, Variable& arg3,
                            Variable& arg4, Variable& arg5);

            /**
             * \brief Invoke the function with 0 arguments.
             * \param method_name The name of a method that you want call.
             * \param arg1 1st argument.
             * \param arg2 2ed argument.
             * \param arg3 3rd argument.
             * \param arg4 4th argument.
             * \param arg5 5th argument.
             * \param arg6 6th argument.
             * \return \ref MM::Reflection::Variable containing the return value of this
             * function.
             * \remark If the number or type of incoming argument is different
             * from the argument required by the function held by this object or the target method not exist, the
             * function held by this object will not be called and return an empty \ref
             * MM::Reflection::Variable.
             */
            Variable Invoke(const std::string& method_name, Variable& arg1, Variable& arg2, Variable& arg3,
                            Variable& arg4, Variable& arg5, Variable& arg6);

            /**
             * \brief Invoke the function with 0 arguments.
             * \param method_name The name of a method that you want call.
             * \param args Arguments.
             * \return \ref MM::Reflection::Variable containing the return value of this
             * function.
             * \remark If the number or type of incoming argument is different
             * from the argument required by the function held by this object or the target method not exist, the
             * function held by this object will not be called and return an empty \ref
             * MM::Reflection::Variable.
             */
            Variable Invoke(const std::string& method_name, std::vector<Variable*>& args);

            /**
             * \brief Invoke the function with 0 arguments.
             * \param method_name The name of a method that you want call.
             * \param args Arguments.
             * \return \ref MM::Reflection::Variable containing the return value of this
             * function.
             * \remark If the number or type of incoming argument is different
             * from the argument required by the function held by this object or the target method not exist, the
             * function held by this object will not be called and return an empty \ref
             * MM::Reflection::Variable.
             */
            Variable Invoke(const std::string& method_name, std::vector<Variable*>&& args);

            /**
             * \brief Returns a pointer to the managed object and releases the ownership.
             * \return The pointer to the managed object.
             */
            void* ReleaseOwnership();

            /**
             * \brief Destroy this object.
             */
            void Destroy();

            template <typename VariableType, typename... Args>
            struct GetVariable
            {
                Variable operator()(Args... args)
                {
                    return Variable{std::make_unique<VariableWrapperBase>(std::forward<Args>(args)...)};
                }
            };

        private:
            std::unique_ptr<VariableWrapperBase> variable_wrapper_ = nullptr;
        };
    } // namespace Reflection
} // namespace MM
