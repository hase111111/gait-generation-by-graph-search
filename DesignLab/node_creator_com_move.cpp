
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "node_creator_com_move.h"

#include "math_util.h"
#include "graph_search_const.h"
#include "leg_state.h"


namespace designlab
{

NodeCreatorComMove::NodeCreatorComMove(
  const DividedMapState& map,
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
  const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
  const enums::HexapodMove next_move
) :
    map_(map),
    maker_(converter_ptr),
    selector_(checker_ptr),
    next_move_(next_move),
    converter_ptr_(converter_ptr),
    presenter_ptr_(presenter_ptr),
    checker_ptr_(checker_ptr)
{
}


void NodeCreatorComMove::Create(const RobotStateNode& current_node,
                                const int current_num,
                                std::vector<RobotStateNode>* output_graph) const
{
    std::array<ComPosAndPolygon, ComCandidatePolygonMaker::kMakePolygonNum> candidate_polygons;

    // 重心移動先の候補地点の範囲を示す多角形を作成する．
    maker_.MakeCandidatePolygon(current_node, &candidate_polygons);

    // 候補範囲から実際に移動する先の座標を選択する．
    for (int i = 0; i < ComCandidatePolygonMaker::kMakePolygonNum; ++i)
    {
        // そもそも多角形が候補点になりえないならば，その多角形は無視する．
        if (!candidate_polygons[i].is_able) { continue; }

        Vector3 result_com;

        if (selector_.GetComFromPolygon(candidate_polygons[i].polygon, current_node, &result_com))
        {
            RobotStateNode next_node = current_node;

            // 重心位置を変更し，それに伴い接地脚の位置も変更する．
            next_node.ChangeGlobalCenterOfMass(result_com, false);

            // leg_state の com_pattern を変更する
            leg_func::ChangeDiscreteComPos(candidate_polygons[i].com_pos, &next_node.leg_state);

            for (int j = 0; j < HexapodConst::kLegNum; ++j)
            {
                leg_func::ChangeDiscreteLegPos(j, enums::DiscreteLegPos::kCenter,
                                               &next_node.leg_state);
            }

            next_node.ChangeToNextNode(current_num, next_move_);  // 深さや親ノードを変更する

            if (checker_ptr_->IsStable(next_node.leg_state, next_node.leg_pos) &&
                !checker_ptr_->IsBodyInterferingWithGround(next_node, map_))
            {
                (*output_graph).push_back(next_node);
            }
        }
    }

    // 結果が空だったら現在姿勢をそのまま追加する．
    if ((*output_graph).empty())
    {
        RobotStateNode next_node = current_node;
        next_node.ChangeToNextNode(current_num, next_move_);

        (*output_graph).push_back(next_node);
    }
}

}  // namespace designlab
