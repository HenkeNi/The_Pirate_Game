#pragma once
#include <type_traits>

// Integral type (ex: int, long, short)
template <typename T>
concept Integral = std::is_integral_v<T>;

// Floating point type (ex: float, double, long double)
template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

// Numeric (ex: int, short, float, double)
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

// Signed
template <typename T>
concept Signed = std::is_signed_v<T>;

// Signed numeric
template <typename T>
concept SignedNumeric = Numeric<T> && Signed<T>;

// Unsigned
template <typename T>
concept Unsigned = std::is_unsigned_v<T>;

template <typename T>
concept UnsignedIntegral = std::is_integral_v<T> && Unsigned<T>;


// Pointer
template <typename T>
concept Pointer = std::is_pointer_v<T>;

// Lvalue reference
template <typename T>
concept LvalueReference = std::is_lvalue_reference_v<T>;

// Rvalue reference
template <typename T>
concept RvalueReference = std::is_rvalue_reference_v<T>;

// Callable (i.e., can be called with Args...)  // rename inovacble
template <typename T, typename... Args>
concept Callable = std::is_invocable_v<T, Args...>;

template <typename Func, typename Return>
concept Returns = requires(Func f)
{
    { f() } -> std::same_as<Return>;
};

template <typename Func, typename Return, typename... Args>
concept CallableReturns = std::invocable<Func, Args...>&& std::same_as<std::invoke_result_t<Func, Args...>, Return>;


//template <typename Func, typename Return>
//concept Returns = requires(Func f)
//{
//    { f() } -> std::same_as<Return>;
//};
//
//template <typename Func, typename Return, typename... Args>
//concept CallableReturns = std::invocable<Func, Args...>&& std::same_as<std::invoke_result_t<Func, Args...>, Return>;
//
//// Same type (checks if two types are the same)
template <typename T, typename U>
concept SameType = std::is_same_v<T, U>;

// Derived from
template <typename T, typename Base>
concept DerivedFrom = std::is_base_of_v<Base, T> && !std::is_same_v<Base, T>; // decay?

template <typename T>
concept ComponentType = std::is_class_v<T>; // TODO; improve!
