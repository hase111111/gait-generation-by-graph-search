#include "com_candidate_polygon_maker.h"

#include "designlab_line_segment2.h"
#include "hexapod_const.h"


void ComCandidatePolygonMaker::makeCandidatePolygon(const SNode& node, std::pair<designlab::Polygon2, EDiscreteComPos> output_poly[MAKE_POLYGON_NUM]) const
{
	if (!mp_calculator) { return; }


	designlab::SVector2 leg_pos_xy[HexapodConst::LEG_NUM];	//XY平面に射影した脚位置を算出する

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		leg_pos_xy[i] = mp_calculator->getGlobalLegPosition(i, node.leg_pos[i], node.global_center_of_mass, node.rot, false).ProjectedXY();		//脚位置(グローバル座標)をXY平面に射影する
	}

	//中心を囲むように4角形を作成する
	makeCandidateBox(leg_pos_xy, 0, &output_poly[0].first); output_poly[0].second = EDiscreteComPos::FRONT_LEFT;
	makeCandidateBox(leg_pos_xy, 1, &output_poly[1].first); output_poly[1].second = EDiscreteComPos::BACK_LEFT;
	makeCandidateBox(leg_pos_xy, 2, &output_poly[2].first); output_poly[2].second = EDiscreteComPos::BACK;
	makeCandidateBox(leg_pos_xy, 3, &output_poly[3].first); output_poly[3].second = EDiscreteComPos::BACK_RIGHT;
	makeCandidateBox(leg_pos_xy, 4, &output_poly[4].first); output_poly[4].second = EDiscreteComPos::FRONT_RIGHT;
	makeCandidateBox(leg_pos_xy, 5, &output_poly[5].first); output_poly[5].second = EDiscreteComPos::FRONT;

	//中心に3角形を作成する
	makeCandidateTriangle(leg_pos_xy, &output_poly[6].first, &output_poly[6].second);

	//生成した多角形が正しいかどうかをチェックし，異常なものは削除する
	if (DO_CHECK_POLYGON)
	{
		for (int i = 0; i < MAKE_POLYGON_NUM; ++i)
		{
			if (!checkPolygon(output_poly[i].first))
			{
				output_poly[i].second = EDiscreteComPos::ERROR_POS;
			}
		}
	}

	//全ての多角形をcoutで出力する
	if (DO_DEBUG_PRINT)
	{
		for (int i = 0; i < MAKE_POLYGON_NUM; ++i)
		{
			std::cout << "ComCandidatePolygonMaker::makeCandidatePolygon() : " << output_poly[i].first << std::endl;
		}
	}
}

void ComCandidatePolygonMaker::makeCandidateBox(const designlab::SVector2 leg_pos[HexapodConst::LEG_NUM], const int start_leg_num, designlab::Polygon2* output_poly) const
{
	//脚位置を線で結ぶ．この交点から重心候補地点が存在する多角形を求める
	designlab::LineSegment2 leg_line_02(leg_pos[(start_leg_num + 0) % HexapodConst::LEG_NUM], leg_pos[(start_leg_num + 2) % HexapodConst::LEG_NUM]);
	designlab::LineSegment2 leg_line_03(leg_pos[(start_leg_num + 0) % HexapodConst::LEG_NUM], leg_pos[(start_leg_num + 3) % HexapodConst::LEG_NUM]);
	designlab::LineSegment2 leg_line_14(leg_pos[(start_leg_num + 1) % HexapodConst::LEG_NUM], leg_pos[(start_leg_num + 4) % HexapodConst::LEG_NUM]);
	designlab::LineSegment2 leg_line_15(leg_pos[(start_leg_num + 1) % HexapodConst::LEG_NUM], leg_pos[(start_leg_num + 5) % HexapodConst::LEG_NUM]);
	designlab::LineSegment2 leg_line_25(leg_pos[(start_leg_num + 2) % HexapodConst::LEG_NUM], leg_pos[(start_leg_num + 5) % HexapodConst::LEG_NUM]);

	//交点(intersection)を求める
	designlab::SVector2 intersection_02_14 = leg_line_02.GetIntersection(leg_line_14);
	designlab::SVector2 intersection_02_15 = leg_line_02.GetIntersection(leg_line_15);
	designlab::SVector2 intersection_03_14 = leg_line_03.GetIntersection(leg_line_14);
	designlab::SVector2 intersection_03_15 = leg_line_03.GetIntersection(leg_line_15);

	//中心と0番の脚位置を結んだ線分を求める
	designlab::LineSegment2 leg_line_0_center(leg_pos[(start_leg_num + 0) % HexapodConst::LEG_NUM], intersection_03_14);

	//多角形生成

	(*output_poly).Reset();

	if (leg_line_0_center.HasIntersection(leg_line_25))
	{
		//交点がある場合，5角形の多角形を作成する
		designlab::SVector2 intersection_03_25 = leg_line_03.GetIntersection(leg_line_25);
		designlab::SVector2 intersection_14_25 = leg_line_14.GetIntersection(leg_line_25);

		(*output_poly).AddVertexCheckForDuplicates(intersection_03_15);
		(*output_poly).AddVertexCheckForDuplicates(intersection_02_15);
		(*output_poly).AddVertexCheckForDuplicates(intersection_02_14);
		(*output_poly).AddVertexCheckForDuplicates(intersection_14_25);
		(*output_poly).AddVertexCheckForDuplicates(intersection_03_25);
	}
	else
	{
		//交点がない場合，既に求めた4点で，4角形の多角形を作成する
		(*output_poly).AddVertex(intersection_03_15);
		(*output_poly).AddVertex(intersection_02_15);
		(*output_poly).AddVertex(intersection_02_14);
		(*output_poly).AddVertex(intersection_03_14);
	}
}

void ComCandidatePolygonMaker::makeCandidateTriangle(const designlab::SVector2 leg_pos[HexapodConst::LEG_NUM], designlab::Polygon2* out_poly, EDiscreteComPos* out_com_pattern) const
{
	designlab::LineSegment2 leg_line_03(leg_pos[0], leg_pos[3]);
	designlab::LineSegment2 leg_line_14(leg_pos[1], leg_pos[4]);
	designlab::LineSegment2 leg_line_25(leg_pos[2], leg_pos[5]);

	//交点(intersection)を求める
	designlab::SVector2 intersection_03_14 = leg_line_03.GetIntersection(leg_line_14);
	designlab::SVector2 intersection_03_25 = leg_line_03.GetIntersection(leg_line_25);
	designlab::SVector2 intersection_14_25 = leg_line_14.GetIntersection(leg_line_25);

	//三角形を作成する．
	(*out_poly).Reset();
	(*out_poly).AddVertex(intersection_03_14);
	(*out_poly).AddVertex(intersection_03_25);
	(*out_poly).AddVertex(intersection_14_25);

	if (intersection_03_14.x > intersection_03_25.x)
	{
		(*out_com_pattern) = EDiscreteComPos::CENTER_BACK;
	}
	else
	{
		(*out_com_pattern) = EDiscreteComPos::CENTER_FRONT;
	}

	return;
}

bool ComCandidatePolygonMaker::checkPolygon(const designlab::Polygon2& _poly) const
{
	//生成されるのは 3 or 4 or 5角形のみ
	if (_poly.GetVertexNum() == 3 || _poly.GetVertexNum() == 4 || _poly.GetVertexNum() == 5)
	{
		//凸多角形であるかを確認する
		if (_poly.IsConvex())
		{
			return true;
		}
	}

	return false;
}
