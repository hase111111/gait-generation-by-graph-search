
//! @file unexpected.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace gaitgen::nostd {

template <typename E>
class unexpected final {
 public:
  // テンプレートパラメータ制約

  template <typename Err>
  static constexpr bool param_concept3_1 =
      !std::is_same_v<std::remove_cvref_t<Err>, unexpected>;

  template <typename Err>
  static constexpr bool param_concept3_2 =
      !std::is_same_v<std::remove_cvref_t<Err>, std::in_place_t>;

  template <typename Err>
  static constexpr bool param_concept3_3 = std::is_constructible_v<E, Err>;

  template <typename Err>
  static constexpr bool param_concept3 =
      param_concept3_1<Err> && param_concept3_2<Err> && param_concept3_3<Err>;

  template <typename... Args>
  static constexpr bool param_concept4 = std::is_constructible_v<E, Args...>;

  template <typename U, typename... Args>
  static constexpr bool param_concept5 =
      std::is_constructible_v<E, std::initializer_list<U>, Args...>;

  // コンストラクタ

  constexpr unexpected(const unexpected&) = default;  //!< コピーコンストラクタ
  constexpr unexpected(unexpected&&) = default;       //!< ムーブコンストラクタ

  template <typename Err = E,
            std::enable_if_t<param_concept3<Err>, std::nullptr_t> = nullptr>
  constexpr explicit unexpected(const Err& e) : error_(e) {}

  template <typename... Args,
            std::enable_if_t<param_concept4<Args...>, std::nullptr_t> = nullptr>
  constexpr explicit unexpected(std::in_place_t, Args&&... args)
      : error_(std::forward<Args>(args)...) {}

  template <
      typename U, typename... Args,
      std::enable_if_t<param_concept5<U, Args...>, std::nullptr_t> = nullptr>
  constexpr explicit unexpected(std::in_place_t, std::initializer_list<U> il,
                                Args&&... args)
      : error_(il, std::forward<Args>(args)...) {}

  constexpr unexpected& operator=(const unexpected&) = default;
  constexpr unexpected& operator=(unexpected&&) = default;

  constexpr ~unexpected() = default;

  // 比較演算子(friend関数で定義)
  template <typename E2>
  friend constexpr bool operator==(const unexpected<E2>& lhs,
                                   const unexpected<E2>& rhs);
  template <typename E2>
  friend constexpr bool operator!=(const unexpected<E2>& lhs,
                                   const unexpected<E2>& rhs);

  constexpr const E& error() const& noexcept { return error_; }
  constexpr E& error() & noexcept { return error_; }
  constexpr E&& error() && noexcept { return std::move(error_); }

 private:
  E error_;
};

// ヘルパー関数（推論支援）
template <typename E>
constexpr unexpected<std::decay_t<E>> make_unexpected(E&& e) {
  return unexpected<std::decay_t<E>>(std::forward<E>(e));
}

// 型推論のためのエイリアス
template <class E>
unexpected(E) -> unexpected<E>;

template <typename E>
constexpr bool operator==(const unexpected<E>& lhs, const unexpected<E>& rhs) {
  return lhs.error() == rhs.error();
}

template <typename E>
constexpr bool operator!=(const unexpected<E>& lhs, const unexpected<E>& rhs) {
  return !(lhs == rhs);
}

// constexprでのテスト
static_assert(unexpected<int>{0}.error() == 0,
              "unexpected<int>のerror()が正しく動作しません。");

static_assert(unexpected<std::string>{"error"}.error() == "error",
              "unexpected<std::string>のerror()が正しく動作しません。");

}  // namespace gaitgen::nostd
