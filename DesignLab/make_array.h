
//! @file make_array.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_MAKE_ARRAY_H_
#define DESIGNLAB_MAKE_ARRAY_H_

#include <array>

namespace designlab::nostd {

//! @brief std::arrayを作成する関数．
//! @n この関数を作成したモチベーションとしては，
//! std::arrayを constexprで初期化する際に苦戦したため．
//! @n この関数を使うことで，std::arrayを constexprで初期化することができる．
//! @n 例えば，std::array<int, 3>を作成する場合は，
//! MakeArray<int>(1, 2, 3)とする．
template <typename T, typename... Args>
constexpr std::array<T, sizeof...(Args)> make_array(Args&&... args) {
  return std::array<T, sizeof...(Args)>{static_cast<Args&&>(args)...};
}

}  // namespace designlab::nostd

#endif  // DESIGNLAB_MAKE_ARRAY_H_
