
//! @file      interpolated_node_creator.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_INTERPOLATED_NODE_CREATOR_H_
#define DESIGNLAB_INTERPOLATED_NODE_CREATOR_H_

#include <array>
#include <memory>
#include <vector>

#include "interface_hexapod_coordinate_converter.h"
#include "math_util.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class InterpolatedNodeCreator
//! @brief 矩形軌道を生成し，ノード間を補間するクラス．
class InterpolatedNodeCreator final
{
public:
    explicit InterpolatedNodeCreator(const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr);

    //! @brief ノード間を補間する．
    //! @param[in] node 現在のノード．
    //! @param[in] next_node 次のノード．
    //! @return 補間されたノード．
    std::vector<RobotStateNode> CreateInterpolatedNode(const RobotStateNode& node, const RobotStateNode& next_node) const;

private:
    static constexpr float kInterpolatedDistance = 0.15f;  //!< 補間する距離 [mm]．
    static constexpr float kInterpolatedAngle = math_util::ConvertDegToRad(0.1f);  //!< 補間する角度 [rad]．

    static constexpr float kBodyMoveMaxInterpolatedDistance = 200.0f;  //!< これ以上の距離を移動する場合は，補間を行わない．

    //! @brief 補間が必要ないかどうかを判定する．
    //! 重心座標の変化と，脚の接地点の変化，ロボットの姿勢の変化を調べて，
    //! 変化がないならば，補間が必要ないと判定する．
    //! @return 補間が必要ないならば trueを返す．
    bool IsNoChange(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

    //! @return 胴体が移動するなら trueを返す．
    bool IsBodyMove(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

    //! @return 胴体が回転するなら trueを返す．
    bool IsBodyRot(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

    std::vector<RobotStateNode> CreateBodyMoveInterpolatedNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

    std::vector<RobotStateNode> CreateBodyRotInterpolatedNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

    //! @brief 接地動作をする脚の indexを取得する．
    std::vector<int> GetGroundMoveIndex(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

    //! @brief 遊脚動作をする脚の indexを取得する．
    std::vector<int> GetFreeMoveIndex(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

    std::vector<RobotStateNode> CreateLegMoveInterpolatedNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const;


    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERPOLATED_NODE_CREATOR_H_
