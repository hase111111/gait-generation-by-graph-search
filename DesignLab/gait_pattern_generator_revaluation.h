
//! @file      gait_pattern_generator_revaluation.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_GAIT_PATTERN_GENERATOR_REVALUATION_H_
#define DESIGNLAB_GAIT_PATTERN_GENERATOR_REVALUATION_H_

#include <memory>
#include <vector>

#include "graph_tree_creator.h"
#include "interface_gait_pattern_generator.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interpolated_node_creator.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class GaitPatternGeneratorRevaluation
//! @brief 再評価手法を実装したクラス．
class GaitPatternGeneratorRevaluation final : public IGaitPatternGenerator
{
public:
    //! @param[in] gait_pattern_generator_ptr 通常時に使用するもの．
    //! @param[in] gait_pattern_generator_revaluation_ptr 再評価時に使用するもの．
    GaitPatternGeneratorRevaluation(
      std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
      std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_revaluation_ptr,
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& joint_calculator_ptr);

    ~GaitPatternGeneratorRevaluation() = default;


    GraphSearchResult GetNextNodeByGraphSearch(
      const RobotStateNode& current_node,
      const MapState& map,
      const RobotOperation& operation,
      RobotStateNode* output_node) override;

private:
    bool IsValidNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

    const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_ptr_;
    const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_revaluation_ptr_;

    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
    const std::shared_ptr<const IHexapodJointCalculator> joint_calculator_ptr_;

    InterpolatedNodeCreator interpolated_node_creator_;
};

}  // namespace designlab


#endif  // DESIGNLAB_GAIT_PATTERN_GENERATOR_REVALUATION_H_
