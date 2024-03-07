
//! @file      interface_hexapod_posture_validator.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_INTERFACE_HEXAPOD_POSTURE_VALIDATOR_H_
#define DESIGNLAB_INTERFACE_HEXAPOD_POSTURE_VALIDATOR_H_

#include <array>

#include "divided_map_state.h"
#include "hexapod_const.h"
#include "leg_state.h"
#include "math_vector3.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class IHexapodPostureValidator
//! @brief 6脚ロボットが有効な姿勢をとっているかを
//! チェックする処理のインターフェース．
class IHexapodPostureValidator
{
public:
    virtual ~IHexapodPostureValidator() = default;


    //! @brief 脚が可動範囲内にあるかどうかを判定する．
    //! @param[in] leg_index 脚番号．
    //! @param[in] leg_pos 脚座標系における脚先の座標．
    //! 脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
    //! @return 脚が可動範囲内にあれば true．可動範囲外にあれば false．
    virtual bool IsLegInRange(const int leg_index, const Vector3& leg_pos) const = 0;

    //! @brief 全ての脚が可動範囲内にあるかどうかを判定する．
    //! @param[in] leg_state 脚の状態．bitで表現される，遊脚・接地脚の情報を持つ．
    //! @param[in] leg_pos 脚座標系における脚先の座標の配列．
    //! 脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
    //! @return 全ての脚が可動範囲内にあれば true．可動範囲外にあれば false．
    bool IsAllLegInRange(
        const leg_func::LegStateBit leg_state,
        const std::array<Vector3, HexapodConst::kLegNum>& leg_pos) const
    {
        for (int i = 0; i < HexapodConst::kLegNum; i++)
        {
            if (leg_func::IsGrounded(leg_state, i) && !IsLegInRange(i, leg_pos[i]))
            {
                return false;
            }
        }

        return true;
    }

    //! @brief 脚が他の脚と干渉しているかどうかを判定する．
    //! @param[in] leg_pos 脚座標系における脚先の座標の配列．
    //! 脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
    //! @return 脚が他の脚と干渉していれば true．干渉していなければ false．
    virtual bool IsLegInterfering(
        const std::array<Vector3, HexapodConst::kLegNum>& leg_pos) const = 0;

    //! @brief 安定余裕(Stability Margin)を計算する．
    //! 詳しくは「不整地における歩行機械の静的安定性評価基準」
    //! という論文を読んで欲しい．
    //! @n 接地脚を繋いで作られる多角形の辺と重心の距離の最小値を計算する．
    //! @param[in] leg_state 脚の状態．bitで表現される，遊脚・接地脚の情報を持つ．
    //! @param[in] leg_pos 脚座標系における脚先の座標の配列．
    //! 脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
    //! @return 安定余裕．大きい方が安定となる，またこの値が0以下なら転倒する．
    virtual float CalculateStabilityMargin(const leg_func::LegStateBit& leg_state,
      const std::array<Vector3, HexapodConst::kLegNum>& leg_pos) const = 0;

    //! @brief 安定余裕を用いて，静的に安定しているかどうかを判定する．
    //! @param[in] leg_state 脚の状態．bitで表現される，遊脚・接地脚の情報を持つ．
    //! @param[in] leg_pos 脚座標系における脚先の座標の配列．
    //! 脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
    //! @return 静的に安定していれば true．そうでなければ false．
    virtual bool IsStable(const leg_func::LegStateBit& leg_state,
      const std::array<Vector3, HexapodConst::kLegNum>& leg_pos) const = 0;

    //! @brief 胴体が地面と干渉しているかどうかを判定する．
    //! @param[in] node ロボットの状態．
    //! @param[in] divided_map 地面の状態．
    //! @return 胴体が地面と干渉していれば true．干渉していなければ false．
    virtual bool IsBodyInterferingWithGround(
        const RobotStateNode& node, const DividedMapState& divided_map) const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_HEXAPOD_POSTURE_VALIDATOR_H_
