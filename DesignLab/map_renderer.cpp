#include "map_renderer.h"

#include "DxLib.h"

#include "Dxlib3DFunction.h"
#include "map_const.h"
#include "HexapodStateCalculator.h"


MapRenderer::MapRenderer() : COLOR_GRAY(GetColor(80, 80, 80)), COLOR_LIGHT_GRAY(GetColor(160, 160, 160))
{
}


void MapRenderer::setNode(const SNode& node)
{
	m_node = node;
}


void MapRenderer::draw(const MapState& map) const
{
	using myDxlib3DFunc::convertToDxVec;
	using myDxlib3DFunc::drawCube3DWithTopPos;

	HexapodStateCalclator _calc;
	int _x = map.getDevideMapNumX(_calc.getGlobalLegPos(m_node, 0, true).x);
	int _y = map.getDevideMapNumY(_calc.getGlobalLegPos(m_node, 0, true).y);

	for (int x = 0; x < MapConst::LP_DIVIDE_NUM; ++x)
	{
		for (int y = 0; y < MapConst::LP_DIVIDE_NUM; ++y)
		{
			for (int i = 0; i < map.getPointNumFromDevideMap(x, y); ++i)
			{

				if (false/*_x == x && _y == y*/) { drawCube3DWithTopPos(convertToDxVec(map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, GetColor(255, 0, 0)); }
				else
				{
					//縞縞模様を作成するために，位置に応じて色を変える．
					if ((x + y) % 2 == 0)
					{
						drawCube3DWithTopPos(convertToDxVec(map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, COLOR_GRAY);
					}
					else
					{
						drawCube3DWithTopPos(convertToDxVec(map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, COLOR_LIGHT_GRAY);
					}
				}

			}
		}
	}
}
