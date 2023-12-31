
//! @file      gait_pattern_generator_switch_move.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_GAIT_PATTERN_GENERATOR_SWITCH_MOVE_H_
#define DESIGNLAB_GAIT_PATTERN_GENERATOR_SWITCH_MOVE_H_

#include <memory>
#include <vector>

#include "graph_tree_creator.h"
#include "interface_gait_pattern_generator.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class GaitPatternGeneratorSwitchMove
//! @brief 複数の種類の歩容パターンの生成方法を切り替えるためのクラス．
class GaitPatternGeneratorSwitchMove final : public IGaitPatternGenerator
{
public:
    //! @param[in] gait_pattern_generator_for_straight 直進時に使用するもの．
    //! @param[in] gait_pattern_generator_for_turn_spot その場旋回時に使用するもの．
    GaitPatternGeneratorSwitchMove(
      std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_for_straight,
      std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_for_turn_spot);

    ~GaitPatternGeneratorSwitchMove() = default;


    GraphSearchResult GetNextNodeByGraphSearch(
      const RobotStateNode& current_node,
      const MapState& map,
      const RobotOperation& operation,
      RobotStateNode* output_node) override;

private:
    const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_for_straight_ptr_;
    const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_for_turn_spot_ptr_;
};

}  // namespace designlab


#endif  // DESIGNLAB_GAIT_PATTERN_GENERATOR_SWITCH_MOVE_H_
