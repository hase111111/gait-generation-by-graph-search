
//! @author    Hasegawa
//! @copyright 埼玉大学 設計工学研究室 2023. All right reserved.

#include "node_creator_com_up_down.h"

#include <algorithm>
#include <cfloat>

#include "math_util.h"
#include "graph_search_const.h"
#include "hexapod_const.h"
#include "leg_state.h"
#include "phantomx_mk2_const.h"


namespace designlab
{

NodeCreatorComUpDown::NodeCreatorComUpDown(
  const DividedMapState& divided_map,
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
  const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
  enums::HexapodMove next_move) :
    map_(divided_map),
    converter_ptr_(converter_ptr),
    presenter_ptr_(presenter_ptr),
    checker_ptr_(checker_ptr),
    next_move_(next_move)
{
}


void NodeCreatorComUpDown::Create(const RobotStateNode& current_node, const int current_num,
                                  std::vector<RobotStateNode>* output_graph) const
{
    // 重心を最も高くあげることのできる位置と，最も低く下げることのできる位置を求める．

    // マップの最大z座標を求める．
    //! @todo この値は消す．
    float map_highest_z = -100000;

    if (map_.IsInMap(current_node.center_of_mass_global_coord))
    {
        const int map_index_x = map_.GetDividedMapIndexX(current_node.center_of_mass_global_coord.x);
        const int map_index_y = map_.GetDividedMapIndexY(current_node.center_of_mass_global_coord.y);
        map_highest_z = map_.GetTopZ(map_index_x, map_index_y);
    }

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        // 脚の先端の座標を求める．
        const Vector3 kCoxaVec = converter_ptr_->ConvertRobotToGlobalCoordinate(
            presenter_ptr_->GetLegBasePosRobotCoordinate(i),
            current_node.center_of_mass_global_coord,
            current_node.posture, true);

        if (map_.IsInMap(kCoxaVec))
        {
            const int kCoxaX = map_.GetDividedMapIndexX(kCoxaVec.x);
            const int kCoxaY = map_.GetDividedMapIndexY(kCoxaVec.y);
            map_highest_z = (std::max)(map_.GetTopZ(kCoxaX, kCoxaY), map_highest_z);
        }
    }


    // ロボットの重心の最も低く下げることのできるz座標と，
    // 高くあげることができるz座標を求める．どちらもグローバル座標．
    float highest_body_pos_z = map_highest_z + presenter_ptr_->GetGroundHeightMarginMax();
    float lowest_body_pos_z = map_highest_z + presenter_ptr_->GetGroundHeightMarginMin();


    // 最も高い地点を修正する．

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        // 接地している脚についてのみ考える．
        if (leg_func::IsGrounded(current_node.leg_state, i))
        {
            // 三平方の定理を使って，脚接地地点から重心位置をどれだけ上げられるか考える．
            const float edge_c =
                PhantomXMkIIConst::kFemurLength + PhantomXMkIIConst::kTibiaLength;
            const float edge_b =
                current_node.leg_pos[i].ProjectedXY().GetLength() - PhantomXMkIIConst::kCoxaLength;

            const float edge_a = sqrt(math_util::Squared(edge_c) - math_util::Squared(edge_b));

            // 接地脚の最大重心高さの中から一番小さいものを全体の最大重心位置として記録する．_aは脚の接地点からどれだけ上げられるかを表しているので，グローバル座標に変更する．
            highest_body_pos_z = (std::min)(
                edge_a + current_node.center_of_mass_global_coord.z + current_node.leg_pos[i].z, highest_body_pos_z);
        }
    }

    // ノードを追加する．
    pushNodeByMaxAndMinPosZ(current_node, current_num, highest_body_pos_z,
                            lowest_body_pos_z, output_graph);
}


void NodeCreatorComUpDown::pushNodeByMaxAndMinPosZ(
    const RobotStateNode& current_node, const int current_num, const float high,
    const float low, std::vector<RobotStateNode>* output_graph) const
{
    // 重心を変化させたものを追加する．変化量が一番少ないノードは削除する．

    // 最大と最小の間を分割する．
    const float kDivZ = (high - low) / static_cast<float>(kDiscretization);

    // 分割した分新しいノードを追加する．
    for (int i = 0; i < kDiscretization + 1; ++i)
    {
        bool is_valid = true;

        RobotStateNode new_node = current_node;

        // 重心の位置を変更する．
        Vector3 new_com = current_node.center_of_mass_global_coord;
        new_com.z = low + kDivZ * i;

        new_node.ChangeGlobalCenterOfMass(new_com, true);


        for (int j = 0; j < HexapodConst::kLegNum; j++)
        {
            if (!checker_ptr_->IsLegInRange(j, new_node.leg_pos[j]))
            {
                is_valid = false;
            }
        }

        // current_numを親とする，新しいノードに変更する
        new_node.ChangeToNextNode(current_num, next_move_);

        // ノードを追加する．
        if (is_valid)
        {
            (*output_graph).emplace_back(new_node);
        }
    }

    // 重心の変化が一切ないものを追加する．
    RobotStateNode same_node = current_node;

    same_node.ChangeToNextNode(current_num, next_move_);

    (*output_graph).emplace_back(same_node);
}

}  // namespace designlab
