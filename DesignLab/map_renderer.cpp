#include "map_renderer.h"

#include "DxLib.h"

#include "dxlib_util.h"
#include "hexapod_state_calculator.h"
#include "map_const.h"


namespace dldu = designlab::dxlib_util;


MapRenderer::MapRenderer() : COLOR_GRAY(GetColor(80, 80, 80)), COLOR_LIGHT_GRAY(GetColor(160, 160, 160))
{
}


void MapRenderer::Draw(const MapState_Old& map) const
{
	for (int x = 0; x < MapConst::LP_DIVIDE_NUM; ++x)
	{
		for (int y = 0; y < MapConst::LP_DIVIDE_NUM; ++y)
		{
			for (int i = 0; i < map.getPointNumFromDevideMap(x, y); ++i)
			{

				//縞縞模様を作成するために，位置に応じて色を変える．
				if ((x + y) % 2 == 0)
				{
					dldu::DrawCube3DWithTopPos(dldu::ConvertToDxlibVec(map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, COLOR_GRAY);
				}
				else
				{
					dldu::DrawCube3DWithTopPos(dldu::ConvertToDxlibVec(map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, COLOR_LIGHT_GRAY);
				}

			}
		}
	}
}
