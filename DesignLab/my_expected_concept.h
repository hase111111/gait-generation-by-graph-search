
//! @file my_expected_concept.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_MY_EXPECTED_CONCEPT_H_
#define DESIGNLAB_MY_EXPECTED_CONCEPT_H_

#include <concepts>
#include <type_traits>

#include "my_unexpected.h"

namespace designlab::nostd::impl {

// 前方宣言
template <typename T>
class unexpected;

//! @brief expected の concept
//! 型 T は参照型ではなく, 関数型でもなく,
//! unexpectedの特殊化でなく，in_place_tの特殊化でもないことを要求する
template <typename T>
concept IsExpected =
    !std::is_reference_v<T> && !std::is_function_v<T> &&
    !std::is_same_v<std::remove_cvref_t<T>, std::in_place_t> &&
    !std::is_same_v<std::remove_cvref_t<T>, unexpected<std::remove_cvref_t<T>>>;

}  // namespace designlab::nostd::impl

#endif  // DESIGNLAB_MY_EXPECTED_CONCEPT_H_
