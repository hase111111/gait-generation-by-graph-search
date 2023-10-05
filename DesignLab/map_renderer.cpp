#include "map_renderer.h"

#include <Dxlib.h>

#include "dxlib_util.h"
#include "map_const.h"


namespace dldu = designlab::dxlib_util;


MapRenderer::MapRenderer() : 
	COLOR_GRAY(GetColor(80, 80, 80)), 
	COLOR_LIGHT_GRAY(GetColor(160, 160, 160)),
	CUBE_SIZE(15.f)
{
}


void MapRenderer::Draw(const MapState& map) const
{
	size_t kSize = map.GetMapPointSize();

	for (size_t i = 0; i < kSize; i++)
	{
		int x_index = DevideMapState::GetDevideMapIndexX(map.GetMapPoint(i).x);
		int y_index = DevideMapState::GetDevideMapIndexY(map.GetMapPoint(i).y);

		dldu::DrawCube3DWithTopPos(dldu::ConvertToDxlibVec(map.GetMapPoint(i)), CUBE_SIZE, (x_index + y_index) % 2 == 0 ? COLOR_GRAY : COLOR_LIGHT_GRAY);
	}
}
