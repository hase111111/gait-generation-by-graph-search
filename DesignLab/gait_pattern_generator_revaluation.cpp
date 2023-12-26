
/// @author    hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "gait_pattern_generator_revaluation.h"

#include <utility>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "gait_pattern_graph_tree.h"
#include "graph_search_const.h"
#include "map_state.h"


namespace designlab
{

GaitPatternGeneratorRevaluation::GaitPatternGeneratorRevaluation(
    std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
    std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_revaluation_ptr) :
    gait_pattern_generator_ptr_(std::move(gait_pattern_generator_ptr)),
    gait_pattern_generator_revaluation_ptr_(std::move(gait_pattern_generator_revaluation_ptr))
{
    // gait_pattern_generator_ptr_ は nullptrでない．
    assert(gait_pattern_generator_ptr_ != nullptr);

    // gait_pattern_generator_revaluation_ptr_ は nullptrでない．
    assert(gait_pattern_generator_revaluation_ptr_ != nullptr);
}

GraphSearchResult GaitPatternGeneratorRevaluation::GetNextNodeByGraphSearch(
  const RobotStateNode& current_node,
  const MapState& map_state,
  const RobotOperation& operation,
  RobotStateNode* output_node
)
{
    assert(output_node != nullptr);  // output_nodeは nullptrでない

    const GraphSearchResult result =
        gait_pattern_generator_ptr_->GetNextNodeByGraphSearch(current_node,
                                                              map_state,
                                                              operation,
                                                              output_node);

    if (result.result != enums::Result::kSuccess)
    {
        // グラフ探索に失敗した場合は終了．
        return result;
    }

    // 成功した場合は，逆運動学計算で脚軌道生成が可能であるか確認する．

    //! @todo 正常に動作しないため，一時凍結．後で修正する．

    return result;
}

bool GaitPatternGeneratorRevaluation::IsValidNode(
    [[maybe_unused]] const RobotStateNode& current_node,
    [[maybe_unused]] const RobotStateNode& next_node) const
{
    return true;
}

}  // namespace designlab
