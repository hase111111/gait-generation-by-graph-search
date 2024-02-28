
//! @author    Hasegawa
//! @copyright 埼玉大学 設計工学研究室 2023. All right reserved.

#include "com_candidate_polygon_maker.h"

#include "cassert_define.h"
#include "math_line_segment2.h"
#include "hexapod_const.h"


namespace designlab
{

ComCandidatePolygonMaker::ComCandidatePolygonMaker(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr) :
    converter_ptr_(converter_ptr)
{
}

void ComCandidatePolygonMaker::MakeCandidatePolygon(
    const RobotStateNode& node,
    std::array<ComPosAndPolygon, kMakePolygonNum>* output_poly) const
{
    assert(converter_ptr_ != nullptr);  // nullptr でないことを確認する．
    assert(output_poly != nullptr);  // nullptr でないことを確認する．

    // XY平面に射影した脚位置を算出する．(グローバル座標系)
    std::array<Vector2, HexapodConst::kLegNum> leg_pos_xy;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        // 脚位置(グローバル座標)をXY平面に射影する．
        leg_pos_xy[i] = converter_ptr_->ConvertLegToGlobalCoordinate(
            node.leg_pos[i],
            i,
            node.center_of_mass_global_coord,
            node.posture,
            true).ProjectedXY();
    }

    // 中心を囲むように4角形を作成する．
    MakeCandidateBox(leg_pos_xy, 0, &(*output_poly)[0].polygon);
    (*output_poly)[0].com_pos = enums::DiscreteComPos::kFrontLeft;
    (*output_poly)[0].is_able = true;

    MakeCandidateBox(leg_pos_xy, 1, &(*output_poly)[1].polygon);
    (*output_poly)[1].com_pos = enums::DiscreteComPos::kBackLeft;
    (*output_poly)[1].is_able = true;

    MakeCandidateBox(leg_pos_xy, 2, &(*output_poly)[2].polygon);
    (*output_poly)[2].com_pos = enums::DiscreteComPos::kBack;
    (*output_poly)[2].is_able = true;

    MakeCandidateBox(leg_pos_xy, 3, &(*output_poly)[3].polygon);
    (*output_poly)[3].com_pos = enums::DiscreteComPos::kBackRight;
    (*output_poly)[3].is_able = true;

    MakeCandidateBox(leg_pos_xy, 4, &(*output_poly)[4].polygon);
    (*output_poly)[4].com_pos = enums::DiscreteComPos::kFrontRight;
    (*output_poly)[4].is_able = true;

    MakeCandidateBox(leg_pos_xy, 5, &(*output_poly)[5].polygon);
    (*output_poly)[5].com_pos = enums::DiscreteComPos::kFront;
    (*output_poly)[5].is_able = true;

    // 中心に3角形を作成する．
    MakeCandidateTriangle(leg_pos_xy, &(*output_poly)[6]);

    // 生成した多角形が正しいかどうかをチェックし，異常なものは削除する．
    if (kDoCheckPolygon)
    {
        for (int i = 0; i < kMakePolygonNum; ++i)
        {
            if (!IsAblePolygon((*output_poly)[i].polygon))
            {
                (*output_poly)[i].is_able = false;
            }
        }
    }
}

void ComCandidatePolygonMaker::MakeCandidateBox(
    const std::array<Vector2, HexapodConst::kLegNum>& leg_pos,
    const int start_leg_num,
    Polygon2* output_poly) const
{
    // 脚位置を線で結ぶ．この交点から重心候補地点が存在する多角形を求める．
    LineSegment2 leg_line_02(
        leg_pos[(start_leg_num + 0) % HexapodConst::kLegNum],
        leg_pos[(start_leg_num + 2) % HexapodConst::kLegNum]);

    LineSegment2 leg_line_03(
        leg_pos[(start_leg_num + 0) % HexapodConst::kLegNum],
        leg_pos[(start_leg_num + 3) % HexapodConst::kLegNum]);

    LineSegment2 leg_line_14(
        leg_pos[(start_leg_num + 1) % HexapodConst::kLegNum],
        leg_pos[(start_leg_num + 4) % HexapodConst::kLegNum]);

    LineSegment2 leg_line_15(
        leg_pos[(start_leg_num + 1) % HexapodConst::kLegNum],
        leg_pos[(start_leg_num + 5) % HexapodConst::kLegNum]);

    LineSegment2 leg_line_25(
        leg_pos[(start_leg_num + 2) % HexapodConst::kLegNum],
        leg_pos[(start_leg_num + 5) % HexapodConst::kLegNum]);

    // 交点(intersection)を求める．
    Vector2 intersection_02_14 = leg_line_02.GetIntersection(leg_line_14);
    Vector2 intersection_02_15 = leg_line_02.GetIntersection(leg_line_15);
    Vector2 intersection_03_14 = leg_line_03.GetIntersection(leg_line_14);
    Vector2 intersection_03_15 = leg_line_03.GetIntersection(leg_line_15);

    // 中心と0番の脚位置を結んだ線分を求める．
    LineSegment2 leg_line_0_center(
        leg_pos[(start_leg_num + 0) % HexapodConst::kLegNum], intersection_03_14);

    // 多角形生成．

    (*output_poly).Reset();

    if (leg_line_0_center.HasIntersection(leg_line_25))
    {
        // 交点がある場合，5角形の多角形を作成する．
        Vector2 intersection_03_25 = leg_line_03.GetIntersection(leg_line_25);
        Vector2 intersection_14_25 = leg_line_14.GetIntersection(leg_line_25);

        (*output_poly).AddVertexCheckForDuplicates(intersection_03_15);
        (*output_poly).AddVertexCheckForDuplicates(intersection_02_15);
        (*output_poly).AddVertexCheckForDuplicates(intersection_02_14);
        (*output_poly).AddVertexCheckForDuplicates(intersection_14_25);
        (*output_poly).AddVertexCheckForDuplicates(intersection_03_25);
    }
    else
    {
        // 交点がない場合，既に求めた4点で，4角形の多角形を作成する．
        (*output_poly).AddVertex(intersection_03_15);
        (*output_poly).AddVertex(intersection_02_15);
        (*output_poly).AddVertex(intersection_02_14);
        (*output_poly).AddVertex(intersection_03_14);
    }
}

void ComCandidatePolygonMaker::MakeCandidateTriangle(
    const std::array<Vector2, HexapodConst::kLegNum>& leg_pos,
    ComPosAndPolygon* output) const
{
    LineSegment2 leg_line_03(leg_pos[0], leg_pos[3]);
    LineSegment2 leg_line_14(leg_pos[1], leg_pos[4]);
    LineSegment2 leg_line_25(leg_pos[2], leg_pos[5]);

    // 交点(intersection)を求める．
    Vector2 intersection_03_14 = leg_line_03.GetIntersection(leg_line_14);
    Vector2 intersection_03_25 = leg_line_03.GetIntersection(leg_line_25);
    Vector2 intersection_14_25 = leg_line_14.GetIntersection(leg_line_25);

    // 三角形を作成する．
    output->polygon.Reset();
    output->polygon.AddVertex(intersection_03_14);
    output->polygon.AddVertex(intersection_03_25);
    output->polygon.AddVertex(intersection_14_25);

    if (intersection_03_14.x > intersection_03_25.x)
    {
        output->com_pos = enums::DiscreteComPos::kCenterBack;
    }
    else
    {
        output->com_pos = enums::DiscreteComPos::kCenterFront;
    }

    output->is_able = true;

    return;
}

bool ComCandidatePolygonMaker::IsAblePolygon(const Polygon2& poly) const
{
    // 生成されるのは 3 or 4 or 5角形のみ．
    if (poly.GetVertexNum() == 3 ||
        poly.GetVertexNum() == 4 ||
        poly.GetVertexNum() == 5)
    {
        // 凸多角形であるかを確認する．
        if (poly.IsConvex())
        {
            return true;
        }
    }

    return false;
}

}  // namespace designlab
