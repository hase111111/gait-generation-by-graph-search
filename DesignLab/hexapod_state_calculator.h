//#pragma once
//
//#include "designlab_vector3.h"
//#include "designlab_euler.h"
//#include "node.h"
//#include "hexapod_const.h"
//
//
////! @class HexapodStateCalclator_Old
////! @date 2023/08/13
////! @author 長谷川
////! @brief ロボットの座標や状態などの値を計算するクラス．旧 Hexapodクラスを軽くしたもの．
//class HexapodStateCalclator_Old
//{
//public:
//	HexapodStateCalclator_Old();
//
//	static constexpr designlab::Vector3 getLocalBaseLegPos(const int _leg_num, const float _z)
//	{
//		return { 160 * HexapodConst::DEFAULT_LEG_ANGLE_COS[_leg_num],160 * HexapodConst::DEFAULT_LEG_ANGLE_SIN[_leg_num],_z };
//	}
//
//	//! @brief 第2引数の座標を現在の重心座標と回転から，脚の付け根を原点としたローカル座標に変換する．
//	//! @param [in] node ロボットの状態を表すノード
//	//! @param [in] global_pos 変換するグローバル座標
//	//! @param [in] leg_num 脚番号 0～5
//	//! @param [in] do_consider_rot 回転を考慮するかどうか
//	//! @return designlab::Vector3 ローカル座標の脚座標
//	designlab::Vector3 convertLocalLegPos(const SNode& node, const designlab::Vector3& global_pos, const int leg_num, const bool do_consider_rot) const;
//
//	//! @brief 脚座標は脚の付け根を原点とした座標系なので，それをグローバル座標に変換する．
//	//! @param [in] _node ロボットの状態を表すノード
//	//! @param [in] leg_num 脚番号 0～5
//	//! @param [in] _consider_rot 回転を考慮するかどうか
//	//! @return designlab::Vector3 グローバル座標の脚座標
//	inline designlab::Vector3 getGlobalLegPos(const SNode& _node, const int _leg_num, const bool _consider_rot) const
//	{
//		if (_consider_rot == true) { return rotVector(getLocalCoxaJointPos(_leg_num) + _node.leg_pos[_leg_num], _node.rot) + _node.global_center_of_mass; }
//		else { return _node.global_center_of_mass + getLocalCoxaJointPos(_leg_num) + _node.leg_pos[_leg_num]; }
//	}
//
//	//! @brief 脚位置は離散化されて制御されるが，その時の4の位置をグローバル座標で出力する．
//	//! @param [in] _node ロボットの状態を表すノード
//	//! @param [in] leg_num 脚番号 0～5
//	//! @param [in] _consider_rot 回転を考慮するかどうか
//	//! @return designlab::Vector3 グローバル座標の脚の基準地点の座標
//	inline designlab::Vector3 getGlobalLegBasePos(const SNode& _node, const int _leg_num, const bool _consider_rot) const
//	{
//		if (_consider_rot == true) { return rotVector(getLocalCoxaJointPos(_leg_num) + _node.leg_reference_pos[_leg_num], _node.rot) + _node.global_center_of_mass; }
//		else { return _node.global_center_of_mass + getLocalCoxaJointPos(_leg_num) + _node.leg_reference_pos[_leg_num]; }
//	}
//
//	// @brief coxa joint (脚の付け根 : 第1関節) をグローバル座標で返す．
//	// @param [in] _node ロボットの状態を表すノード
//	// @param [in] leg_num 脚番号 0～5
//	// @param [in] _consider_rot 回転を考慮するかどうか
//	// @return designlab::Vector3 グローバル座標の脚の付け根の座標
//	inline designlab::Vector3 getGlobalCoxaJointPos(const SNode& _node, const int _leg_num, const bool _consider_rot) const
//	{
//		if (_consider_rot == true) { return rotVector(getLocalCoxaJointPos(_leg_num), _node.rot) + _node.global_center_of_mass; }
//		else { return _node.global_center_of_mass + getLocalCoxaJointPos(_leg_num); }
//	}
//
//	//! @brief ノードの情報は現在の脚位置と重心位置しか持たないので，ジョイントがどこにあるかが分からない．よってこの関数で計算する．<br>
//	//! 三角関数を多く使用するので，計算量が多い．
//	//! @param [in] _node ロボットの状態を表すノード
//	//! @details この関数を使用すると，メンバ変数が更新される．
//	void calclateJointPos(const SNode& _node);
//
//	//【calclateJointPos関数を使用してから使うこと!!】femur joint (第2関節) の座標を返す．回転を考慮したグローバル座標.
//	designlab::Vector3 getGlobalFemurJointPos(const SNode& _node, const int _leg_num) const;
//
//	//【calclateJointPos関数を使用してから使うこと!!】tibia joint (第3関節) の座標を返す．回転を考慮したグローバル座標.
//	designlab::Vector3 getGlobalTibiaJointPos(const SNode& _node, const int _leg_num) const;
//
//	//静的メンバ変数の m_leg_max_r , m_leg_min_r の値を計算して初期化する．この関数自体が静的なのでSystemMainで一度だけ実行すればよい．
//	static void initLegR();
//
//	//【initLegR関数を使用してから使うこと!!】付け根から脚先までのZ座標の差を利用して，付け根から脚先までの最大半径を取得する．付け根の方が上にある前提で動きます．
//	float getMaxLegR(const float _coxa_z_to_leg_z) const;
//
//	//【initLegR関数を使用してから使うこと!!】付け根から脚先までのZ座標の差を利用して，付け根から脚先までの最小半径を取得する．付け根の方が上にある前提で動きます．
//	float getMinLegR(const float _coxa_z_to_leg_z) const;
//
//	//! @brief 脚の干渉をチェックする．
//	//! @param _node ノード情報
//	//! @return 干渉している場合はtrueを返す．
//	bool IsLegInterfering(const SNode& _node) const;
//
//	//! @brief 脚が可動範囲内かチェックする．速度重視のため，ざっくりとした計算を行う．
//	//! @param _node ノード情報
//	//! @param leg_num 脚番号
//	//! @return 可動範囲内ならtrueを返す．
//	bool IsLegInRange(const SNode& _node, const int _leg_num) const;
//
//	//! @brief 脚が可動範囲内かチェックする．速度重視のため，ざっくりとした計算を行う．
//	//! @param local_leg_pos 脚の付け根の座標を原点とした脚先の座標
//	//! @param leg_num 脚番号
//	//! @return 可動範囲内ならtrueを返す．
//	bool IsLegInRange(const designlab::Vector3& local_leg_pos, const int leg_num) const;
//
//	//! @brief 全ての接地脚が可動範囲内かチェックする．速度重視のため，ざっくりとした計算を行う．
//	//! @param _node ノード情報
//	//! @return 可動範囲内ならtrueを返す．
//	bool isAllLegInRange(const SNode& _node) const;
//
//	//! @brief 転ばない姿勢かどうか調べる．
//	//! @param [in] _node ノード情報
//	//! @return 転ばない姿勢ならtrueを返す．
//	bool isAblePause(const SNode& _node) const;
//
//	//! @brief 静的安定余裕を計算する．
//	//! @param [in] _node ノード情報
//	//! @return float 静的安定余裕
//	//! @note 波東さんのプログラムに書いてあったままを移しているが，isAblePause関数と同じようなことをしているので，isAblePause関数を書き直すべきだと思う．
//	float calculateStaticMargin(const SNode& _node) const;
//
//private:
//
//	//! @brief coxa joint (脚の付け根)の座標を返す．重心を原点とするローカル座標．
//	//! @param [in] leg_num 脚番号
//	//! @return coxa jointのローカル座標
//	designlab::Vector3 getLocalCoxaJointPos(const int leg_num) const
//	{
//		if (leg_num == 0) { return designlab::Vector3(HexapodConst::BODY_FRONT_LENGTH, -HexapodConst::BODY_FRONT_WIDTH, 0.0f); }	// 脚0 右上
//		else if (leg_num == 1) { return designlab::Vector3(0.0f, -HexapodConst::BODY_CENTER_WIDTH, 0.0f); }	// 脚1 右横
//		else if (leg_num == 2) { return designlab::Vector3(-HexapodConst::BODY_REAR_LENGTH, -HexapodConst::BODY_REAR_WIDTH, 0.0f); }	// 脚2 右下
//		else if (leg_num == 3) { return designlab::Vector3(-HexapodConst::BODY_REAR_LENGTH, HexapodConst::BODY_REAR_WIDTH, 0.0f); }	// 脚3 左下
//		else if (leg_num == 4) { return designlab::Vector3(0.0f, HexapodConst::BODY_CENTER_WIDTH, 0.0f); }	// 脚4 左横
//		else if (leg_num == 5) { return designlab::Vector3(HexapodConst::BODY_FRONT_LENGTH, HexapodConst::BODY_FRONT_WIDTH, 0.0f); }	// 脚5 左上
//
//		return designlab::Vector3(0, 0, 0);
//	}
//
//
//	constexpr static int MAX_DIF_Z = 200;	//付け根から脚先までのZ座標の差の最大値
//
//
//	designlab::Vector3 m_local_femurjoint_pos[HexapodConst::LEG_NUM];	//FemurJoint(第2関節)の位置．脚の付け根を原点とするローカル座標．calclateJointPos関数で値をセットする．
//	designlab::Vector3 m_local_tibiajoint_pos[HexapodConst::LEG_NUM];	//TibiaJoint(第3関節)の位置．脚の付け根を原点とするローカル座標．calclateJointPos関数で値をセットする．
//
//	static float m_leg_max_r[MAX_DIF_Z];	//重心高さから脚位置を下げた時の，脚の取りうる最大半径を記録したもの．旧名 Leg_ROM_R
//	static float m_leg_min_r[MAX_DIF_Z];	//重心高さから脚位置を下げた時の，脚の取りうる最小半径を記録したもの．旧名 Leg_ROM_R
//};
//
//
////! @file hexapod_state_calculator.h
////! @date 2023/08/13
////! @author 長谷川
////! @brief HexapodStateCalclatorクラスのヘッダファイル．
////! @n 行数 : @lineinfo
