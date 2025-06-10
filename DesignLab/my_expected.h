
//! @file expected.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_MY_EXPECTED_H_
#define DESIGNLAB_MY_EXPECTED_H_

#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>

#include "my_expected_concept.h"
#include "my_unexpected.h"

namespace designlab::nostd {

// 自作の例外 bad_expected_access
template <typename T>
class bad_expected_access : public std::exception {
 public:
  explicit bad_expected_access(const std::string& msg) : message_(msg) {}
  explicit bad_expected_access(const char* msg) : message_(msg) {}

  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};

//! @brief 自作の expected クラス
//! @tparam T 型 T は成功時の値の型
//! @tparam E 型 E は失敗時の値の型
template <impl::IsExpected T, typename E>
class expected {
 public:
  using value_type = T;
  using error_type = E;
  using unexpected_type = unexpected<E>;

  constexpr expected() : storage_(T{}), has_value_(true) {}
  constexpr expected(const expected& rhs)
      : storage_(rhs.storage_), has_value_(rhs.has_value_) {}
  constexpr expected(expected&& rhs) noexcept
      : storage_(std::move(rhs.storage_)), has_value_(rhs.has_value_) {}

  template <class U, class G>
  constexpr expected(const expected<U, G>& rhs)
      : storage_(rhs.storage_), has_value_(rhs.has_value_) {}
  template <class U, class G>
  constexpr expected(const expected<U, G>&& rhs) noexcept
      : storage_(std::move(rhs.storage_)), has_value_(rhs.has_value_) {}

  template <class U = T>
  constexpr expected(U&& v)
    requires(std::is_constructible_v<T, U> &&
             !std::is_same_v<expected, std::remove_cvref_t<U>> &&
             !std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
      : storage_(v), has_value_(true) {}

  template <class G>
  constexpr expected(const unexpected<G>& e)
      : storage_(unexpected(e.error())), has_value_(false) {}

  constexpr ~expected() = default;  //!< デストラクタ

  constexpr bool has_value() const noexcept { return has_value_; }
  constexpr explicit operator bool() const noexcept { return has_value_; }

  constexpr const T& value() const {
    if (!has_value_) throw bad_expected_access<E>(error());
    return std::get<T>(storage_);
  }

  constexpr const E& error() const {
    if (has_value_) throw bad_expected_access<E>(value());
    return std::get<E>(storage_);
  }

 private:
  std::variant<T, unexpected<E>> storage_;
  bool has_value_;
};

}  // namespace designlab::nostd

#endif  // DESIGNLAB_MY_EXPECTED_H_
