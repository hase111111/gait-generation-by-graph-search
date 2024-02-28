
//! @file      interpolate_validator.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_INTERPOLATE_VALIDATOR_H_
#define DESIGNLAB_INTERPOLATE_VALIDATOR_H_

#include <memory>

#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interpolated_node_creator.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class InterpolateValidator
//! @brief 補間の妥当性を検証するクラス．
class InterpolateValidator final
{
public:
    //! @brief コンストラクタ．
    //! @param converter 座標変換クラス．
    InterpolateValidator(const std::shared_ptr<const IHexapodCoordinateConverter>& converter,
                         const std::shared_ptr<const IHexapodJointCalculator>& calculator);

    //! @brief 2つのノード間を矩形軌道で補完し，可動範囲外を通過しないか検証する．
    //! @param current_node 現在のノード．
    //! @param next_node 次のノード．
    //! @retval true 可動範囲外を通過しない．
    //! @retval false 可動範囲外を通過する．
    bool IsValid(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

private:
    const std::shared_ptr<const IHexapodCoordinateConverter> converter_;
    const std::shared_ptr<const IHexapodJointCalculator> calculator_;

    InterpolatedNodeCreator interpolated_node_creator_;
};

}  // namespace designlab

#endif  // DESIGNLAB_INTERPOLATE_VALIDATOR_H_
