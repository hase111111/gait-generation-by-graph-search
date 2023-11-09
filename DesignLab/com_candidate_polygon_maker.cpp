#include "com_candidate_polygon_maker.h"

#include "cassert_define.h"
#include "designlab_line_segment2.h"
#include "hexapod_const.h"


ComCandidatePolygonMaker::ComCandidatePolygonMaker(const std::shared_ptr<const AbstractHexapodStateCalculator>& calc) : 
	calculator_ptr_(calc)
{
}

void ComCandidatePolygonMaker::MakeCandidatePolygon(const RobotStateNode& node, std::array<ComPosAndPolygon, MAKE_POLYGON_NUM>* output_poly) const
{
	assert(calculator_ptr_ != nullptr);	//nullptrでないことを確認する
	assert(output_poly != nullptr);		//nullptrでないことを確認する


	std::array<designlab::Vector2, HexapodConst::kLegNum> leg_pos_xy;	//XY平面に射影した脚位置を算出する

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		leg_pos_xy[i] = calculator_ptr_->ConvertLegToGlobalCoordinate(node.leg_pos[i], i, node.global_center_of_mass, node.rot, false).ProjectedXY();		//脚位置(グローバル座標)をXY平面に射影する
	}

	//中心を囲むように4角形を作成する
	MakeCandidateBox(leg_pos_xy, 0, &(*output_poly)[0].polygon); 
	(*output_poly)[0].com_pos = DiscreteComPos::kFrontLeft;
	(*output_poly)[0].is_able = true;

	MakeCandidateBox(leg_pos_xy, 1, &(*output_poly)[1].polygon); 
	(*output_poly)[1].com_pos = DiscreteComPos::kBackLeft;
	(*output_poly)[1].is_able = true;

	MakeCandidateBox(leg_pos_xy, 2, &(*output_poly)[2].polygon); 
	(*output_poly)[2].com_pos = DiscreteComPos::kBack;
	(*output_poly)[2].is_able = true;

	MakeCandidateBox(leg_pos_xy, 3, &(*output_poly)[3].polygon);
	(*output_poly)[3].com_pos = DiscreteComPos::kBackRight;
	(*output_poly)[3].is_able = true;

	MakeCandidateBox(leg_pos_xy, 4, &(*output_poly)[4].polygon); 
	(*output_poly)[4].com_pos = DiscreteComPos::kFrontRight;
	(*output_poly)[4].is_able = true;

	MakeCandidateBox(leg_pos_xy, 5, &(*output_poly)[5].polygon); 
	(*output_poly)[5].com_pos = DiscreteComPos::kFront;
	(*output_poly)[5].is_able = true;

	//中心に3角形を作成する
	MakeCandidateTriangle(leg_pos_xy, &(*output_poly)[6]);

	//生成した多角形が正しいかどうかをチェックし，異常なものは削除する
	if (kDoCheckPolygon)
	{
		for (int i = 0; i < MAKE_POLYGON_NUM; ++i)
		{
			if (!IsAblePolygon((*output_poly)[i].polygon))
			{
				(*output_poly)[i].is_able = false;
			}
		}
	}

}

void ComCandidatePolygonMaker::MakeCandidateBox(const std::array<designlab::Vector2, HexapodConst::kLegNum>& leg_pos, const int start_leg_num, designlab::Polygon2* output_poly) const
{
	//脚位置を線で結ぶ．この交点から重心候補地点が存在する多角形を求める
	designlab::LineSegment2 leg_line_02(leg_pos[(start_leg_num + 0) % HexapodConst::kLegNum], leg_pos[(start_leg_num + 2) % HexapodConst::kLegNum]);
	designlab::LineSegment2 leg_line_03(leg_pos[(start_leg_num + 0) % HexapodConst::kLegNum], leg_pos[(start_leg_num + 3) % HexapodConst::kLegNum]);
	designlab::LineSegment2 leg_line_14(leg_pos[(start_leg_num + 1) % HexapodConst::kLegNum], leg_pos[(start_leg_num + 4) % HexapodConst::kLegNum]);
	designlab::LineSegment2 leg_line_15(leg_pos[(start_leg_num + 1) % HexapodConst::kLegNum], leg_pos[(start_leg_num + 5) % HexapodConst::kLegNum]);
	designlab::LineSegment2 leg_line_25(leg_pos[(start_leg_num + 2) % HexapodConst::kLegNum], leg_pos[(start_leg_num + 5) % HexapodConst::kLegNum]);

	//交点(intersection)を求める
	designlab::Vector2 intersection_02_14 = leg_line_02.GetIntersection(leg_line_14);
	designlab::Vector2 intersection_02_15 = leg_line_02.GetIntersection(leg_line_15);
	designlab::Vector2 intersection_03_14 = leg_line_03.GetIntersection(leg_line_14);
	designlab::Vector2 intersection_03_15 = leg_line_03.GetIntersection(leg_line_15);

	//中心と0番の脚位置を結んだ線分を求める
	designlab::LineSegment2 leg_line_0_center(leg_pos[(start_leg_num + 0) % HexapodConst::kLegNum], intersection_03_14);

	//多角形生成

	(*output_poly).Reset();

	if (leg_line_0_center.HasIntersection(leg_line_25))
	{
		//交点がある場合，5角形の多角形を作成する
		designlab::Vector2 intersection_03_25 = leg_line_03.GetIntersection(leg_line_25);
		designlab::Vector2 intersection_14_25 = leg_line_14.GetIntersection(leg_line_25);

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

void ComCandidatePolygonMaker::MakeCandidateTriangle(const std::array<designlab::Vector2, HexapodConst::kLegNum>& leg_pos, ComPosAndPolygon* output) const
{
	designlab::LineSegment2 leg_line_03(leg_pos[0], leg_pos[3]);
	designlab::LineSegment2 leg_line_14(leg_pos[1], leg_pos[4]);
	designlab::LineSegment2 leg_line_25(leg_pos[2], leg_pos[5]);

	//交点(intersection)を求める
	designlab::Vector2 intersection_03_14 = leg_line_03.GetIntersection(leg_line_14);
	designlab::Vector2 intersection_03_25 = leg_line_03.GetIntersection(leg_line_25);
	designlab::Vector2 intersection_14_25 = leg_line_14.GetIntersection(leg_line_25);

	//三角形を作成する．
	output->polygon.Reset();
	output->polygon.AddVertex(intersection_03_14);
	output->polygon.AddVertex(intersection_03_25);
	output->polygon.AddVertex(intersection_14_25);

	if (intersection_03_14.x > intersection_03_25.x)
	{
		output->com_pos = DiscreteComPos::kCenterBack;
	}
	else
	{
		output->com_pos = DiscreteComPos::kCenterFront;
	}

	output->is_able = true;

	return;
}

bool ComCandidatePolygonMaker::IsAblePolygon(const designlab::Polygon2& _poly) const
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