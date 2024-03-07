
//! @file      interface_hexapod_joint_calculator.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_INTERFACE_HEXAPOD_JOINT_CALCULATOR_H_
#define DESIGNLAB_INTERFACE_HEXAPOD_JOINT_CALCULATOR_H_

#include <array>
#include <vector>

#include "robot_state_node.h"


namespace designlab
{

//! @struct HexapodJointState
//! @brief ロボットの関節の状態を表す構造体．
//! @details 関節の位置と角度を表す．
struct HexapodJointState
{
    //! 関節の位置．付け根から初めて，脚先の順に並んでいる．
    //! 脚の付け根の座標は joint_position[0] である．
    //! @n この座標は脚の付け根を原点とし，
    //! 軸はロボット座標系と同様な脚座標系 ( leg coordinate ) である．単位は [mm]．
    std::vector<Vector3> joint_pos_leg_coordinate;

    //! 関節の角度．付け根から初めて，脚先の順に並んでいる．
    //! 脚の付け根の角度は joint_angle[0]である．
    //! @n この角度の単位は [rad] である．
    std::vector<float> joint_angle;

    //! 目標座標に脚が届かないならば false になる．
    bool is_in_range{ false };
};


//! class IHexapodJointCalculator
//! @brief 間接角度や角速度を計算する処理のインターフェース．
class IHexapodJointCalculator
{
public:
    virtual ~IHexapodJointCalculator() = default;


    //! @brief 全ての関節のグローバル座標と，角度を計算する．
    //! @n 重たいのでグラフ探索や，描画処理中にループで使用することは推奨しない．
    //! @n 間接の可動範囲外まで動いてしまう場合でも，答えを返す．
    //! @n 目標座標に届かない場合は，最も近い座標まで脚を伸ばし，
    //! 戻り値の is_in_range を false にする．
    //! @param[in] node ノードの情報．
    //! @return 全ての関節の状態．
    [[nodiscard]]
    std::array<HexapodJointState, HexapodConst::kLegNum> CalculateAllJointState(
        const RobotStateNode& node) const noexcept
    {
        std::array<HexapodJointState, HexapodConst::kLegNum> joint_state;

        // 計算を行う．
        for (int i = 0; i < HexapodConst::kLegNum; i++)
        {
            joint_state[i] = CalculateJointState(i, node.leg_pos[i]);
        }

        return joint_state;
    }

    //! @brief 指定した脚の関節のグローバル座標と，角度を計算する．
    //! @n 重たいのでグラフ探索や，描画処理中にループで使用することは推奨しない．
    //! @n 間接の可動範囲外まで動いてしまう場合でも，答えを返す．
    //! @n 目標座標に届かない場合は，最も近い座標まで脚を伸ばし，
    //! 戻り値の is_in_range を false にする．
    //! @param[in] leg_index 脚のインデックス．
    //! @param[in] leg_pos 脚先座標，脚座標系．
    //! @return 指定した脚の関節の状態．
    [[nodiscard]]
    virtual HexapodJointState CalculateJointState(
        const int leg_index, const Vector3& leg_pos) const noexcept = 0;

    //! @brief HexapodJointStateが正しく計算できているかを調べる．
    //! @n 目標座標に届かない場合や，間接の可動範囲外まで動いてしまう場合，
    //! 戻り値は false になる．
    //! @param[in] node ノードの情報．
    //! @param[in] joint_state 関節の状態．
    //! @return 計算が正しくできているならば true を返す．
    [[nodiscard]]
    bool IsValidAllJointState(
        const RobotStateNode& node,
        const std::array<HexapodJointState, HexapodConst::kLegNum>& joint_state)
        const noexcept
    {
        for (int i = 0; i < HexapodConst::kLegNum; i++)
        {
            if (!IsValidJointState(i, node.leg_pos[i], joint_state[i]))
            {
                return false;
            }
        }

        return true;
    }

    //! @brief 指定した脚のHexapodJointStateが正しく計算できているかを調べる．
    //! @n 目標座標に届かない場合や，間接の可動範囲外まで動いてしまう場合，
    //! 戻り値は false になる．
    //! @param[in] leg_index 脚のインデックス．
    //! @param[in] leg_pos 脚先座標，脚座標系．
    //! @param[in] joint_state 関節の状態．
    //! @return 計算が正しくできているならば true を返す．
    [[nodiscard]]
    virtual bool IsValidJointState(
        const int leg_index, const Vector3& leg_pos,
        const HexapodJointState& joint_state) const noexcept = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_HEXAPOD_JOINT_CALCULATOR_H_
