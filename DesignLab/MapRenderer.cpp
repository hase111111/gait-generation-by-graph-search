#include "MapRenderer.h"
#include "DxLib.h"
#include "Dxlib3DFunction.h"
#include "MapConst.h"

#include "HexapodStateCalculator.h"

MapRenderer::MapRenderer() : COLOR_GRAY(GetColor(80, 80, 80)), COLOR_LIGHT_GRAY(GetColor(160, 160, 160))
{
}

void MapRenderer::setNode(const SNode& _node)
{
	m_node = _node;
}

void MapRenderer::draw(const MapState& _map) const
{
	using namespace myDxlib3DFunc;

	HexapodStateCalclator _calc;
	int _x = _map.getDevideMapNumX(_calc.getGlobalLegPos(m_node, 0, true).x);
	int _y = _map.getDevideMapNumY(_calc.getGlobalLegPos(m_node, 0, true).y);

	for (int x = 0; x < MapConst::LP_DIVIDE_NUM; x++)
	{
		for (int y = 0; y < MapConst::LP_DIVIDE_NUM; y++)
		{
			for (int i = 0; i < _map.getPointNumFromDevideMap(x, y); i++)
			{

				if (false/*_x == x && _y == y*/) { drawCube3DWithTopPos(convertToDxVec(_map.getPosFromDevideMap(x, y, i)), CUBE_SIZE, GetColor(255, 0, 0)); }
				else
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
}
