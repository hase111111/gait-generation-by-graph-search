
//! @file      implicit_metafunction_for_math_util.h
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_IMPLICIT_METAFUNCTION_FOR_MATH_UTIL_H_
#define DESIGNLAB_IMPLICIT_METAFUNCTION_FOR_MATH_UTIL_H_

#include <type_traits>
#include <vector>

namespace designlab::impl
{

//! @brief float型または double型かを判定するメタ関数．
//! @tparam T 判定する型．
//! @details float型でも double型でもない場合は false_typeを継承する．
template <typename T>
struct is_float_or_double : std::false_type {};

//! @brief float型または double型かを判定するメタ関数．
//! @details float型の場合は true_typeを継承する．
template <>
struct is_float_or_double<float> : std::true_type {};

//! @brief float型または double型かを判定するメタ関数．
//! @details double型の場合は true_typeを継承する．
template <>
struct is_float_or_double<double> : std::true_type {};



// テスト．
static_assert(is_float_or_double<int>::value == false,
              "int型は floatでも doubleでもありません．");
static_assert(is_float_or_double<std::vector<float>>::value == false,
              "std::vector<float>型は floatでも doubleでもありません．");
static_assert(is_float_or_double<std::vector<double>>::value == false,
              "std::vector<double>型は floatでも doubleでもありません．");

static_assert(is_float_or_double<float>::value == true, "float型の場合，trueです．");
static_assert(is_float_or_double<double>::value == true, "double型の場合，trueです．");

}  // namespace designlab::impl


#endif  // DESIGNLAB_IMPLICIT_METAFUNCTION_FOR_MATH_UTIL_H_
