
//! @file divided_map_state.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "divided_map_state.h"

#include "cmdio_util.h"
#include "math_util.h"

namespace gaitgen {

DividedMapState::DividedMapState(const float min_z)
    : kMapMinZ(min_z),
      global_robot_com_{},
      divided_map_point_(math_util::Squared(kDividedNum)),
      divided_map_top_z_(math_util::Squared(kDividedNum)) {
  // コンストラクタで,配列のサイズを確保しておく.
  Clear();
}

void DividedMapState::Init(const MapState& map_state,
                           const Vector3& global_robot_com) {
  // 配列のサイズが確保されているか確認.
  assert(divided_map_point_.size() == math_util::Squared(kDividedNum));
  assert(divided_map_top_z_.size() == math_util::Squared(kDividedNum));

  Clear();

  global_robot_com_ = global_robot_com;  // ロボットの位置を更新する.

  // マップのデータ全てを参照し,切り分ける.
  const size_t kMapPointSize = map_state.GetMapPointSize();

  std::vector<Vector3> points_data_for_plane_;  // 平面の情報を格納する.

  for (size_t i = 0; i < kMapPointSize; ++i) {
    // xy方向のブロック番号をそれぞれ求める.
    const Vector3 point = map_state.GetMapPoint(i);

    // 範囲内にいないならば処理をやめ,続行.
    if (!IsInMap(point)) {
      continue;
    }

    // 平面の情報を更新する.
    points_data_for_plane_.push_back(point);

    const int x = GetDividedMapIndexX(point.x);
    const int y = GetDividedMapIndexY(point.y);

    // マップの範囲内にいる時のみ追加する.
    if (IsValidIndex(x) && IsValidIndex(y)) {
      divided_map_point_[GetDividedMapIndex(x, y)].push_back(point);

      // 最大値を更新する.
      divided_map_top_z_[GetDividedMapIndex(x, y)] =
          (std::max)(point.z, divided_map_top_z_[GetDividedMapIndex(x, y)]);
    }
  }

  // 平面の情報を更新する.
  auto plane =
      detectMultiplePlanes(points_data_for_plane_, 10, 100, 0.01f, 200);
  divided_map_plane_ = plane;
}

void DividedMapState::Clear() {
  // vectorの中身を全てクリアする.
  for (auto& i : divided_map_point_) {
    i.clear();
  }

  // 最小値で埋める.
  for (auto& i : divided_map_top_z_) {
    i = kMapMinZ;
  }

  divided_map_plane_.clear();  // 平面の情報もクリアする.
}

nostd::expected<int, std::string> DividedMapState::GetPointNum(
    const int x_index, const int y_index) const {
  // 存在していなければ終了
  if (!IsValidIndex(x_index) || !IsValidIndex(y_index)) {
    return nostd::unexpected<std::string>(
        "DividedMapState::GetPointNum: Invalid index.");
  }

  return static_cast<int>(
      divided_map_point_[GetDividedMapIndex(x_index, y_index)].size());
}

nostd::expected<Vector3, std::string> DividedMapState::GetPointPos(
    const int x_index, const int y_index, const int divide_map_index) const {
  // 存在していなければ終了.
  if (!IsValidIndex(x_index) || !IsValidIndex(y_index)) {
    return nostd::unexpected<std::string>(
        "DividedMapState::GetPointPos: Invalid index.");
  }

  const size_t size =
      divided_map_point_[GetDividedMapIndex(x_index, y_index)].size();

  if (divide_map_index < 0 || static_cast<int>(size) <= divide_map_index) {
    return Vector3::GetZeroVec();
  }

  // 存在しているならば値を返す.
  return divided_map_point_[GetDividedMapIndex(x_index, y_index)]
                           [divide_map_index];
}

void DividedMapState::GetPointVector(const int x_index, const int y_index,
                                     std::vector<Vector3>* point_vec) const {
  if (point_vec == nullptr) {
    return;
  }

  // 存在していなければ終了.
  if (!IsValidIndex(x_index) || !IsValidIndex(y_index)) {
    return;
  }

  (*point_vec) = divided_map_point_[GetDividedMapIndex(x_index, y_index)];
}

float DividedMapState::GetTopZ(const int x_index, const int y_index) const {
  // 存在していなければ終了.
  if (!IsValidIndex(x_index) || !IsValidIndex(y_index)) {
    return kMapMinZ;
  }

  return divided_map_top_z_[GetDividedMapIndex(x_index, y_index)];
}

}  // namespace gaitgen
