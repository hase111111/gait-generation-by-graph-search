
//! @file my_expected_test.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <doctest.h>

#include <string>

#include "my_expected.h"

namespace designlab::impl::my_expected_test {

struct CustomValue final {
  int code;
  std::string message;
};

struct Sample final {
  void count() { cnt++; }
  int value() const { return 42; }

  int cnt = 0;
};

std::string ToFizzBuzz(int value) {
  if (value % 3 == 0 && value % 5 == 0) {
    return "FizzBuzz";
  } else if (value % 3 == 0) {
    return "Fizz";
  } else if (value % 5 == 0) {
    return "Buzz";
  } else {
    return std::to_string(value);
  }
}

using ess = designlab::nostd::expected<std::string, std::string>;
using ues = designlab::nostd::unexpected<std::string>;

ess FizzBuzz(int value) {
  if (value < 0) {
    return ues("Negative value");
  }
  return ess(ToFizzBuzz(value));
}

ess FizzBuzzRef(const int& value) {
  if (value < 0) {
    return ues("Negative value");
  }
  return ToFizzBuzz(value);
}

}  // namespace designlab::impl::my_expected_test

TEST_SUITE("expected") {
  using designlab::impl::my_expected_test::CustomValue;
  using designlab::impl::my_expected_test::FizzBuzz;
  using designlab::impl::my_expected_test::FizzBuzzRef;
  using designlab::impl::my_expected_test::Sample;
  using designlab::nostd::bad_expected_access;
  using designlab::nostd::expected;
  using designlab::nostd::unexpected;

  TEST_CASE("expected construction") {
    // 単一の変数によるコンストラクタ

    SUBCASE("when T is a integral type, it should be constructible") {
      expected<int, std::string> e(42);

      CHECK_EQ(e.value(), 42);
      CHECK(e.has_value());
    }

    SUBCASE("when T is a string, it should be constructible") {
      expected<std::string, int> e("success");

      CHECK_EQ(e.value(), "success");
      CHECK(e.has_value());
    }

    SUBCASE("when T and E are the same type, it should be constructible") {
      expected<std::string, std::string> e("success");

      CHECK_EQ(e.value(), "success");
      CHECK(e.has_value());
    }

    SUBCASE("when T is a custom type, it should be constructible") {
      expected<CustomValue, std::string> e(CustomValue{200, "OK"});

      CHECK_EQ(e.value().code, 200);
      CHECK_EQ(e.value().message, "OK");
      CHECK(e.has_value());
    }
  }

  TEST_CASE("arrow operator") {
    SUBCASE(
        "when T is a class with member functions, arrow operator should work") {
      // Act1
      expected<Sample, std::string> e;

      CHECK_EQ(e->value(), 42);
      CHECK_EQ(e->cnt, 0);

      // Act2
      e->count();

      CHECK_EQ(e->cnt, 1);
    }
  }

  TEST_CASE("asterisk operator") {
    SUBCASE("when T is a class with member functions, * operator should work") {
      expected<int, std::string> e(42);

      CHECK_EQ(*e, 42);
    }
  }

  TEST_CASE("bool operator") {
    SUBCASE("when has_value is true, bool operator should return true") {
      expected<int, std::string> e(42);
      CHECK(e);
    }

    SUBCASE("when has_value is false, bool operator should return false") {
      expected<int, std::string> e(unexpected<std::string>("error"));
      CHECK(!e);
    }
  }

  TEST_CASE("has_value") {
    SUBCASE("has_value should return true when value is present") {
      expected<int, std::string> e(42);
      CHECK(e.has_value());
    }

    SUBCASE("has_value should return false when error is present") {
      expected<int, std::string> e(unexpected<std::string>("error"));
      CHECK(!e.has_value());
    }
  }

  TEST_CASE("value") {
    SUBCASE("value should return the value when has_value is true") {
      expected<int, std::string> e(42);
      CHECK_EQ(e.value(), 42);
    }

    SUBCASE("value should throw when has_value is false") {
      expected<int, std::string> e(unexpected<std::string>("error"));
      CHECK_THROWS_AS(e.value(), bad_expected_access);
    }
  }

  TEST_CASE("error") {
    SUBCASE("error should return the error when has_value is false") {
      expected<int, std::string> e(unexpected<std::string>("error"));
      CHECK_EQ(e.error(), "error");
    }

    SUBCASE("error should throw when has_value is true") {
      expected<int, std::string> e(42);
      CHECK_THROWS_AS(e.error(), bad_expected_access);
    }
  }

  TEST_CASE("value_or") {
    SUBCASE("value_or should return the value when has_value is true") {
      expected<int, std::string> e(42);
      CHECK_EQ(e.value_or(0), 42);
    }

    SUBCASE(
        "value_or should return the default value when has_value is false") {
      expected<int, std::string> e(unexpected<std::string>("error"));
      CHECK_EQ(e.value_or(0), 0);
    }
  }

  TEST_CASE("error_or") {
    SUBCASE("error_or should return the error when has_value is false") {
      expected<int, std::string> e(unexpected<std::string>("error"));
      CHECK_EQ(e.error_or("default error"), "error");
    }

    SUBCASE("error_or should return the default value when has_value is true") {
      expected<int, std::string> e(42);
      CHECK_EQ(e.error_or("default error"), "default error");
    }
  }

  TEST_CASE("and_then") {
    SUBCASE("and_then should call the function when has_value is true") {
      const auto div2 = [](int value) -> expected<int, std::string> {
        return expected<int, std::string>(value / 2);
      };

      // Act1
      const expected<int, std::string> e1(42);
      const auto result = e1.and_then(div2);

      CHECK(result.has_value());
      CHECK_EQ(result.value(), 21);

      // Act2
      const expected<int, std::string> e2(44);
      const auto result2 = e2.and_then(div2).and_then(div2);

      CHECK(result2.has_value());
      CHECK_EQ(result2.value(), 11);
    }

    SUBCASE("and_then should not call the function when has_value is false") {
      const auto div2 = [](int value) -> expected<int, std::string> {
        return expected<int, std::string>(value / 2);
      };

      // Act1
      const expected<int, std::string> e1(unexpected<std::string>("error"));
      const auto result1 = e1.and_then(div2);

      CHECK(!result1.has_value());
      CHECK_EQ(result1.error(), "error");

      // Act2
      const expected<int, std::string> e2(unexpected<std::string>("error"));
      const auto result2 = e2.and_then(div2).and_then(div2);

      CHECK(!result2.has_value());
      CHECK_EQ(result2.error(), "error");
    }

    SUBCASE("and_then with member function should work") {
      const auto get_value = [](const Sample& s) {
        return expected<int, std::string>(s.value());
      };

      // Act
      expected<Sample, std::string> e(Sample{});
      const auto result = e.and_then(get_value);

      CHECK(result.has_value());
      CHECK_EQ(result.value(), 42);
    }

    SUBCASE("") {
      const expected<int, std::string> e1(42);

      // Act1
      const auto res1 = e1.and_then(FizzBuzz);
      const auto res2 = e1.and_then(FizzBuzzRef);

      CHECK(res1.has_value());
      CHECK_EQ(res1.value(), "Fizz");
      CHECK(res2.has_value());
      CHECK_EQ(res2.value(), "Fizz");
    }
  }
}
