
//! @file map_renderer.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "map_renderer.h"

#include <Dxlib.h>

#include "dxlib_util.h"
#include "loop_util.h"

namespace designlab
{

MapRenderer::MapRenderer() :
    color_gray_(GetColor(180, 80, 80)),
    color_light_gray_(GetColor(200, 160, 160)),
    color_dark_gray_(GetColor(40, 40, 40)),
    cube_size_(15.f)
{
}

void MapRenderer::SetNode(const RobotStateNode& pos)
{
    hexapod_pos_ = pos.center_of_mass_global_coord;

    divided_map_.Init(map_, hexapod_pos_);
}

void MapRenderer::SetMapState(const MapState& map)
{
    map_ = map;

    divided_map_.Init(map_, hexapod_pos_);
}


void MapRenderer::Draw() const
{
    using dxlib_util::DrawCube3DWithTopPos;
    using dxlib_util::ConvertToDxlibVec;

    const size_t map_point_size = map_.GetMapPointSize();  // マップの点の数．

    for (size_t i = 0; i < map_point_size; i++)
    {
        DrawCube3DWithTopPos(ConvertToDxlibVec(map_.GetMapPoint(i)), cube_size_, color_dark_gray_);
    }

    for (const auto& [i, j] : DoubleIntRange(30, 30))
    {
        for (int k = 0; k < divided_map_.GetPointNum(i, j); k++)
        {
            DrawCube3DWithTopPos(
              ConvertToDxlibVec(divided_map_.GetPointPos(i, j, k)),
              cube_size_,
              (i + j) % 2 == 0 ? color_light_gray_ : color_gray_);
        }

    }
}

}  // namespace designlab
