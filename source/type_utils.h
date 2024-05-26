#pragma once

#include <cstdint>
#include <type_traits>

namespace MM {
namespace Utils {
template <bool Condition, typename True, typename False>
struct IfThenElse;

template <typename True, typename False>
struct IfThenElse<true, True, False> {
  using Type = True;
};

template <typename True, typename False>
struct IfThenElse<false, True, False> {
  using Type = False;
};

template <bool Condition, typename True, typename False>
using IfThenElseT = typename IfThenElse<Condition, True, False>::Type;

template <bool Condition1, bool Condition2, typename T>
struct IsOr {};

template <typename T>
struct IsOr<true, true, T> {
  using Type = T;
};

template <typename T>
struct IsOr<true, false, T> {
  using Type = T;
};

template <typename T>
struct IsOr<false, true, T> {
  using Type = T;
};

template <bool Condition1, bool Condition2, typename T>
struct IsAnd {};

template <typename T>
struct IsAnd<true, true, T> {
  using Type = T;
};

template <bool Condition1, bool Condition2, typename T>
struct IsXor {};

template <typename T>
struct IsXor<true, false, T> {
  using Type = T;
};

template <typename T>
struct IsXor<false, true, T> {
  using Type = T;
};

template <bool Condition1, bool Condition2>
using IsOrT = typename IsOr<Condition1, Condition2, void>::Type;

template <bool Condition1, bool Condition2>
using IsAndT = typename IsAnd<Condition1, Condition2, void>::Type;

template <bool Condition1, bool Condition2>
using IsXorT = typename IsXor<Condition1, Condition2, void>::Type;

template <typename T1, typename T2, typename... Args>
struct IsAllSame;

template <typename T>
struct IsAllSame<T, T> {
  static constexpr bool Value = true;
};

template <typename T1, typename T2>
struct IsAllSame<T1, T2> {
  static constexpr bool Value = false;
};

template <typename T1, typename T2, typename... Args>
struct IsAllSame {
  static constexpr bool Value =
      IsAllSame<T1, T2>::Value && IsAllSame<T2, Args...>::Value;
  ;
};

template <typename T1, typename T2, typename... Args>
constexpr bool IsAllSameV = IsAllSame<T1, T2, Args...>::Value;

template <typename... BoolTypes>
bool CheckAllTrue(bool arg1, BoolTypes... args) {
  static_assert(IsAllSameV<bool, BoolTypes...>);
  return arg1 && (args && ...);
}

template <typename T, typename U>
class Conversion {
 private:
  static char Test(U);
  static int Test(...);
  static T MakeT();

 public:
  enum { value = sizeof(Test(MakeT())) == sizeof(char) };
};

template<typename Func>
struct FunctionSignature;

template<typename ReturnType_, typename... Args_>
struct FunctionSignature<ReturnType_(*)(Args_...)> {
  using ReturnType = ReturnType_;
  using Args = std::tuple<Args_...>;
  constexpr static std::uint32_t ArgsNum = sizeof...(Args_);
  constexpr static bool IsStatic = true;
  constexpr static bool IsConst = false;
  constexpr static bool IsVolatile = false;
};

template<typename ReturnType_, typename InstanceType_, typename... Args_>
struct FunctionSignature<ReturnType_(InstanceType_::*)(Args_...)> {
  using ReturnType = ReturnType_;
  using InstanceType = InstanceType_;
  using Args = std::tuple<Args_...>;
  constexpr static std::uint32_t ArgsNum = sizeof...(Args_);
  constexpr static bool IsStatic = false;
  constexpr static bool IsConst = false;
  constexpr static bool IsVolatile = false;
};

template<typename ReturnType_, typename InstanceType_, typename... Args_>
struct FunctionSignature<ReturnType_(InstanceType_::*)(Args_...) const> {
  using ReturnType = ReturnType_;
  using InstanceType = InstanceType_;
  using Args = std::tuple<Args_...>;
  constexpr static std::uint32_t ArgsNum = sizeof...(Args_);
  constexpr static bool IsStatic = false;
  constexpr static bool IsConst = true;
  constexpr static bool IsVolatile = false;
};

template<typename ReturnType_, typename InstanceType_, typename... Args_>
struct FunctionSignature<ReturnType_(InstanceType_::*)(Args_...) volatile> {
  using ReturnType = ReturnType_;
  using InstanceType = InstanceType_;
  using Args = std::tuple<Args_...>;
  constexpr static std::uint32_t ArgsNum = sizeof...(Args_);
  constexpr static bool IsStatic = false;
  constexpr static bool IsConst = false;
  constexpr static bool IsVolatile = true;
};

template<typename ReturnType_, typename InstanceType_, typename... Args_>
struct FunctionSignature<ReturnType_(InstanceType_::*)(Args_...) const volatile> {
  using ReturnType = ReturnType_;
  using InstanceType = InstanceType_;
  using Args = std::tuple<Args_...>;
  constexpr static std::uint32_t ArgsNum = sizeof...(Args_);
  constexpr static bool IsStatic = false;
  constexpr static bool IsConst = true;
  constexpr static bool IsVolatile = true;
};

template<typename Sig>
struct FunctionSignature : public FunctionSignature<decltype(&Sig::operator())> {};

template <typename T>
struct PropertyMetaData;

template <typename ClassType_, typename PropertyType_>
struct PropertyMetaData<PropertyType_ ClassType_::*> {
  using ClassType = ClassType_;
  using PropertyType = PropertyType_;
  constexpr static std::uint32_t size = sizeof(PropertyType_);
  constexpr static bool IsStatic = false;
  constexpr static bool IsRefrence = false;
};

template <typename PropertyType_>
struct PropertyMetaData<PropertyType_ *> {
  using ClassType = void;
  using PropertyType = PropertyType_;
  constexpr static std::uint32_t size = sizeof(PropertyType_);
  constexpr static bool IsStatic = true;
  constexpr static bool IsRefrence = false;
};

template <typename PropertyType_>
struct PropertyMetaData<PropertyType_&> {
  using ClassType = void;
  using PropertyType = PropertyType_&;
  constexpr static std::uint32_t size = sizeof(PropertyType_&);
  constexpr static bool IsStatic = true;
  constexpr static bool IsRefrence = true;
};

// template <typename PropertyType_>
// struct PropertyMetaData<PropertyType_**> {
//   using ClassType = void;
//   using PropertyType = PropertyType_&;
//   constexpr static std::uint32_t size = sizeof(PropertyType_&);
//   constexpr static bool IsStatic = true;
//   constexpr static bool IsRefrence = true;
// };

template <typename Fun>
bool IsFunction = std::is_function_v<Fun>;

template <typename Fun>
bool IsMemberFunction = std::is_member_function_pointer_v<std::decay_t<Fun>>;

template <typename Fun>
bool IsClass = std::is_class_v<std::decay_t<Fun>>;

template <class T, T... I>
struct IntegerSequence {
  template <T N>
  using Append = IntegerSequence<T, I..., N>;
  static std::size_t size() { return sizeof...(I); }
  using Next = Append<sizeof...(I)>;
  using Type = T;
};

template <class T, T Index, std::size_t N>
struct SequenceGenerator {
  using Type = typename SequenceGenerator<T, Index - 1, N - 1>::Type::Next;
};

template <class T, T Index>
struct SequenceGenerator<T, Index, 0ul> {
  using Type = IntegerSequence<T>;
};

template <std::size_t... I>
using IndexSequence = IntegerSequence<std::size_t, I...>;

template <class T, T N>
using MakeIntegerSequence = typename SequenceGenerator<T, N, N>::Type;

template <std::size_t N>
using MakeIndexSequence = MakeIntegerSequence<std::size_t, N>;

template <class... T>
using IndexSequenceFor = MakeIndexSequence<sizeof...(T)>;

template <typename FirstType, typename... Types>
struct AllTypeHashCode {
  static std::size_t HashCode();
};

template <typename FirstType>
struct AllTypeHashCode<FirstType> {
  static std::size_t HashCode();
};

template <typename FirstType, typename... Types>
std::size_t AllTypeHashCode<FirstType, Types...>::HashCode() {
  return typeid(FirstType).hash_code() +
         (AllTypeHashCode<Types...>::HashCode() << 1);
}

template <typename FirstType>
std::size_t AllTypeHashCode<FirstType>::HashCode() {
  return typeid(FirstType).hash_code();
}

 /**
 * \brief A destructor that does nothing. Prevent the deletion of data by
 * variable reference obtained through \ref GetProperty(const Variable&
 * class_variable) from causing access errors.
 */
template<typename T>
struct DoNothingDestructor {
  void operator()(T*) {}
};

/**
 * \brief Get the original type of \ref TypeName (no reference, const, pointer,
 * etc.); \tparam TypeName You want to get the type of its original type.
 */
template <typename TypeName>
struct GetOriginalType {
  using Type = std::remove_const_t<std::remove_pointer_t<
      std::remove_reference_t<std::remove_all_extents_t<TypeName>>>>;
};

template <typename TypeName>
using GetOriginalTypeT = typename GetOriginalType<TypeName>::Type;

template <typename T> struct GetCommonType {
  using Type = std::remove_reference_t<std::remove_const_t<T>>;
};

template <typename PointType> struct GetCommonType<const PointType *> {
  using Type = std::remove_const_t<PointType *>;
};

template <typename PointType> struct GetCommonType<const PointType *const> {
  using Type = std::remove_const_t<PointType *>;
};

template <typename T> using GetCommonTypeT = typename GetCommonType<T>::Type;

template<typename ReturnType, typename InstanceType, bool IsStatic, bool IsConst, bool IsVolatile, typename ...Args>
struct GetFunctionPointType {
  using Type = ReturnType(InstanceType::*)(Args...);
};

template<typename ReturnType, typename InstanceType, typename ...Args>
struct GetFunctionPointType<ReturnType, InstanceType, true, false, false, Args...> {
  using Type = ReturnType(*)(Args...);
};

template<typename ReturnType, typename InstanceType, typename ...Args>
struct GetFunctionPointType<ReturnType, InstanceType, false, true, false, Args...> {
  using Type = ReturnType(InstanceType::*)(Args...) const;
};

template<typename ReturnType, typename InstanceType, typename ...Args>
struct GetFunctionPointType<ReturnType, InstanceType, false, true, true, Args...> {
  using Type = ReturnType(InstanceType::*)(Args...) volatile const ;
};

template<typename ReturnType, typename InstanceType, typename ...Args>
struct GetFunctionPointType<ReturnType, InstanceType, false, false, true, Args...> {
  using Type = ReturnType(InstanceType::*)(Args...) volatile;
};

template<typename ReturnType, typename InstanceType, bool IsStatic, bool IsConst, bool IsVolatile,typename ...Args>
using GetFunctionPointTypeT =
    typename GetFunctionPointType<ReturnType, InstanceType, IsStatic, IsConst, IsVolatile, Args...>::Type;

template<typename T>
struct GetNotConstRefrence {
  using Type = T;
};

template<typename T>
struct GetNotConstRefrence<T&>
{
  using Type = T&;
};

template<typename T>
struct GetNotConstRefrence<const T&>
{
  using Type = T&;
};

template<typename T>
using GetNotConstRefrenceT = typename GetNotConstRefrence<T>::Type;


template<typename T>
struct GetFunctionArgType{
  using Type = T;
};

template<typename T>
struct GetFunctionArgType<const T>
{
  using Type = T;
};

template<typename T>
struct GetFunctionArgType<const T&>
{
  using Type = const T&;
};

template<typename T>
using GetFunctionArgTypeT = typename GetFunctionArgType<T>::Type;

template <typename T> struct IsConst {
  static constexpr bool Value = false;
};

template <typename T> struct IsConst<const T> {
  static constexpr bool Value = true;
};

template <typename T> struct IsConst<const T &> {
  static constexpr bool Value = true;
};

template <typename T> constexpr bool IsConstV = IsConst<T>::Value;
}  // namespace Utils

struct Nil {};
}  // namespace MM
