
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "map_renderer.h"

#include <Dxlib.h>

#include "dxlib_util.h"


namespace designlab
{

MapRenderer::MapRenderer() :
    kColorGray(GetColor(180, 80, 80)),
    kColorLightGray(GetColor(200, 160, 160)),
    kColorDarkGray(GetColor(40, 40, 40)),
    kCubeSize(20.f)
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
    size_t kSize = map_.GetMapPointSize();

    for (size_t i = 0; i < kSize; i++)
    {
        dxlib_util::DrawCube3DWithTopPos(
          dxlib_util::ConvertToDxlibVec(map_.GetMapPoint(i)),
          kCubeSize,
          kColorDarkGray);
    }

    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            for (int k = 0; k < divided_map_.GetPointNum(i, j); k++)
            {
                dxlib_util::DrawCube3DWithTopPos(
                  dxlib_util::ConvertToDxlibVec(divided_map_.GetPointPos(i, j, k)),
                  kCubeSize,
                  (i + j) % 2 == 0 ? kColorLightGray : kColorGray);
            }
        }
    }
}

}  // namespace designlab
