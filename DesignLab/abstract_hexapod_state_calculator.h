#pragma once

#include <vector>

#include "designlab_vector.h"
#include "designlab_rotator.h"
#include "node.h"
#include "hexapod_const.h"



struct SHexapodJointState
{
	SHexapodJointState()
	{
		joint_position.resize(6);
		joint_angle.resize(6);
	}

	//! 関節の位置．付け根から初めて，脚先の順に並んでいる．2番脚の付け根の座標はjoint_position[2][0]である．@n この座標はグローバル座標系である．
	std::vector<std::vector<dl_vec::SVector>> joint_position;

	//! 関節の角度．付け根から初めて，脚先の順に並んでいる．2番脚の付け根の角度はjoint_angle[2][0]である．@n この角度はradである．
	std::vector<std::vector<float>> joint_angle;
};



//! @class AbstractHexapodStateCalculator
//! @date 2023/08/30
//! @author 長谷川
//! @brief ロボットの状態を計算するクラスの抽象クラス．
//! @n このクラスを継承して，具体的なロボット(例えばphantomXとかAUSRAとか)の状態を計算するクラスを作成する．
//! @n スレッドセーフなクラスにすること．https://yohhoy.hatenablog.jp/entry/2013/12/15/204116
class AbstractHexapodStateCalculator
{
public:
	AbstractHexapodStateCalculator() {};
	virtual ~AbstractHexapodStateCalculator() = default;


	//! @brief 全ての関節のグローバル座標と，角度を計算する．重たいのでグラフ探索や，描画処理中にループで使用することは推奨しない．
	//! @param [in] node ノードの情報．
	//! @param [out] joint_state 関節の状態．
	//! @return 計算に成功したらtrue．失敗したらfalse．
	virtual bool calculateAllJointState(const SNode& node, SHexapodJointState* const joint_state) const = 0;


	virtual dl_vec::SVector getLocalLegBasePosition(const int leg_index) const = 0;

	virtual dl_vec::SVector getLocalLegPosition(const SNode& node, const int leg_index) = 0;


	virtual dl_vec::SVector getGlobalLegBasePosition(const int leg_index) const = 0;

	virtual dl_vec::SVector getGlobalLegPosition(const SNode& node, const int leg_index) = 0;

private:

	static constexpr bool DO_CHECK_LEG_INDEX = false;	//!< 脚番号のチェックをするかどうか．速さのために，デバッグ時以外はfalseにすること．


	dl_vec::SVector m_local_leg_base_pos[HexapodConst::LEG_NUM];	//!< 脚の付け根の座標( leg base position)．ロボットの重心を原点，向いている方向をx軸としたローカル(ロボット)座標系である．
};

