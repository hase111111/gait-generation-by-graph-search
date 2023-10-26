//! @file abstract_hexapod_state_calculator.h
//! @brief ロボットの状態を計算するクラスの抽象クラス．
//! @details このクラスを継承して，具体的なロボット(例えばphantomXとかAUSRAとか)の状態を計算するクラスを作成する．
//! @n ロボットの姿勢を計算する都合上，座標系が入り交じることに注意．
//! @n 3つの座標系が存在している，
//! @n
//! @n [1] グローバル座標系(ワールド座標系)
//! @n		マップの原点を原点とする座標系，座標軸はマップの座標軸と同じ．
//! @n
//! @n [2] ローカル座標系(ロボット座標系)
//! @n		ロボットの重心を原点とする座標系，座標軸はロボットの姿勢に合わせる．
//! @n 
//! @n [3] 脚座標系 
//! @n		脚の付け根を原点とする座標系，座標軸はロボットの姿勢に合わせる(ローカル座標系と同じ)．
//! @n		運動学の教科書とか読む感じ，この座標系の取り方とは違うのだが，先行研究で使われた手法であり，
//! @n		実用上問題ないのでこのまま．
//! @n
//! @n 変換は，脚座標系→グローバル座標系ならば，
//! @n	( 脚座標系の座標 ＋ ロボットの原点から脚の付け根までの座標(ローカル) ) * 姿勢の逆回転 ＋ ロボットの重心の座標(グローバル)


#ifndef DESIGNLAB_ABSTRACT_HEXAPOD_STATE_CALCULATOR_H_
#define DESIGNLAB_ABSTRACT_HEXAPOD_STATE_CALCULATOR_H_

#include <array>
#include <optional>
#include <vector>

#include "designlab_vector3.h"
#include "designlab_euler.h"
#include "leg_state.h"
#include "robot_state_node.h"
#include "hexapod_const.h"


//! @struct HexapodJointState
//! @brief ロボットの関節の状態を表す構造体．
//! @details 関節の位置と角度を表す．
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
	bool is_in_range = false;
};


//! @class AbstractHexapodStateCalculator
//! @brief ロボットの状態を計算するクラスの抽象クラス．
//! @details このクラスを継承して，具体的なロボット(例えばphantomXとかAUSRAとか)の状態を計算するクラスを作成する．
//! @n このクラスを用いてロボットのステートを表現しているので，HexapodPresenterとでもしたほうが良いかもしれない
//! @n スレッドセーフなクラスにすること．https://yohhoy.hatenablog.jp/entry/2013/12/15/204116
class AbstractHexapodStateCalculator
{
public:
	AbstractHexapodStateCalculator() = default;
	virtual ~AbstractHexapodStateCalculator() = default;


	//! @brief 全ての関節のグローバル座標と，角度を計算する．重たいのでグラフ探索や，描画処理中にループで使用することは推奨しない．
	//! @n 目標座標に届かない場合や，間接の可動範囲外まで動いてしまう場合でも，答えを返す．
	//! @n 脚が届かない場合は，戻り値のis_in_rangeがfalseになる．
	//! @param [in] node ノードの情報．
	//! @param [out] joint_state 関節の状態．
	virtual void CalculateAllJointState(const RobotStateNode& node, std::array<HexapodJointState, HexapodConst::kLegNum>* joint_state) const = 0;

	virtual bool IsVaildJointState(const RobotStateNode& node, const std::array<HexapodJointState, HexapodConst::kLegNum>& joint_state) const = 0;

	//! @brief グローバル座標系→脚座標系に変換する．
	//! @param [in] leg_index 脚番号．
	//! @param [in] global_pos グローバル座標系の脚先座標．
	//! @param [in] global_center_of_mass ロボットの重心の座標．グローバル座標系．
	//! @param [in] robot_rot ロボットの姿勢．角度はrad.
	//! @param [in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
	//! @return designlab::Vector3 脚座標系の脚先座標．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	virtual designlab::Vector3 ConvertGlobalToLegPosition(int leg_index, const designlab::Vector3& global_pos, 
		const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, bool consider_rot) const = 0;


	//! @brief 遊脚する位置を返す，脚座標系
	//! @param [in] leg_index 脚番号．
	//! @return designlab::Vector3 遊脚する位置．脚座標系
	virtual designlab::Vector3 GetFreeLegPosition(int leg_index) const = 0;


	//! @brief 脚の付け根の座標( leg base position)を取得する．ローカル(ロボット)座標系
	//! @param [in] leg_index 脚番号．
	//! @return designlab::Vector3 脚の付け根の座標．ローカル(ロボット)座標系
	virtual designlab::Vector3 GetLocalLegBasePosition(int leg_index) const = 0;

	//! @brief 脚先の座標を取得する．ローカル(ロボット)座標系
	//! @param [in] leg_index 脚番号．
	//! @param [in] leg_pos 脚座標系における脚先の座標．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @return designlab::Vector3 脚先の座標．ローカル座標系
	virtual designlab::Vector3 GetLocalLegPosition(int leg_index, const designlab::Vector3& leg_pos) const = 0;


	//! @brief 脚の付け根の座標( leg base position)を取得する．グローバル(ワールド)座標系
	//! @param [in] leg_index 脚番号．
	//! @param [in] global_center_of_mass ロボットの重心の座標．グローバル座標系．
	//! @param [in] robot_rot ロボットの姿勢．角度はrad.
	//! @param [in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
	//! @return designlab::Vector3 脚の付け根の座標．グローバル座標系．
	virtual designlab::Vector3 GetGlobalLegBasePosition(int leg_index, const designlab::Vector3& global_center_of_mass, 
		const designlab::EulerXYZ& robot_rot, bool consider_rot) const = 0;

	//! @brief 脚の先端の座標を取得する．グローバル(ワールド)座標系
	//! @param [in] leg_index 脚番号．
	//! @param [in] leg_pos 脚座標系における脚先の座標．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	//! @param [in] global_center_of_mass ロボットの重心の座標．グローバル座標系．
	//! @param [in] robot_rot ロボットの姿勢．角度はrad.
	//! @param [in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
	//! @return designlab::Vector3 脚先の座標．グローバル座標系．
	virtual designlab::Vector3 GetGlobalLegPosition(int leg_index, const designlab::Vector3& leg_pos, 
		const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, bool consider_rot) const = 0;


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
	float CalculateStabilityMargin(const ::designlab::leg_func::LegStateBit& leg_state, const std::array<designlab::Vector3, HexapodConst::kLegNum>& leg_pos) const;
};


#endif // !DESIGNLAB_ABSTRACT_HEXAPOD_STATE_CALCULATOR_H_