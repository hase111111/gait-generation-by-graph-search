
//! @file expected.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <functional>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>

#include "my_expected_concept.h"
#include "my_unexpected.h"

namespace gaitgen::nostd {

// 自作の例外 bad_expected_access
class bad_expected_access : public std::exception {
 public:
  explicit bad_expected_access(const std::string& msg) : message_(msg) {}
  explicit bad_expected_access(const char* msg) : message_(msg) {}

  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};

template <class T, class W>
constexpr bool converts_from_any_cvref = std::disjunction_v<
    std::is_constructible<T, W&>, std::is_convertible<W&, T>,
    std::is_constructible<T, W>, std::is_convertible<W, T>,
    std::is_constructible<T, const W&>, std::is_convertible<const W&, T>,
    std::is_constructible<T, const W>, std::is_convertible<const W, T>>;

//! @brief 自作の expected クラス
//! @tparam T 型 T は成功時の値の型
//! @tparam E 型 E は失敗時の値の型
template <impl::IsExpected T, typename E>
class expected final {
 public:
  using value_type = T;
  using error_type = E;
  using unexpected_type = unexpected<E>;

  //! @brief デフォルトコンストラクタ
  //! @details T はデフォルトコンストラクタが必要
  constexpr expected()
    requires std::is_default_constructible_v<T>
      : storage_(T{}), has_value_(true) {}

  constexpr expected(const expected& rhs)
      : storage_(rhs.storage_), has_value_(rhs.has_value_) {}

  constexpr expected(expected&& rhs) noexcept
    requires(std::is_move_constructible_v<T> && std::is_move_constructible_v<E>)
      : storage_(std::move(rhs.storage_)), has_value_(rhs.has_value_) {}

  template <class U, class G>
  constexpr expected(const expected<U, G>& rhs)
    requires(std::is_constructible_v<T, const U&> &&
             std::is_constructible_v<E, const G&> &&
             !converts_from_any_cvref<T, expected<U, G>&> &&
             !std::is_constructible_v<unexpected<E>, expected<U, G>&> &&
             !std::is_constructible_v<unexpected<E>, expected<U, G>> &&
             !std::is_constructible_v<unexpected<E>, const expected<U, G>&> &&
             !std::is_constructible_v<unexpected<E>, const expected<U, G>>)
      : storage_(rhs.storage_), has_value_(rhs.has_value_) {}

  template <class U, class G>
  constexpr expected(const expected<U, G>&& rhs) noexcept
    requires(std::is_constructible_v<T, U> && std::is_constructible_v<E, G> &&
             !converts_from_any_cvref<T, expected<U, G>> &&
             !std::is_constructible_v<unexpected<E>, expected<U, G>&> &&
             !std::is_constructible_v<unexpected<E>, expected<U, G>> &&
             !std::is_constructible_v<unexpected<E>, const expected<U, G>&> &&
             !std::is_constructible_v<unexpected<E>, const expected<U, G>>)
      : storage_(std::move(rhs.storage_)), has_value_(rhs.has_value_) {}

  template <class U = T>
  constexpr expected(U&& v)
    requires(std::is_constructible_v<T, U> &&
             !std::is_same_v<expected, std::remove_cvref_t<U>> &&
             !std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
      : storage_(v), has_value_(true) {}

  template <class G>
  constexpr expected(const unexpected<G>& e)
<<<<<<< HEAD
      : storage_(unexpected<E>{e.error()}), has_value_(false) {}
=======
      : storage_{unexpected<E>{e.error()}}, has_value_(false) {}
>>>>>>> b3b30736d78eb3de877d40a43c264a3b0b4186e8

  constexpr ~expected() = default;  //!< デストラクタ

  //! @brief 正常値のポインタを返す.
  //! @details 正常値を持ってない場合は未定義動作になるので注意.
  constexpr const T* operator->() const noexcept {
    return std::addressof(std::get<T>(storage_));
  }

  //! @brief 正常値のポインタを返す.
  //! @details 正常値を持ってない場合は未定義動作になるので注意.
  constexpr T* operator->() noexcept {
    return std::addressof(std::get<T>(storage_));
  }

  //! @brief 正常値を参照する.
  //! @details 正常値を持ってない場合は未定義動作になるので注意.
  constexpr const T& operator*() const& noexcept {
    return std::get<T>(storage_);
  }

  //! @brief 正常値を参照する.
  //! @details 正常値を持ってない場合は未定義動作になるので注意.
  constexpr T& operator*() & noexcept { return std::get<T>(storage_); }

  constexpr explicit operator bool() const noexcept { return has_value_; }

  //! @brief 正常値を持っている場合に true を返す.
  constexpr bool has_value() const noexcept { return has_value_; }

  //! @brief 正常値を返す.
  //! @details has_value() が true のときのみ呼び出すこと.
  //! @throw bad_expected_access
  constexpr const T& value() const {
    if (!has_value_) throw bad_expected_access("no value present");
    return std::get<T>(storage_);
  }

  //! @brief 正常値を返す.
  //! @details has_value() が true のときのみ呼び出すこと.
  //! @throw bad_expected_access
  constexpr T& value() {
    if (!has_value_) throw bad_expected_access("no value present");
    return std::get<T>(storage_);
  }

  //! @brief 失敗時の値を返す.
  //! @details has_value() が false のときのみ呼び出すこと.
  //! @throw bad_expected_access
  constexpr const E& error() const {
    if (has_value_) throw bad_expected_access("no error present");
    auto& unexpected_value = std::get<unexpected<E>>(storage_);
    return unexpected_value.error();
  }

  //! @brief 失敗時の値を返す.
  //! @details has_value() が false のときのみ呼び出すこと.
  //! @throw bad_expected_access
  constexpr E& error() {
    if (has_value_) throw bad_expected_access("no error present");
    auto& unexpected_value = std::get<unexpected<E>>(storage_);
    return unexpected_value.error();
  }

  template <class U>
  constexpr T value_or(U&& v) const&
    requires(std::is_constructible_v<T, U> && std::is_copy_constructible_v<T>)
  {
    return has_value() ? **this : static_cast<T>(std::forward<U>(v));
  }

  template <class G = E>
  constexpr E error_or(G&& e) const&
    requires(std::is_constructible_v<E, G> && std::is_copy_constructible_v<E>)
  {
    return has_value() ? std::forward<G>(e) : error();
  }

  // Monadic operations.
  template <class Return>
  constexpr expected<Return, E> and_then(
      const std::function<expected<Return, E>(T)>& func) const& {
    using ReturnType = expected<Return, E>;
    if (has_value_) {
      return ReturnType{func(value())};
    } else {
      return ReturnType{unexpected(error())};
    }
  }

  template <class Func, class Ret = std::invoke_result_t<Func, T>>
  constexpr auto and_then(Func&& func) const&
    requires(std::is_invocable_r_v<Ret, Func, T> &&
             std::is_same_v<typename Ret::error_type, E> &&
             std::is_same_v<std::remove_cvref_t<Ret>,
                            expected<typename Ret::value_type, E>>)
  {
    using ReturnType = std::invoke_result_t<Func, T>;
    if (has_value_) {
      return ReturnType{func(value())};
    } else {
      return ReturnType{unexpected(error())};
    }
  }

 private:
  std::variant<T, unexpected<E>> storage_;
  bool has_value_;
};

}  // namespace gaitgen::nostd
