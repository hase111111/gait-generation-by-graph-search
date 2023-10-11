#include "map_renderer.h"

#include <Dxlib.h>

#include "dxlib_util.h"
#include "map_const.h"


namespace dldu = designlab::dxlib_util;


MapRenderer::MapRenderer() : 
	kColorGray(GetColor(80, 80, 80)), 
	kColorLightGray(GetColor(160, 160, 160)),
	kCubeSize(15.f)
{
}


void MapRenderer::Draw(const MapState& map) const
{
	size_t kSize = map.GetMapPointSize();

	for (size_t i = 0; i < kSize; i++)
	{
		int x_index = DevideMapState::GetDevideMapIndexX(map.GetMapPoint(i).x);
		int y_index = DevideMapState::GetDevideMapIndexY(map.GetMapPoint(i).y);

		dldu::DrawCube3DWithTopPos(dldu::ConvertToDxlibVec(map.GetMapPoint(i)), kCubeSize, (x_index + y_index) % 2 == 0 ? kColorGray : kColorLightGray);
	}
}
