
//! @file map_renderer.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "map_renderer.h"

#include <Dxlib.h>

#include "dxlib_util.h"
#include "loop_util.h"

namespace designlab {

MapRenderer::MapRenderer(const bool draw_flat_map)
    : color_gray_(GetColor(180, 80, 80)),
      color_light_gray_(GetColor(200, 160, 160)),
      color_dark_gray_(GetColor(40, 40, 40)),
      cube_size_(15.f),
      draw_flat_map_(draw_flat_map) {}

void MapRenderer::SetNode(const RobotStateNode& pos) {
  hexapod_pos_ = pos.center_of_mass_global_coord;

  divided_map_.Init(map_, hexapod_pos_);
}

void MapRenderer::SetMapState(const MapState& map) {
  map_ = map;

  divided_map_.Init(map_, hexapod_pos_);
}

void MapRenderer::Draw() const {
  using dxlib_util::ConvertToDxlibVec;
  using dxlib_util::DrawCube3DWithTopPos;

  const size_t map_point_size = map_.GetMapPointSize();  // マップの点の数.

  for (size_t i = 0; i < map_point_size; i++) {
    DrawCube3DWithTopPos(ConvertToDxlibVec(map_.GetMapPoint(i)), cube_size_,
                         color_dark_gray_);
  }

  for (const auto& [i, j] : DoubleIntRange(30, 30)) {
    const auto num = divided_map_.GetPointNum(i, j);
    if (!num) {
      continue;  // このマスに脚設置可能点が存在しない場合はスキップ.
    }

    for (int k = 0; k < *num; k++) {
      // 脚設置可能点を描画する.
      const auto point_pos = divided_map_.GetPointPos(i, j, k);
      if (!point_pos) {
        continue;  // このマスに脚設置可能点が存在しない場合はスキップ.
      }

      DrawCube3DWithTopPos(ConvertToDxlibVec(*point_pos), cube_size_,
                           (i + j) % 2 == 0 ? color_light_gray_ : color_gray_);
    }
  }

  if (draw_flat_map_) {
    const auto plane = divided_map_.GetDividedMapPlane();

    for (size_t i = 0; i < plane.size(); ++i) {
      // 平面を描画する.
      std::array<VECTOR, 4> vertex{};
      for (size_t l = 0; l < 4; l++) {
        Vector3 p_corner = plane[i].corners[l];
        p_corner.z -= 10.0;

        vertex[l] = ConvertToDxlibVec(p_corner);
      }

      unsigned int color{};
      if (i % 3 == 0) {
        color = GetColor(255, 20, 20);  // 赤
      } else if (i % 3 == 1) {
        color = GetColor(20, 255, 20);  // 緑
      } else {
        color = GetColor(20, 20, 255);  // 青
      }

      dxlib_util::DrawFlatPlane(vertex, ConvertToDxlibVec(plane[i].normal),
                                color);
    }
  }
}

}  // namespace designlab
