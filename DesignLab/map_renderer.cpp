#include "map_renderer.h"

#include <Dxlib.h>

#include "dxlib_util.h"


namespace dldu = designlab::dxlib_util;


MapRenderer::MapRenderer() :
	kColorGray(GetColor(80, 80, 80)),
	kColorLightGray(GetColor(160, 160, 160)),
	kColorDarkGray(GetColor(40, 40, 40)),
	kCubeSize(15.f)
{
}

void MapRenderer::SetNode(const RobotStateNode& pos)
{
	hexapod_pos_ = pos.global_center_of_mass;

	devide_map_.Init(map_, hexapod_pos_);
}

void MapRenderer::SetMapState(const MapState& map)
{
	map_ = map;

	devide_map_.Init(map_, hexapod_pos_);
}


void MapRenderer::Draw() const
{
	size_t kSize = map_.GetMapPointSize();

	for (size_t i = 0; i < kSize; i++)
	{
		dldu::DrawCube3DWithTopPos(
			dldu::ConvertToDxlibVec(map_.GetMapPoint(i)),
			kCubeSize,
			kColorDarkGray
		);
	}

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			for (int k = 0; k < devide_map_.GetPointNum(i, j); k++)
			{
				dldu::DrawCube3DWithTopPos(
					dldu::ConvertToDxlibVec(devide_map_.GetPointPos(i, j, k)),
					kCubeSize,
					(i + j) % 2 == 0 ? kColorLightGray : kColorGray
				);
			}
		}
	}
}