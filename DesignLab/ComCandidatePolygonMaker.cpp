#include "ComCandidatePolygonMaker.h"
#include "MyLine.h"
#include "HexapodConst.h"

void ComCandidatePolygonMaker::makeCandidatePolygon(const SNode& _node, std::vector<my_vec::SPolygon2>& _output_poly)
{
	using namespace my_vec;

	_output_poly.clear();	// outputをクリア


	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		SVector2 _leg_pos_xy[HexapodConst::LEG_NUM];	//XY平面に射影した脚位置

		for (int j = 0; j < HexapodConst::LEG_NUM; j++)
		{
			_leg_pos_xy[j] = _node.leg_pos[(i + 0) % HexapodConst::LEG_NUM].projectedXY();	//脚位置をXY平面に射影する
		}

		//脚位置を線で結ぶ．この交点から重心候補地点が存在する多角形を求める
		SLine2 _leg_line_02(_leg_pos_xy[0], _leg_pos_xy[2]);
		SLine2 _leg_line_03(_leg_pos_xy[0], _leg_pos_xy[3]);
		SLine2 _leg_line_14(_leg_pos_xy[1], _leg_pos_xy[4]);
		SLine2 _leg_line_15(_leg_pos_xy[1], _leg_pos_xy[5]);
		SLine2 _leg_line_25(_leg_pos_xy[2], _leg_pos_xy[5]);

		//交点を求める
		SVector2 _intersection_02_14 = _leg_line_02.getIntersection(_leg_line_14);
		SVector2 _intersection_02_15 = _leg_line_02.getIntersection(_leg_line_15);
		SVector2 _intersection_03_14 = _leg_line_03.getIntersection(_leg_line_14);
		SVector2 _intersection_03_15 = _leg_line_03.getIntersection(_leg_line_15);

		//中心と0番の脚位置を結んだ線分を求める
		SLine2 _leg_line_0_center(_leg_pos_xy[0], _intersection_03_14);

		if (_leg_line_0_center.hasIntersection(_leg_line_25) == true)
		{
			//交点がある場合，5角形の多角形を作成する
			SVector2 _intersection_03_25 = _leg_line_03.getIntersection(_leg_line_25);
			SVector2 _intersection_14_25 = _leg_line_14.getIntersection(_leg_line_25);

			SPolygon2 _poly;
			_poly.addVertex(_intersection_03_15);
			_poly.addVertex(_intersection_02_15);
			_poly.addVertex(_intersection_02_14);
			_poly.addVertex(_intersection_14_25);
			_poly.addVertex(_intersection_03_25);
			_output_poly.push_back(_poly);
		}
		else
		{
			//交点がない場合，既に求めた4点で，4角形の多角形を作成する
			SPolygon2 _poly;
			_poly.addVertex(_intersection_03_15);
			_poly.addVertex(_intersection_02_15);
			_poly.addVertex(_intersection_02_14);
			_poly.addVertex(_intersection_03_14);
			_output_poly.push_back(_poly);
		}

	}

}
