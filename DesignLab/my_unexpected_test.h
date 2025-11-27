
//! @file my_unexpected_test.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <doctest.h>

#include <string>

#include "my_unexpected.h"

TEST_SUITE("unexpected") {
  using designlab::nostd::unexpected;

  TEST_CASE("unexpected construction") {
    // 単一の変数によるコンストラクタ

    SUBCASE("when E is a integral type, it should be constructible") {
      unexpected<int> u(42);
      CHECK_EQ(u.error(), 42);
    }

    SUBCASE("when E is a string, it should be constructible") {
      unexpected<std::string> u("error");
      CHECK_EQ(u.error(), "error");
    }

    SUBCASE("when E is a custom type, it should be constructible") {
      struct CustomError {
        int code;
        std::string message;
      };
      unexpected<CustomError> u(CustomError{404, "Not Found"});
      CHECK_EQ(u.error().code, 404);
      CHECK_EQ(u.error().message, "Not Found");
    }
  }

  // 任意個の引数を取るコンストラクタ
  TEST_CASE("unexpected with in_place_t") {
    struct CustomError {
      int code;
      std::string message;
    };

    unexpected<CustomError> u(std::in_place, 500, "Internal Server Error");
    CHECK_EQ(u.error().code, 500);
    CHECK_EQ(u.error().message, "Internal Server Error");
  }

  TEST_CASE("unexpected copy construction") {
    unexpected<int> u1(42);
    unexpected<int> u2 = u1;  // copy
    CHECK_EQ(u2.error(), 42);
  }

  TEST_CASE("unexpected move construction") {
    unexpected<int> u1(42);
    unexpected<int> u2 = std::move(u1);  // move
    CHECK_EQ(u2.error(), 42);
  }

  TEST_CASE("unexpected assignment") {
    unexpected<int> u1(42);
    unexpected<int> u2(100);
    u2 = u1;  // copy assignment
    CHECK_EQ(u2.error(), 42);

    unexpected<int> u3(200);
    u3 = std::move(u1);  // move assignment
    CHECK_EQ(u3.error(), 42);
  }

  TEST_CASE("unexpected equality comparison") {
    unexpected<int> u1(42);
    unexpected<int> u2(42);
    unexpected<int> u3(100);

    CHECK(u1 == u2);        // should be equal
    CHECK_FALSE(u1 == u3);  // should not be equal
    CHECK_FALSE(u2 == u3);  // should not be equal
  }

  TEST_CASE("unexpected inequality comparison") {
    unexpected<int> u1(42);
    unexpected<int> u2(42);
    unexpected<int> u3(100);

    CHECK_FALSE(u1 != u2);  // should not be unequal
    CHECK(u1 != u3);        // should be unequal
    CHECK(u2 != u3);        // should be unequal
  }

  TEST_CASE("unexpected error access") {
    unexpected<std::string> u("error");
    CHECK_EQ(u.error(), "error");

    // エラーを参照する
    std::string& err_ref = u.error();
    err_ref = "new error";
    CHECK_EQ(u.error(), "new error");

    // ムーブしてもエラーは保持される
    unexpected<std::string> u2 = std::move(u);
    CHECK_EQ(u2.error(), "new error");
  }
}
