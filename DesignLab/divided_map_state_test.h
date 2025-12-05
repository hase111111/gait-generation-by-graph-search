
//! @file divided_map_state_test.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <vector>

#include "divided_map_state.h"
#include "doctest.h"
#include "interface_map_creator.h"

namespace divided_map_state_test_internal {

using gaitgen::DividedMapState;
using gaitgen::IMapCreator;
using gaitgen::MapState;
using gaitgen::Vector3;

class TestMapCreator final : public IMapCreator {
 public:
  static constexpr float kMax{1000.f};
  static constexpr float kMin{-kMax};
  static constexpr float kInterval{10.f};

  //! @brief kMin [mm] ～ kMax [mm]の範囲に kInterval
  //! [mm]間隔で脚設置可能点を生成する.
  MapState InitMap() const override {
    std::vector<Vector3> map_point;

    for (float x = kMin; x <= kMax; x += kInterval) {
      for (float y = kMin; y <= kMax; y += kInterval) {
        Vector3 point(x, y, 0);
        map_point.emplace_back(point);
      }
    }

    return MapState(map_point);
  }

  void UpdateMap([[maybe_unused]] MapState* current_map) const override {}

 private:
  static_assert(kMax > DividedMapState::kDividedMapMaxX,
                "テストマップのサイズが足りません.");
  static_assert(kMin < DividedMapState::kDividedMapMinX,
                "テストマップのサイズが足りません.");
  static_assert(kMax > DividedMapState::kDividedMapMaxY,
                "テストマップのサイズが足りません.");
  static_assert(kMin < DividedMapState::kDividedMapMinY,
                "テストマップのサイズが足りません.");
};

}  // namespace divided_map_state_test_internal

TEST_SUITE("DividedMapState::Init") {
  using gaitgen::DividedMapState;
  using gaitgen::MapState;
  using gaitgen::Vector3;

  TEST_CASE(
      "マップの状態とロボットの重心座標を渡した時,"
      "マップを重心を中心に格子状に分割するべき") {
    using divided_map_state_test_internal::TestMapCreator;

    // Act.テスト用のマップを生成する.
    TestMapCreator test_map_creator;
    MapState map_state = test_map_creator.InitMap();

    // Act.テスト用のロボットの重心座標を生成する.
    const Vector3 global_robot_com(0, 0, 0);

    // Arrange.テスト対象のクラスのメソッドを呼び出す.
    DividedMapState divided_map_state;
    divided_map_state.Init(map_state, global_robot_com);

    // Assert.マップのサイズが正しいか確認する.
    int center_index_x =
        divided_map_state.GetDividedMapIndexX(global_robot_com.x);
    int center_index_y =
        divided_map_state.GetDividedMapIndexX(global_robot_com.y);

    constexpr int expected_num = static_cast<int>(
        (DividedMapState::kDividedAreaLength / TestMapCreator::kInterval) *
        (DividedMapState::kDividedAreaLength / TestMapCreator::kInterval));

    const auto result =
        divided_map_state.GetPointNum(center_index_x, center_index_y);

    REQUIRE(result.has_value());
    CHECK_EQ(result.value(), expected_num);
  }
}

TEST_SUITE("DividedMapState::ClampDividedMapIndex") {
  using gaitgen::DividedMapState;

  TEST_CASE("0未満の値を渡した時,0を返すべき") {
    CHECK_EQ(DividedMapState::ClampDividedMapIndex(-1), 0);
    CHECK_EQ(DividedMapState::ClampDividedMapIndex(-100), 0);

    CHECK_EQ(DividedMapState::ClampDividedMapIndex(0), 0);
  }

  TEST_CASE("最大値を渡した時,最大値 - 1を返すべき") {
    CHECK_EQ(
        DividedMapState::ClampDividedMapIndex(DividedMapState::kDividedNum),
        DividedMapState::kDividedNum - 1);

    CHECK_EQ(
        DividedMapState::ClampDividedMapIndex(DividedMapState::kDividedNum + 1),
        DividedMapState::kDividedNum - 1);

    CHECK_EQ(DividedMapState::ClampDividedMapIndex(
                 DividedMapState::kDividedNum + 100),
             DividedMapState::kDividedNum - 1);

    CHECK_EQ(
        DividedMapState::ClampDividedMapIndex(DividedMapState::kDividedNum - 1),
        DividedMapState::kDividedNum - 1);
  }
}
