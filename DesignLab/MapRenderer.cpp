#include "MapRenderer.h"
#include "DxLib.h"
#include "Dxlib3DFunction.h"
#include "MapConst.h"

MapRenderer::MapRenderer() : COLOR_GRAY(GetColor(80, 80, 80)), COLOR_LIGHT_GRAY(GetColor(160, 160, 160))
{
}

void MapRenderer::draw(const MapState& _map) const
{
	using namespace myDxlib3DFunc;

	for (int x = 0; x < MapConst::LP_DIVIDE_NUM; x++)
	{
		for (int y = 0; y < MapConst::LP_DIVIDE_NUM; y++)
		{
			for (int i = 0; i < _map.getPointNumFromDevideMap(x,y); i++)
			{

				//縞縞模様を作成するために，位置に応じて色を変える．
				if ((x + y) % 2 == 0) 
				{
					drawCube3DWithTopPos(convertToDxVec(_map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, COLOR_GRAY);
				}
				else
				{
					drawCube3DWithTopPos(convertToDxVec(_map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, COLOR_LIGHT_GRAY);
				}

			}
		}
	}
}
