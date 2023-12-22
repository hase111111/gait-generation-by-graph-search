#ifndef DESIGNLAB_IMPLICIT_METAFUNCTION_FOR_MATH_UTIL_H_
#define DESIGNLAB_IMPLICIT_METAFUNCTION_FOR_MATH_UTIL_H_

#include <type_traits>
#include <vector>

namespace designlab::impl
{

//! @brief float型またはdouble型かを判定するメタ関数．
//! @tparam T 判定する型．
//! @details float型でもdouble型でもない場合はfalse_typeを継承する．
template <typename T>
struct is_float_or_double : std::false_type {};

//! @brief float型またはdouble型かを判定するメタ関数．
//! @details float型の場合はtrue_typeを継承する．
template <>
struct is_float_or_double<float> : std::true_type {};

//! @brief float型またはdouble型かを判定するメタ関数．
//! @details double型の場合はtrue_typeを継承する．
template <>
struct is_float_or_double<double> : std::true_type {};



// テスト
static_assert(is_float_or_double<int>::value == false, "int型はfloatでもdoubleでもありません．");
static_assert(is_float_or_double<std::vector<float>>::value == false, "std::vector<float>型はfloatでもdoubleでもありません．");
static_assert(is_float_or_double<std::vector<double>>::value == false, "std::vector<double>型はfloatでもdoubleでもありません．");

static_assert(is_float_or_double<float>::value == true, "float型の場合，trueです．");
static_assert(is_float_or_double<double>::value == true, "double型の場合，trueです．");

}	// namespace designlab::impl


#endif	// DESIGNLAB_IMPLICIT_METAFUNCTION_FOR_MATH_UTIL_H_