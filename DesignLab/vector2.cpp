
//! @file math_vector2.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "vector2.h"

#include <format>

namespace designlab {

Vector2& Vector2::operator+=(const Vector2& other) {
  x += other.x;
  y += other.y;
  return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

Vector2& Vector2::operator*=(const float s) {
  x *= s;
  y *= s;
  return *this;
}

Vector2& Vector2::operator/=(const float s) {
  x /= s;
  y /= s;
  return *this;
}

Vector2 Vector2::GetNormalized() const {
  const float length = GetLength();

  if (math_util::IsEqual(length, 0.f)) {
    return {0, 0};
  }

  return *this * (1.f / length);
}

std::string Vector2::ToString() const {
  return std::format("( x : {}, y : {} )",
                     math_util::FloatingPointNumToString(x),
                     math_util::FloatingPointNumToString(y));
}

std::string Vector2::ToCsvString() const {
  return std::format("{},{}", math_util::FloatingPointNumToString(x),
                     math_util::FloatingPointNumToString(y));
}

}  // namespace designlab
