#pragma once

#include <vector>

#include "designlab_vector.h"
#include "designlab_rotator.h"
#include "node.h"
#include "hexapod_const.h"



struct SHexapodJointState
{
	//! 関節の位置．付け根から初めて，脚先の順に並んでいる．脚の付け根の座標はjoint_position[0]である．@n この座標は脚の付け根を原点とし，軸はロボット座標系と同様な脚座標系．
	std::vector<dl_vec::SVector> local_joint_position;

	//! 関節の位置．付け根から初めて，脚先の順に並んでいる．脚の付け根の座標はjoint_position[0]である．@n この座標はグローバル座標系である．
	std::vector<dl_vec::SVector> global_joint_position;

	//! 関節の角度．付け根から初めて，脚先の順に並んでいる．脚の付け根の角度はjoint_angle[0]である．@n この角度の単位はradである．
	std::vector<float> joint_angle;
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
	AbstractHexapodStateCalculator() = default;
	virtual ~AbstractHexapodStateCalculator() = default;



	//! @brief 全ての関節のグローバル座標と，角度を計算する．重たいのでグラフ探索や，描画処理中にループで使用することは推奨しない．
	//! @param [in] node ノードの情報．
	//! @param [out] joint_state 関節の状態．
	//! @return 計算に成功したらtrue．失敗したらfalse．
	virtual bool calculateAllJointState(const SNode& node, SHexapodJointState joint_state[HexapodConst::LEG_NUM]) const = 0;



	//! @brief 【スレッドセーフ】脚の付け根の座標( leg base position)を取得する．ローカル(ロボット)座標系
	//! @param [in] leg_index 脚番号．
	//! @return dl_vec::SVector 脚の付け根の座標．ローカル座標系
	dl_vec::SVector getLocalLegBasePosition(const int leg_index) const;

	//! @brief 【スレッドセーフ】脚先の座標を取得する．ローカル(ロボット)座標系
	//! @param [in] leg_index 脚番号．
	//! @param [in] leg_pos 脚座標系における脚先の座標．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @return dl_vec::SVector 脚先の座標．ローカル座標系
	virtual dl_vec::SVector getLocalLegPosition(const int leg_index, const dl_vec::SVector& leg_pos) const = 0;



	//! @brief 【スレッドセーフ】脚の付け根の座標( leg base position)を取得する．グローバル(ワールド)座標系
	//! @param [in] leg_index 脚番号．
	//! @param [in] global_center_of_mass ロボットの重心の座標．グローバル座標系．
	//! @param [in] robot_rot ロボットの姿勢．角度はrad.
	//! @param [in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
	//! @return dl_vec::SVector 脚の付け根の座標．グローバル座標系．
	virtual dl_vec::SVector getGlobalLegBasePosition(const int leg_index, const dl_vec::SVector& global_center_of_mass, const dl_vec::SRotator& robot_rot, const bool consider_rot) const = 0;

	//! @brief 【スレッドセーフ】脚の先端の座標を取得する．グローバル(ワールド)座標系
	//! @param [in] leg_index 脚番号．
	//! @param [in] leg_pos 脚座標系における脚先の座標．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @param [in] global_center_of_mass ロボットの重心の座標．グローバル座標系．
	//! @param [in] robot_rot ロボットの姿勢．角度はrad.
	//! @param [in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
	//! @return dl_vec::SVector 脚先の座標．グローバル座標系．
	virtual dl_vec::SVector getGlobalLegPosition(const int leg_index, const dl_vec::SVector& leg_pos, const dl_vec::SVector& global_center_of_mass, const dl_vec::SRotator& robot_rot, const bool consider_rot) const = 0;



	//! @brief 【スレッドセーフ】脚が可動範囲内にあるかどうかを判定する．
	//! @param [in] leg_index 脚番号．
	//! @param [in] leg_pos 脚座標系における脚先の座標．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @return bool 脚が可動範囲内にあればtrue．可動範囲外にあればfalse．
	virtual bool isLegInRange(const int leg_index, const dl_vec::SVector& leg_pos) const = 0;

	//! @brief 【スレッドセーフ】脚が他の脚と干渉しているかどうかを判定する．
	//! @param [in] leg_pos 脚座標系における脚先の座標の配列．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @return bool 脚が他の脚と干渉していればtrue．干渉していなければfalse．
	virtual bool isLegInterfering(const dl_vec::SVector leg_pos[HexapodConst::LEG_NUM]) const = 0;

	//! @brief 【スレッドセーフ】安定余裕(Stability Margin))を計算する．詳しくは「不整地における歩行機械の静的安定性評価基準」という論文を読んで欲しい
	//! @n 接地脚を繋いで作られる多角形の辺と重心の距離の最小値を計算する．
	//! @param [in] leg_state 脚の状態．bitで表現される，遊脚・接地脚の情報を持つ．
	//! @param [in] leg_pos 脚座標系における脚先の座標の配列．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @return float 安定余裕．大きい方が安定となる，またこの値が0以下なら転倒する．
	float calcStabilityMargin(const int leg_state, const dl_vec::SVector leg_pos[HexapodConst::LEG_NUM]) const;

protected:

	//! @brief 脚番号のチェックを行う．constexprなので，コンパイル時にチェックされる．
	//! @param [in] leg_index 脚番号．0以上，6未満であること．
	//! @return 脚番号が正しければtrue．正しくなければfalse．
	constexpr bool checkLegIndex(const int leg_index) const
	{
		if (leg_index < 0 || leg_index >= HexapodConst::LEG_NUM)
		{
			return false;
		}
		return true;
	}


	static constexpr bool DO_CHECK_LEG_INDEX = false;				//!< 脚番号のチェックをするかどうか．速さのために，デバッグ時以外はfalseにすること．


	dl_vec::SVector m_local_leg_base_pos[HexapodConst::LEG_NUM];	//!< 脚の付け根の座標( leg base position)．ロボットの重心を原点，向いている方向をx軸としたローカル(ロボット)座標系である．
};

