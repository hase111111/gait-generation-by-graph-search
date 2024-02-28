
//! @author    Hasegawa
//! @copyright 埼玉大学 設計工学研究室 2023. All right reserved.

#include "com_selector.h"

#include <algorithm>
#include <iostream>

#include "math_util.h"
#include "leg_state.h"


namespace designlab
{

bool ComSelector::GetComFromPolygon(
    const Polygon2& polygon,
    const RobotStateNode& current_node,
    Vector3* output_com) const
{
    std::pair<bool, Vector2> com_candidate[kDiscretizationNum * kDiscretizationNum];

    // 候補点を生成する．
    if (!MakeComCandidatePoint(polygon, com_candidate))
    {
        return false;
    }

    // 頂点から次の頂点へ向かう辺を正規化したベクトルを作成する．
    std::vector<Vector2> edge_vec;
    edge_vec.resize(polygon.GetVertexNum());

    for (int i = 0; i < polygon.GetVertexNum(); ++i)
    {
        Vector2 edge = polygon.GetVertex(i) - polygon.GetVertex((i + 1) % polygon.GetVertexNum());
        edge_vec[i] = edge.GetNormalized();
    }

    // 候補点を順番にチェックし，移動後の重心が安定余裕を満たすならば，
    // その点を重心として採用する．
    for (int i = 0; i < kDiscretizationNum * kDiscretizationNum; ++i)
    {
        if (!IsInMargin(polygon, edge_vec, com_candidate[i].second))
        {
            // 候補点が多角形の外側ならば次の候補点へ．
            com_candidate[i].first = false;
            continue;
        }

        // 現在の重心を移動させたものを作成する．
        Vector3 after_move_com_pos = { com_candidate[i].second.x,
            com_candidate[i].second.y,
            current_node.center_of_mass_global_coord.z };
        const Vector3 dif = after_move_com_pos - current_node.center_of_mass_global_coord;
        const Vector3 real_dif = RotateVector3(dif, current_node.posture.GetConjugate());

        for (int j = 0; j < HexapodConst::kLegNum; ++j)
        {
            if (leg_func::IsGrounded(current_node.leg_state, j))
            {
                const Vector3 after_move_leg_pos = current_node.leg_pos[j] - real_dif;

                if (!checker_ptr_->IsLegInRange(j, after_move_leg_pos))
                {
                    // 脚が可動範囲外ならば次の候補点へ．
                    com_candidate[i].first = false;
                    continue;
                }
            }
        }
    }

    // 候補点の中から現在の重心から最も遠くに移動できるものを選択する．

    const Vector2 k_rotate_center = { -10000, 0 };
    const float k_rotate_r = 10000;

    float min_dist = -100000;
    int min_index = -1;

    for (int i = 0; i < kDiscretizationNum * kDiscretizationNum; ++i)
    {
        if (com_candidate[i].first)
        {
            const float dist = fabsf(
                (com_candidate[i].second - k_rotate_center).GetLength() - k_rotate_r);

            if (min_dist < dist)
            {
                min_dist = dist;
                min_index = i;
            }
        }
    }

    if (min_index == -1)
    {
        // 該当するものがなければ false を返す．
        return false;
    }

    (*output_com).x = com_candidate[min_index].second.x;
    (*output_com).y = com_candidate[min_index].second.y;
    (*output_com).z = current_node.center_of_mass_global_coord.z;
    return true;
}


bool ComSelector::MakeComCandidatePoint(
    const Polygon2& polygon,
    std::pair<bool, Vector2> coms[kDiscretizationNum * kDiscretizationNum]) const
{
    // 波東さんの処理では多角形を囲むような四角形を作るので，まずはそれを作る．
    const float kMinX = polygon.GetMinX();
    const float kMaxX = polygon.GetMaxX();
    const float kMinY = polygon.GetMinY();
    const float kMaxY = polygon.GetMaxY();

    const float kWidth = kMaxX - kMinX;
    const float kHeight = kMaxY - kMinY;

    if (math_util::IsEqual(kWidth, 0.0f) || math_util::IsEqual(kHeight, 0.0f))
    {
        return false;
    }

    const float kDeltaWidth = kWidth / static_cast<float>(kDiscretizationNum);
    const float kDeltaHeight = kHeight / static_cast<float>(kDiscretizationNum);

    // 上記の四角形の中にある点を全て候補に追加する．
    for (int x = 0; x < kDiscretizationNum; ++x)
    {
        for (int y = 0; y < kDiscretizationNum; ++y)
        {
            coms[x * kDiscretizationNum + y].first = true;
            coms[x * kDiscretizationNum + y].second.x = kMinX + kDeltaWidth * x;
            coms[x * kDiscretizationNum + y].second.y = kMinY + kDeltaHeight * y;
        }
    }

    return true;
}


bool ComSelector::IsInMargin(const Polygon2& polygon,
                                 const std::vector<Vector2>& edge_vec,
                                 const Vector2& candidate_point) const
{
    for (int i = 0; i < polygon.GetVertexNum(); ++i)
    {
        Vector2 v_map = candidate_point - polygon.GetVertex(i);

        if (v_map.Cross(edge_vec[i]) > -kStabilityMargin)
        {
            // 安定余裕を満たさないならば候補から削除する．
            return false;
        }
    }

    return true;
}

}  // namespace designlab
