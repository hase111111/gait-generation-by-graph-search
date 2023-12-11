//! @file gait_pattern_generator_revaluation.h
//! @brief 再評価手法を実装したクラス．


#ifndef GAIT_PATTERN_GENERATOR_REVALUATION_H_
#define GAIT_PATTERN_GENERATOR_REVALUATION_H_


#include <memory>
#include <vector>

#include "graph_tree_creator.h"
#include "interface_gait_pattern_generator.h"
#include "robot_state_node.h"


//! @class GaitPatternGeneratorRevaluation
//! @brief 再評価手法を実装したクラス．
class GaitPatternGeneratorRevaluation final : public IGaitPatternGenerator
{
public:

	//! @param[in] gait_pattern_generator_ptr 通常時に使用するもの．
	//! @param[in] gait_pattern_generator_revaluation_ptr 再評価時に使用するもの．
	GaitPatternGeneratorRevaluation(
		std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
		std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_revaluation_ptr
	);

	~GaitPatternGeneratorRevaluation() = default;


	GraphSearchResult GetNextNodebyGraphSearch(
		const RobotStateNode& current_node,
		const MapState& map,
		const RobotOperation& operation,
		RobotStateNode* output_node
	) override;

private:

	bool IsVaildNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

	const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_ptr_;
	const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_revaluation_ptr_;
};


#endif	//	GAIT_PATTERN_GENERATOR_REVALUATION_H_