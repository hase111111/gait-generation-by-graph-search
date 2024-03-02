
//! @file      array_util.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_ARRAY_UTIL_H_
#define DESIGNLAB_ARRAY_UTIL_H_

#include <array>


namespace designlab
{

//! @brief std::arrayを作成する関数．
//! @n この関数を作成したモチベーションとしては，
//! std::arrayを constexprで初期化する際に苦戦したため．
//! @n この関数を使うことで，std::arrayを constexprで初期化することができる．
//! @n 例えば，std::array<int, 3>を作成する場合は，MakeArray<int>(1, 2, 3)とする．
template<typename T, typename ...Args>
constexpr std::array<T, sizeof...(Args)> MakeArray(Args&&... args)
{
    return std::array<T, sizeof...(Args)>{ static_cast<Args&&>(args)... };
}

}  // namespace designlab


#endif  // DESIGNLAB_ARRAY_UTIL_H_
