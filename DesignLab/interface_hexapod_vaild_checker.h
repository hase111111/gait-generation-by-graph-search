//! @file interface_hexapod_vaild_checker.h
//! @brief 6脚ロボットが有効な姿勢をとっているかをチェックするクラス

#ifndef DESIGNLAB_INTERFACE_HEXAPOD_VAILD_CHECKER_H_
#define DESIGNLAB_INTERFACE_HEXAPOD_VAILD_CHECKER_H_

#include <array>

#include "designlab_vector3.h"
#include "devide_map_state.h"
#include "hexapod_const.h"
#include "leg_state.h"
#include "robot_state_node.h"


//! @class IHexapodVaildChecker
//! @brief 6脚ロボットが有効な姿勢をとっているかをチェックするクラス
class IHexapodVaildChecker
{
public:
	virtual ~IHexapodVaildChecker() = default;


	//! @brief 脚が可動範囲内にあるかどうかを判定する．
	//! @param [in] leg_index 脚番号．
	//! @param [in] leg_pos 脚座標系における脚先の座標．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @return bool 脚が可動範囲内にあればtrue．可動範囲外にあればfalse．
	virtual bool IsLegInRange(const int leg_index, const designlab::Vector3& leg_pos) const = 0;

	//! @brief 脚が他の脚と干渉しているかどうかを判定する．
	//! @param [in] leg_pos 脚座標系における脚先の座標の配列．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @return bool 脚が他の脚と干渉していればtrue．干渉していなければfalse．
	virtual bool IsLegInterfering(const std::array<designlab::Vector3, HexapodConst::kLegNum>& leg_pos) const = 0;

	//! @brief 安定余裕(Stability Margin))を計算する．詳しくは「不整地における歩行機械の静的安定性評価基準」という論文を読んで欲しい
	//! @n 接地脚を繋いで作られる多角形の辺と重心の距離の最小値を計算する．
	//! @param [in] leg_state 脚の状態．bitで表現される，遊脚・接地脚の情報を持つ．
	//! @param [in] leg_pos 脚座標系における脚先の座標の配列．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @return float 安定余裕．大きい方が安定となる，またこの値が0以下なら転倒する．
	virtual float CalculateStabilityMargin(const ::designlab::leg_func::LegStateBit& leg_state,
		const std::array<designlab::Vector3, HexapodConst::kLegNum>& leg_pos) const = 0;

	//! @brief 安定余裕を用いて，静的に安定しているかどうかを判定する．
	//! @param [in] leg_state 脚の状態．bitで表現される，遊脚・接地脚の情報を持つ．
	//! @param [in] leg_pos 脚座標系における脚先の座標の配列．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @return bool 静的に安定していればtrue．そうでなければfalse．
	virtual bool IsStable(const ::designlab::leg_func::LegStateBit& leg_state,
		const std::array<designlab::Vector3, HexapodConst::kLegNum>& leg_pos) const = 0;

	//! @brief 胴体が地面と干渉しているかどうかを判定する．
	//! @param [in] node ロボットの状態．
	//! @param [in] devide_map 地面の状態．
	//! @return bool 胴体が地面と干渉していればtrue．干渉していなければfalse．
	virtual bool IsBodyInterferingWithGround(const RobotStateNode& node, const DevideMapState& devide_map) const = 0;
};


#endif	// DESIGNLAB_INTERFACE_HEXAPOD_VAILD_CHECKER_H_