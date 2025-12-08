//! @file my_expected_concept.h

#pragma once

#include <concepts>
#include <type_traits>

#include "my_unexpected.h"

namespace gaitgen::nostd::impl {

// unexpected かどうかを判定する trait
template <class T>
struct is_unexpected : std::false_type {};

template <class E>
struct is_unexpected<unexpected<E>> : std::true_type {};

template <class T>
inline constexpr bool is_unexpected_v = is_unexpected<T>::value;

//! @brief expected の concept
//! 型 T は参照型ではなく, 関数型でもなく,
<<<<<<< HEAD
//! unexpected<> の特殊化でもなく, in_place_t でもないこと
=======
//! unexpectedの特殊化でなく,in_place_tの特殊化でもないことを要求する.
>>>>>>> b3b30736d78eb3de877d40a43c264a3b0b4186e8
template <typename T>
concept IsExpected = !std::is_reference_v<T> && !std::is_function_v<T> &&
                     !std::is_same_v<std::remove_cvref_t<T>, std::in_place_t> &&
                     !is_unexpected_v<std::remove_cvref_t<T>>;

}  // namespace gaitgen::nostd::impl
