#include "ComCandidatePolygonMaker.h"
#include "MyLine.h"
#include "HexapodConst.h"
#include "HexapodStateCalculator.h"

void ComCandidatePolygonMaker::makeCandidatePolygon(const SNode& _node, std::vector<std::pair<my_vec::SPolygon2, ComType::EComPattern>>& _output_poly) const
{
	_output_poly.clear();	// outputをクリア

	my_vec::SVector2 _leg_pos_xy[HexapodConst::LEG_NUM];	//XY平面に射影した脚位置を算出する

	HexapodStateCalclator _calc;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		_leg_pos_xy[i] = _calc.getGlobalLegPos(_node, i).projectedXY();		//脚位置(グローバル座標)をXY平面に射影する
	}

	//中心を囲むように4角形を作成する
	_output_poly.push_back(std::make_pair<my_vec::SPolygon2, ComType::EComPattern>(makeCandidateBox(_leg_pos_xy, 0), ComType::EComPattern::left_front));
	_output_poly.push_back(std::make_pair<my_vec::SPolygon2, ComType::EComPattern>(makeCandidateBox(_leg_pos_xy, 1), ComType::EComPattern::left_back));
	_output_poly.push_back(std::make_pair<my_vec::SPolygon2, ComType::EComPattern>(makeCandidateBox(_leg_pos_xy, 2), ComType::EComPattern::back));
	_output_poly.push_back(std::make_pair<my_vec::SPolygon2, ComType::EComPattern>(makeCandidateBox(_leg_pos_xy, 3), ComType::EComPattern::right_back));
	_output_poly.push_back(std::make_pair<my_vec::SPolygon2, ComType::EComPattern>(makeCandidateBox(_leg_pos_xy, 4), ComType::EComPattern::right_front));
	_output_poly.push_back(std::make_pair<my_vec::SPolygon2, ComType::EComPattern>(makeCandidateBox(_leg_pos_xy, 5), ComType::EComPattern::front));

	//中心に3角形を作成する
	my_vec::SPolygon2 _poly;
	ComType::EComPattern _com_pattern;
	makeCandidateTriangle(_leg_pos_xy, _poly, _com_pattern);
	_output_poly.push_back({ _poly, _com_pattern });

	//生成した多角形が正しいかどうかをチェックし，異常なものは削除する
	for (auto it = _output_poly.begin(); it != _output_poly.end();)
	{
		if (it->first.isConvex() == false)
		{
			it = _output_poly.erase(it);
		}
		else
		{
			it++;
		}
	}

	//全ての多角形をcoutで出力する
	if (DO_DEBUG_OUTPUT)
	{
		for (const auto& i : _output_poly)
		{
			std::cout << "ComCandidatePolygonMaker::makeCandidatePolygon() : " << i.first << std::endl;
		}
	}
}

my_vec::SPolygon2 ComCandidatePolygonMaker::makeCandidateBox(const my_vec::SVector2 _leg_pos[HexapodConst::LEG_NUM], const int _start_leg_num) const
{
	using my_vec::SLine2;
	using my_vec::SPolygon2;
	using my_vec::SVector2;

	//脚位置を線で結ぶ．この交点から重心候補地点が存在する多角形を求める
	SLine2 _leg_line_02(_leg_pos[(_start_leg_num + 0) % HexapodConst::LEG_NUM], _leg_pos[(_start_leg_num + 2) % HexapodConst::LEG_NUM]);
	SLine2 _leg_line_03(_leg_pos[(_start_leg_num + 0) % HexapodConst::LEG_NUM], _leg_pos[(_start_leg_num + 3) % HexapodConst::LEG_NUM]);
	SLine2 _leg_line_14(_leg_pos[(_start_leg_num + 1) % HexapodConst::LEG_NUM], _leg_pos[(_start_leg_num + 4) % HexapodConst::LEG_NUM]);
	SLine2 _leg_line_15(_leg_pos[(_start_leg_num + 1) % HexapodConst::LEG_NUM], _leg_pos[(_start_leg_num + 5) % HexapodConst::LEG_NUM]);
	SLine2 _leg_line_25(_leg_pos[(_start_leg_num + 2) % HexapodConst::LEG_NUM], _leg_pos[(_start_leg_num + 5) % HexapodConst::LEG_NUM]);

	//交点を求める
	SVector2 _intersection_02_14 = _leg_line_02.getIntersection(_leg_line_14);
	SVector2 _intersection_02_15 = _leg_line_02.getIntersection(_leg_line_15);
	SVector2 _intersection_03_14 = _leg_line_03.getIntersection(_leg_line_14);
	SVector2 _intersection_03_15 = _leg_line_03.getIntersection(_leg_line_15);

	//中心と0番の脚位置を結んだ線分を求める
	SLine2 _leg_line_0_center(_leg_pos[(_start_leg_num + 0) % HexapodConst::LEG_NUM], _intersection_03_14);

	SPolygon2 _poly;

	if (_leg_line_0_center.hasIntersection(_leg_line_25) == true)
	{
		//交点がある場合，5角形の多角形を作成する
		SVector2 _intersection_03_25 = _leg_line_03.getIntersection(_leg_line_25);
		SVector2 _intersection_14_25 = _leg_line_14.getIntersection(_leg_line_25);

		_poly.addVertexCheckForDuplicates(_intersection_03_15);
		_poly.addVertexCheckForDuplicates(_intersection_02_15);
		_poly.addVertexCheckForDuplicates(_intersection_02_14);
		_poly.addVertexCheckForDuplicates(_intersection_14_25);
		_poly.addVertexCheckForDuplicates(_intersection_03_25);
	}
	else
	{
		//交点がない場合，既に求めた4点で，4角形の多角形を作成する
		_poly.addVertexCheckForDuplicates(_intersection_03_15);
		_poly.addVertexCheckForDuplicates(_intersection_02_15);
		_poly.addVertexCheckForDuplicates(_intersection_02_14);
		_poly.addVertexCheckForDuplicates(_intersection_03_14);
	}

	return _poly;
}

void ComCandidatePolygonMaker::makeCandidateTriangle(const my_vec::SVector2 _leg_pos[HexapodConst::LEG_NUM], my_vec::SPolygon2& _out_poly, ComType::EComPattern& _out_com_pattern) const
{
	using my_vec::SLine2;
	using my_vec::SPolygon2;
	using my_vec::SVector2;

	SLine2 _leg_line_03(_leg_pos[0], _leg_pos[3]);
	SLine2 _leg_line_14(_leg_pos[1], _leg_pos[4]);
	SLine2 _leg_line_25(_leg_pos[2], _leg_pos[5]);

	//交点を求める
	SVector2 _intersection_03_14 = _leg_line_03.getIntersection(_leg_line_14);
	SVector2 _intersection_03_25 = _leg_line_03.getIntersection(_leg_line_25);
	SVector2 _intersection_14_25 = _leg_line_14.getIntersection(_leg_line_25);

	//三角形を作成する．
	SPolygon2 _poly;
	_poly.addVertexCheckForDuplicates(_intersection_03_14);
	_poly.addVertexCheckForDuplicates(_intersection_03_25);
	_poly.addVertexCheckForDuplicates(_intersection_14_25);

	_out_poly = _poly;

	if (_intersection_03_14.x > _intersection_03_25.x)
	{
		_out_com_pattern = ComType::EComPattern::center_back;
	}
	else
	{
		_out_com_pattern = ComType::EComPattern::center_front;
	}

	return;
}

bool ComCandidatePolygonMaker::checkPolygon(const my_vec::SPolygon2& _poly) const
{
	//生成されるのは 3 or 4 or 5角形のみ
	if (_poly.getVertexNum() == 3 || _poly.getVertexNum() == 4 || _poly.getVertexNum() == 5)
	{
		//凸多角形であるかを確認する
		if (_poly.isConvex() == true)
		{
			return true;
		}
	}

	return false;
}
