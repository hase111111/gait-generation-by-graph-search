
//! @file map_creator_for_simulation.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "map_creator_for_simulation.h"

#include <algorithm>
#include <random>

#include "cassert_define.h"
#include "math_util.h"

namespace designlab {

MapCreatorForSimulation::MapCreatorForSimulation(
    const SimulationMapParameter& param)
    : parameter_(param) {
  // map_min_x が map_max_x より小さいことを確認する.
  assert(parameter_.map_min_x < parameter_.map_max_x);

  // map_min_y が map_max_y より小さいことを確認する.
  assert(parameter_.map_min_y < parameter_.map_max_y);
}

MapState MapCreatorForSimulation::InitMap() const {
  std::vector<Vector3> map_data;

  switch (parameter_.mode) {
    case SimulationMapMode::kFlat: {
      CreateFlatMap(&map_data);
      break;
    }
    case SimulationMapMode::kVerticalStripe: {
      CreateVerticalStripeMap(&map_data);
      break;
    }
    case SimulationMapMode::kHorizontalStripe: {
      CreateHorizontalStripeMap(&map_data);
      break;
    }
    case SimulationMapMode::kDiagonalStripe: {
      CreateDiagonalStripeMap(&map_data);
      break;
    }
    case SimulationMapMode::kSlantedStripes: {
      CreateSlantedStripesMap(&map_data);
      break;
    }
    case SimulationMapMode::kMesh: {
      CreateMeshMap(&map_data);
      break;
    }
    case SimulationMapMode::kLatticePoint: {
      CreateLatticePointMap(&map_data);
      break;
    }
    case SimulationMapMode::kCircle: {
      CreateCircleMap(&map_data);
      break;
    }
    case SimulationMapMode::kDonut: {
      CreateDonutMap(&map_data);
      break;
    }
    case SimulationMapMode::kWall: {
      CreateWallMap(&map_data);
      break;
    }
    default: {
      // 異常な値が入力されたら,平面のマップを生成する.
      CreateFlatMap(&map_data);
      break;
    }
  }

  // オプション指定に基づき,Z座標を変更する.

  if (parameter_.option &
      static_cast<unsigned int>(SimulationMapOption::kPerforated)) {
    // 穴あき地形にする.
    ChangeMapToPerforated(&map_data);
  }

  if (parameter_.option &
      static_cast<unsigned int>(SimulationMapOption::kStep)) {
    // 階段状にする.
    ChangeMapToStep(&map_data);
  }

  if (parameter_.option &
      static_cast<unsigned int>(SimulationMapOption::kSlope)) {
    // 坂道にする.
    ChangeMapToSlope(&map_data);
  }

  if (parameter_.option &
      static_cast<unsigned int>(SimulationMapOption::kTilt)) {
    // 坂道にする.
    ChangeMapToTilt(&map_data);
  }

  if (parameter_.option &
      static_cast<unsigned int>(SimulationMapOption::kRough)) {
    // デコボコにする.
    ChangeMapToRough(&map_data);
  }

  if (parameter_.option &
      static_cast<unsigned int>(SimulationMapOption::kRadiation)) {
    // 放射状に穴をあける.
    ChangeMapToRadial(&map_data);
  }

  return MapState(map_data);
}

void MapCreatorForSimulation::UpdateMap(
    [[maybe_unused]] MapState* current_map) const {
  // マップを更新する必要がないので,何もしない.
}

void MapCreatorForSimulation::CreateFlatMap(std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.
  assert(map->empty());    // map が空であることを確認する.

  // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める.
  const float x_max = (parameter_.map_max_x - parameter_.map_min_x) /
                      MapState::kMapPointDistance;
  const float y_max = (parameter_.map_max_y - parameter_.map_min_y) /
                      MapState::kMapPointDistance;

  for (int x = 0; x < x_max; x++) {
    for (int y = 0; y < y_max; y++) {
      // ロボットの正面方向.
      const float x_pos =
          parameter_.map_min_x + x * MapState::kMapPointDistance;

      // ロボットの側面方向.
      const float y_pos =
          parameter_.map_min_y + y * MapState::kMapPointDistance;

      map->push_back(
          {x_pos, y_pos, parameter_.base_z});  // 脚設置可能点を追加する.
    }
  }
}

void MapCreatorForSimulation::CreateVerticalStripeMap(
    std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.
  assert(map->empty());    // map が空であることを確認する.

  // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める.
  const float x_max = (parameter_.map_max_x - parameter_.map_min_x) /
                      MapState::kMapPointDistance;
  const float y_max = (parameter_.map_max_y - parameter_.map_min_y) /
                      MapState::kMapPointDistance;

  for (int x = 0; x < x_max; x++) {
    for (int y = 0; y < y_max; y++) {
      // 縦じまをつくるために,一定間隔ごとに追加する.最初の待機場所の座標ならば無条件に追加する.
      const float x_rough =
          (parameter_.map_start_rough_x - parameter_.map_min_x) /
          MapState::kMapPointDistance;

      if (y % (parameter_.stripe_interval * 2) < parameter_.stripe_interval ||
          x < x_rough) {
        // ロボットの正面方向.
        const float x_pos =
            parameter_.map_min_x + x * MapState::kMapPointDistance;

        // ロボットの側面方向.
        const float y_pos =
            parameter_.map_min_y + y * MapState::kMapPointDistance;

        map->push_back(
            {x_pos, y_pos, parameter_.base_z});  // 脚設置可能点を追加する.
      }
    }
  }
}

void MapCreatorForSimulation::CreateHorizontalStripeMap(
    std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.
  assert(map->empty());    // map が空であることを確認する.

  // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める.
  const float x_max = (parameter_.map_max_x - parameter_.map_min_x) /
                      MapState::kMapPointDistance;
  const float y_max = (parameter_.map_max_y - parameter_.map_min_y) /
                      MapState::kMapPointDistance;

  for (int x = 0; x < x_max; x++) {
    for (int y = 0; y < y_max; y++) {
      // 縦じまをつくるために,一定間隔ごとに追加する.最初の待機場所の座標ならば無条件に追加する.
      const float x_rough =
          (parameter_.map_start_rough_x - parameter_.map_min_x) /
          MapState::kMapPointDistance;

      if (x % (parameter_.stripe_interval * 2) < parameter_.stripe_interval ||
          x < x_rough) {
        // ロボットの正面方向.
        const float x_pos =
            parameter_.map_min_x + x * MapState::kMapPointDistance;

        // ロボットの側面方向.
        const float y_pos =
            parameter_.map_min_y + y * MapState::kMapPointDistance;

        map->push_back(
            {x_pos, y_pos, parameter_.base_z});  // 脚設置可能点を追加する.
      }
    }
  }
}

void MapCreatorForSimulation::CreateDiagonalStripeMap(
    std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.
  assert(map->empty());    // map が空であることを確認する.

  // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める.
  const float x_max = (parameter_.map_max_x - parameter_.map_min_x) /
                      MapState::kMapPointDistance;
  const float y_max = (parameter_.map_max_y - parameter_.map_min_y) /
                      MapState::kMapPointDistance;

  for (int x = 0; x < x_max; x++) {
    for (int y = 0; y < y_max; y++) {
      // 斜めじまをつくるために,一定間隔ごとに追加する.最初の待機場所の座標ならば無条件に追加する.
      const bool x_in_stripe =
          x % (parameter_.stripe_interval * 2) < parameter_.stripe_interval;

      const bool y_in_stripe =
          y % (parameter_.stripe_interval * 2) < parameter_.stripe_interval;

      const bool do_create_map = x_in_stripe == y_in_stripe;

      const float x_rough =
          (parameter_.map_start_rough_x - parameter_.map_min_x) /
          MapState::kMapPointDistance;

      if (do_create_map || x < x_rough) {
        // ロボットの正面方向.
        const float x_pos =
            parameter_.map_min_x + x * MapState::kMapPointDistance;

        // ロボットの側面方向.
        const float y_pos =
            parameter_.map_min_y + y * MapState::kMapPointDistance;

        map->push_back(
            {x_pos, y_pos, parameter_.base_z});  // 脚設置可能点を追加する.
      }
    }
  }
}

void MapCreatorForSimulation::CreateSlantedStripesMap(
    std::vector<Vector3>* map) const {
  assert(map != nullptr);
  assert(map->empty());

  // マップの x と y の最大点数
  const float x_max = (parameter_.map_max_x - parameter_.map_min_x) /
                      MapState::kMapPointDistance;
  const float y_max = (parameter_.map_max_y - parameter_.map_min_y) /
                      MapState::kMapPointDistance;

  // 追加：斜め縞の角度を指定（deg → rad）
  const float theta = math_util::ConvertDegToRad(parameter_.diagonal_angle);

  // ストライプ周期
  const float stripe_cycle = parameter_.stripe_interval * 2.f;

  for (int x = 0; x < x_max; x++) {
    for (int y = 0; y < y_max; y++) {
      // 座標を角度 θ だけ回転した「仮想の x座標」
      float xr = x * cos(theta) - y * sin(theta);

      // fmod は負になる可能性があるため大きなオフセットを足す
      float mod_x = fmod(xr + 10000.0f * stripe_cycle, stripe_cycle);

      bool in_stripe = mod_x < parameter_.stripe_interval;

      // 待機場所 x < x_rough は常に追加
      const float x_rough =
          (parameter_.map_start_rough_x - parameter_.map_min_x) /
          MapState::kMapPointDistance;

      if (in_stripe || x < x_rough) {
        const float x_pos =
            parameter_.map_min_x + x * MapState::kMapPointDistance;
        const float y_pos =
            parameter_.map_min_y + y * MapState::kMapPointDistance;

        map->push_back({x_pos, y_pos, parameter_.base_z});
      }
    }
  }
}

void MapCreatorForSimulation::CreateMeshMap(std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.
  assert(map->empty());    // map が空であることを確認する.

  // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める.
  const float x_max = (parameter_.map_max_x - parameter_.map_min_x) /
                      MapState::kMapPointDistance;
  const float y_max = (parameter_.map_max_y - parameter_.map_min_y) /
                      MapState::kMapPointDistance;

  for (int x = 0; x < x_max; x++) {
    for (int y = 0; y < y_max; y++) {
      // 網目模様をつくるために,一定間隔ごとに追加する.
      // 最初の待機場所の座標ならば無条件に追加する.
      bool do_create_map;

      if ((x % (parameter_.stripe_interval * 2) < parameter_.stripe_interval)) {
        if ((y % (parameter_.stripe_interval * 2) <
             parameter_.stripe_interval)) {
          do_create_map = true;
        } else {
          do_create_map = false;
        }
      } else {
        do_create_map = true;
      }

      const float x_rough =
          (parameter_.map_start_rough_x - parameter_.map_min_x) /
          MapState::kMapPointDistance;

      if (do_create_map || x < x_rough) {
        // ロボットの正面方向.
        const float x_pos =
            parameter_.map_min_x + x * MapState::kMapPointDistance;

        // ロボットの側面方向.
        const float y_pos =
            parameter_.map_min_y + y * MapState::kMapPointDistance;

        map->push_back(
            {x_pos, y_pos, parameter_.base_z});  // 脚設置可能点を追加する.
      }
    }
  }
}

void MapCreatorForSimulation::CreateLatticePointMap(
    std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.
  assert(map->empty());    // map が空であることを確認する.

  // マップの x と y の存在範囲全体に脚設置可能点を敷き詰める.

  const float x_max = (parameter_.map_max_x - parameter_.map_min_x) /
                      MapState::kMapPointDistance;
  const float y_max = (parameter_.map_max_y - parameter_.map_min_y) /
                      MapState::kMapPointDistance;

  for (int x = 0; x < x_max; x++) {
    for (int y = 0; y < y_max; y++) {
      // 網目模様をつくるために,一定間隔ごとに追加する.
      // 最初の待機場所の座標ならば無条件に追加する.
      bool do_create_map = false;

      if ((x % (parameter_.stripe_interval * 2) < parameter_.stripe_interval)) {
        if ((y % (parameter_.stripe_interval * 2) >=
             parameter_.stripe_interval)) {
          do_create_map = false;
        } else {
          do_create_map = true;
        }
      } else {
        do_create_map = false;
      }

      const float x_rough =
          (parameter_.map_start_rough_x - parameter_.map_min_x) /
          MapState::kMapPointDistance;

      if (do_create_map || x < x_rough) {
        // ロボットの正面方向.
        const float x_pos =
            parameter_.map_min_x + x * MapState::kMapPointDistance;

        // ロボットの側面方向.
        const float y_pos =
            parameter_.map_min_y + y * MapState::kMapPointDistance;

        // 脚設置可能点を追加する.
        map->push_back({x_pos, y_pos, parameter_.base_z});
      }
    }
  }
}

void MapCreatorForSimulation::CreateCircleMap(std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.

  // 円が存在する範囲に脚設置可能点を敷き詰める.
  // その後,円の外側の脚設置可能点を削除する.

  const float x_min = parameter_.circle_center.x - parameter_.circle_radius;
  const float x_max = parameter_.circle_center.x + parameter_.circle_radius;
  const float y_min = parameter_.circle_center.y - parameter_.circle_radius;
  const float y_max = parameter_.circle_center.y + parameter_.circle_radius;

  for (int x = 0; x < (x_max - x_min) / MapState::kMapPointDistance; ++x) {
    for (int y = 0; y < (y_max - y_min) / MapState::kMapPointDistance; ++y) {
      // ロボットの正面方向.
      const float x_pos = x_min + x * MapState::kMapPointDistance;

      // ロボットの側面方向.
      const float y_pos = y_min + y * MapState::kMapPointDistance;

      // 脚設置可能点を追加する.
      const float distance =
          Vector2(x_pos, y_pos)
              .GetDistanceFrom(parameter_.circle_center.ProjectedXY());

      if (distance <= parameter_.circle_radius) {
        map->push_back({x_pos, y_pos, parameter_.base_z});
      }
    }
  }
}

void MapCreatorForSimulation::CreateDonutMap(std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.

  // ドーナツが存在する範囲に脚設置可能点を敷き詰める.
  // その後,ドーナツの外側の脚設置可能点を削除する.

  const float x_min = parameter_.circle_center.x - parameter_.circle_radius;
  const float x_max = parameter_.circle_center.x + parameter_.circle_radius;
  const float y_min = parameter_.circle_center.y - parameter_.circle_radius;
  const float y_max = parameter_.circle_center.y + parameter_.circle_radius;

  for (int x = 0; x < (x_max - x_min) / MapState::kMapPointDistance; ++x) {
    for (int y = 0; y < (y_max - y_min) / MapState::kMapPointDistance; ++y) {
      // ロボットの正面方向.
      const float x_pos = x_min + x * MapState::kMapPointDistance;

      // ロボットの側面方向.
      const float y_pos = y_min + y * MapState::kMapPointDistance;

      // 脚設置可能点を追加する.
      const float distance =
          Vector2(x_pos, y_pos)
              .GetDistanceFrom(parameter_.circle_center.ProjectedXY());

      if (parameter_.donut_radius <= distance &&
          distance <= parameter_.circle_radius) {
        map->push_back({x_pos, y_pos, parameter_.base_z});
      }
    }
  }
}

void MapCreatorForSimulation::CreateWallMap(std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.
  assert(map->empty());    // map が空であることを確認する.

  // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める.
  const float x_max = (parameter_.map_max_x - parameter_.map_min_x) /
                      MapState::kMapPointDistance;
  const float y_max = (parameter_.map_max_y - parameter_.map_min_y) /
                      MapState::kMapPointDistance;

  for (int x = 0; x < x_max; x++) {
    for (int y = 0; y < y_max; y++) {
      // ロボットの正面方向.
      const float x_pos =
          parameter_.map_min_x + x * MapState::kMapPointDistance;

      // ロボットの側面方向.
      const float y_pos =
          parameter_.map_min_y + y * MapState::kMapPointDistance;

      map->push_back(
          {x_pos, y_pos, parameter_.base_z});  // 脚設置可能点を追加する.
    }
  }

  // 壁を作成する.
  for (float h = 0; h < parameter_.wall_height;
       h += MapState::kMapPointDistance) {
    for (int y = 1; y < y_max; y++) {
      // ロボットの側面方向.
      const float y_pos =
          parameter_.map_min_y + y * MapState::kMapPointDistance;

      // 脚設置可能点を追加する.
      map->push_back({parameter_.wall_x, y_pos, h});
    }
  }
}

void MapCreatorForSimulation::ChangeMapToPerforated(
    std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.

  // 厳密にホール率に合わせるために,まずはマップを stripe_interval
  // に合わせて区切って, 全部で何マスあるか調べる.
  const int cell_num_x =
      static_cast<int>((parameter_.map_max_x - parameter_.map_start_rough_x) /
                       MapState::kMapPointDistance) /
      parameter_.stripe_interval;

  const int cell_num_y =
      static_cast<int>((parameter_.map_max_y - parameter_.map_min_y) /
                       MapState::kMapPointDistance) /
      parameter_.stripe_interval;

  const int cell_sum = cell_num_x * cell_num_y;

  // マスの数だけ要素を持つ vector を用意する.値は全て false で初期化する.
  std::vector<bool> do_perforated(cell_sum, false);

  // ホール率に合わせて,値を true に変更する.
  const int hole_num = cell_sum * parameter_.hole_rate / 100;

  for (int i = 0; i < hole_num; i++) {
    do_perforated.at(i) = true;
  }

  // ランダムなホールにするために要素の順番をシャッフルする.
  std::random_device rd;
  std::mt19937 mt(rd());
  std::shuffle(std::begin(do_perforated), std::end(do_perforated), mt);

  // マップに穴をあける.
  for (auto itr = (*map).begin(); itr != (*map).end();) {
    // 待機場所の外に対してのみ作業をする.
    if ((*itr).x < parameter_.map_start_rough_x) {
      itr++;
      continue;
    }

    // マスで区切るとどこに位置するかを調べる.
    const int cell_pos_x =
        static_cast<int>(((*itr).x - parameter_.map_start_rough_x) /
                         MapState::kMapPointDistance) /
        parameter_.stripe_interval;

    const int cell_pos_y = static_cast<int>(((*itr).y - parameter_.map_min_y) /
                                            MapState::kMapPointDistance) /
                           parameter_.stripe_interval;

    const int cell_index = cell_pos_x * cell_num_y + cell_pos_y;

    // cell_pos の値がおかしくないかチェックする.
    if (0 <= cell_index && cell_index < do_perforated.size()) {
      // 穴あけをする場所ならば.
      if (do_perforated[cell_index]) {
        // 脚設置可能点を消してイテレータを更新する.
        itr = (*map).erase(itr);
      } else {
        // 消さないならば次へ移動する.
        itr++;
      }
    } else {
      // 消さずに次へ移動する.
      itr++;
    }
  }
}

void MapCreatorForSimulation::ChangeMapToStep(std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.

  for (auto& i : *map) {
    // 待機場所の外に対してのみ作業をする.
    if (i.x > parameter_.map_start_rough_x) {
      // 階段の何段目かを計算する.待機場所のすぐ上が1段目なので1を足している.
      const int step_count =
          1 + static_cast<int>((i.x - parameter_.map_start_rough_x) /
                               parameter_.step_length);

      // 階段状にZ座標を変更する.
      i.z += parameter_.step_height * step_count;
    }
  }
}

void MapCreatorForSimulation::ChangeMapToSlope(
    std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.

  for (auto& i : *map) {
    // 待機場所の外に対してのみ作業をする.
    if (i.x > parameter_.map_start_rough_x) {
      // 階段状にZ座標を変更する.
      i.z += (i.x - parameter_.map_start_rough_x) *
             tan(math_util::ConvertDegToRad(parameter_.slope_angle));
    }
  }
}

void MapCreatorForSimulation::ChangeMapToTilt(std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.

  for (auto& i : *map) {
    // 待機場所の外に対してのみ作業をする
    if (i.x > parameter_.map_start_rough_x) {
      // 階段状にZ座標を変更する.
      i.z += i.y * tan(math_util::ConvertDegToRad(parameter_.tilt_angle));
    }
  }
}

void MapCreatorForSimulation::ChangeMapToRough(
    std::vector<Vector3>* map) const {
  assert(map != nullptr);

  // 粗い地形を適用する範囲（待機場所より右側）
  const float rough_range_x =
      (parameter_.map_max_x - parameter_.map_start_rough_x);

  const int cell_num_x =
      static_cast<int>(rough_range_x / MapState::kMapPointDistance) /
      parameter_.stripe_interval;

  const int cell_num_y =
      static_cast<int>((parameter_.map_max_y - parameter_.map_min_y) /
                       MapState::kMapPointDistance) /
      parameter_.stripe_interval;

  const int cell_sum = cell_num_x * cell_num_y;

  if (cell_sum <= 0) return;

  // 各セルにランダムな高さを割り当てる
  std::vector<float> change_z_length(cell_sum);

  for (int i = 0; i < cell_sum; i++) {
    change_z_length[i] = math_util::GenerateRandomNumber(
        parameter_.rough_min_height, parameter_.rough_max_height);
  }

  // マップに適用
  for (auto& i : *map) {
    if (i.x <= parameter_.map_start_rough_x) {
      continue;
    }

    const float local_x =
        (i.x - parameter_.map_start_rough_x) / MapState::kMapPointDistance;
    const float local_y =
        (i.y - parameter_.map_min_y) / MapState::kMapPointDistance;

    const int cell_pos_x =
        static_cast<int>(local_x) / parameter_.stripe_interval;
    const int cell_pos_y =
        static_cast<int>(local_y) / parameter_.stripe_interval;

    const int cell_index = cell_pos_x * cell_num_y + cell_pos_y;

    if (0 <= cell_index && cell_index < cell_sum) {
      i.z += change_z_length[cell_index];
    }
  }
}

void MapCreatorForSimulation::ChangeMapToRadial(
    std::vector<Vector3>* map) const {
  assert(map != nullptr);  // map が nullptr でないことを確認する.

  const float divided_angle =
      std::numbers::pi_v<float> / parameter_.radial_division;

  for (auto itr = (*map).begin(); itr != (*map).end();) {
    // 放射状の穴あけの中心からの角度を計算する.
    const float angle =
        atan2((*itr).y - parameter_.radial_center.y,
              (*itr).x - parameter_.radial_center.x) +
        std::numbers::pi_v<float> +
        math_util::ConvertDegToRad(parameter_.radial_angle_offset);

    if (static_cast<int>(angle / divided_angle) % 2 == 1) {
      const int i =
          static_cast<int>(angle / divided_angle);  // 何番目の角度かを計算する.
      const float angle_dif =
          angle - i * divided_angle;  // 何番目の角度からの差を計算する.

      // 角度の差がホール率より小さい場合は消す.
      if (angle_dif < divided_angle * parameter_.radial_hole_rate / 100) {
        // 脚設置可能点を消してイテレータを更新する.
        itr = (*map).erase(itr);
      } else {
        // 消さないならば次へ移動する.
        itr++;
      }
    } else {
      // 消さないならば次へ移動する.
      itr++;
    }
  }
}

}  // namespace designlab
