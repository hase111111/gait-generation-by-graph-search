//! @file interface_hexapod_joint_calculator.h
//! @brief 6脚ロボットの間接角度やジョイントの位置を計算する処理のインターフェース．
 
#ifndef	DESIGNLAB_INTERFACE_HEXAPOD_JOINT_CALCULATOR_H
#define	DESIGNLAB_INTERFACE_HEXAPOD_JOINT_CALCULATOR_H


#include <array>

#include "robot_state_node.h"


//! @struct HexapodJointState
//! @brief ロボットの関節の状態を表す構造体．
//! @n 関節の位置と角度を表す．
struct HexapodJointState
{
	//! 関節の位置．付け根から初めて，脚先の順に並んでいる．脚の付け根の座標はjoint_position[0]である．
	//! @n この座標は脚の付け根を原点とし，軸はロボット座標系と同様な脚座標系である．単位は [mm]．
	std::vector<designlab::Vector3> local_joint_position;

	//! 関節の位置．付け根から初めて，脚先の順に並んでいる．脚の付け根の座標はjoint_position[0]である．
	//! @n この座標はグローバル座標系である．単位は [mm]．
	std::vector<designlab::Vector3> global_joint_position;

	//! 関節の角度．付け根から初めて，脚先の順に並んでいる．脚の付け根の角度はjoint_angle[0]である．
	//! @n この角度の単位は [rad] である．
	std::vector<float> joint_angle;

	//! 目標座標に脚が届かないならばfalseになる
	bool is_in_range{false};
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
	//! @n 目標座標に届かない場合は，最も近い座標まで脚を伸ばし，戻り値のis_in_rangeをfalseにする．
	//! @param [in] node ノードの情報．
	//! @return 全ての関節の状態．
	virtual std::array<HexapodJointState, HexapodConst::kLegNum> CalculateAllJointState(const RobotStateNode& node) const = 0;

	//! @brief 指定した脚の関節のグローバル座標と，角度を計算する．
	//! @n 重たいのでグラフ探索や，描画処理中にループで使用することは推奨しない．
	//! @n 間接の可動範囲外まで動いてしまう場合でも，答えを返す．
	//! @n 目標座標に届かない場合は，最も近い座標まで脚を伸ばし，戻り値のis_in_rangeをfalseにする．
	//! @param [in] node ノードの情報．
	//! @param [in] leg_index 脚のインデックス．
	//! @return 指定した脚の関節の状態．
	virtual HexapodJointState CalculateJointState(const RobotStateNode& node, const int leg_index) const = 0;

	//! @brief HexapodJointStateが正しく計算できているかを調べる．
	//! @n 目標座標に届かない場合や，間接の可動範囲外まで動いてしまう場合，戻り値はfalseになる．
	//! @param [in] node ノードの情報．
	//! @param [in] joint_state 関節の状態．
	//! @return 計算が正しくできているならばtrueを返す
	virtual bool IsVaildAllJointState(const RobotStateNode& node, const std::array<HexapodJointState, HexapodConst::kLegNum>& joint_state) const = 0;

	//! @brief 指定した脚のHexapodJointStateが正しく計算できているかを調べる．
	//! @n 目標座標に届かない場合や，間接の可動範囲外まで動いてしまう場合，戻り値はfalseになる．
	//! @param [in] node ノードの情報．
	//! @param [in] joint_state 関節の状態．
	//! @param [in] leg_index 脚のインデックス．
	//! @return 計算が正しくできているならばtrueを返す
	virtual bool IsVaildJointState(const RobotStateNode& node, const HexapodJointState& joint_state, const int leg_index) const = 0;
};


#endif