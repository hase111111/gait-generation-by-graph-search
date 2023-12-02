//! @file gait_pattern_generator_switch_move.h
//! @brief 複数種類の歩容パターンの生成方法を切り替えるためのクラス．


#ifndef GAIT_PATTERN_GENERATOR_SWITCH_MOVE_H
#define GAIT_PATTERN_GENERATOR_SWITCH_MOVE_H

#include "interface_gait_pattern_generator.h"

#include <memory>
#include <vector>

#include "graph_tree_creator.h"
#include "interface_gait_pattern_generator.h"
#include "robot_state_node.h"


//! @class GaitPatternGeneratorSwitchMove
//! @brief 再評価手法を実装したクラス．
class GaitPatternGeneratorSwitchMove final : public IGaitPatternGenerator
{
public:

	//! @param[in] gait_pattern_generator_ptr 通常時に使用するもの．
	//! @param[in] gait_pattern_generator_revaluation_ptr 再評価時に使用するもの．
	GaitPatternGeneratorSwitchMove(
		std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_for_straigt,
		std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_for_turn_spot
	);

	~GaitPatternGeneratorSwitchMove() = default;


	GraphSearchResult GetNextNodebyGraphSearch(
		const RobotStateNode& current_node,
		const MapState& map,
		const TargetRobotState& target,
		RobotStateNode* output_node
	) override;

private:

	const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_for_straigt_ptr_;
	const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_for_turn_spot_ptr_;
};

#endif // GAIT_PATTERN_GENERATOR_SWITCH_MOVE_H