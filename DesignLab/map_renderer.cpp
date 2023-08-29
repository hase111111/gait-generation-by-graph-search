#include "map_renderer.h"

#include "DxLib.h"

#include "designlab_dxlib.h"
#include "map_const.h"
#include "hexapod_state_calculator.h"


MapRenderer::MapRenderer() : COLOR_GRAY(GetColor(80, 80, 80)), COLOR_LIGHT_GRAY(GetColor(160, 160, 160))
{
}



void MapRenderer::draw(const MapState& map) const
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
					dl_dxlib::drawCube3DWithTopPos(dl_dxlib::convertToDxVec(map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, COLOR_GRAY);
				}
				else
				{
					dl_dxlib::drawCube3DWithTopPos(dl_dxlib::convertToDxVec(map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, COLOR_LIGHT_GRAY);
				}

			}
		}
	}
}
