//! @file phantomx_renderer_simple.h
//! @brief ロボットの描画を行うクラス．

#ifndef DESIGNLAB_PHANTOMX_RENDERER_SIMPLE_H_
#define DESIGNLAB_PHANTOMX_RENDERER_SIMPLE_H_


#include <array>
#include <memory>

#include <Dxlib.h>

#include "abstract_hexapod_state_calculator.h"
#include "define.h"
#include "display_quality.h"
#include "hexapod_const.h"
#include "interface_hexapod_renderer.h"
#include "robot_state_node.h"


#ifndef DESIGNLAB_DONOT_USE_DXLIB


//! @class PhantomXRendererSimple
//! @brief PhantomXの描画を行うクラス．3Dモデルを使用せず，多角形を組み合わせてロボットを描画する．
class PhantomXRendererSimple final : public IHexapodRenderer
{
public:

	PhantomXRendererSimple(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr, DisplayQuality display_quality);

	void SetDrawNode(const RobotStateNode& node) override;

	void Draw() const override;

private:

	//! @struct PhantomXDrawData
	//! @brief 描画に必要なデータを毎回計算するのは無駄なので，あらかじめ計算して，その値を保持しておくための構造体．
	struct PhantomXDrawData
	{
		VECTOR coxa_joint_pos;		//!< 逆運動学で求めた第１関節の位置
		VECTOR femur_joint_pos;		//!< 逆運動学で求めた第２関節の位置
		VECTOR tibia_joint_pos;		//!< 逆運動学で求めた第３関節の位置
		VECTOR leg_end_pos;			//!< 逆運動学で求めた脚先の位置
		VECTOR leg_reference_pos;	//!< 脚が接地する際に基準とする位置

		float coxa_cos;		//!< 第１関節のコサイン
		float coxa_sin;		//!< 第１関節のサイン
		float femur_cos;	//!< 第２関節のコサイン
		float femur_sin;	//!< 第２関節のサイン
		float tibia_cos;	//!< 第３関節のコサイン
		float tibia_sin;	//!< 第３関節のサイン

		designlab::Vector3 kine_coxa_joint_vec;		//!< 間接角度から順運動学で求めた第１関節の位置(designlab::Vector3)．脚座標系
		designlab::Vector3 kine_femur_joint_vec;	//!< 間接角度から順運動学で求めた第２関節の位置(designlab::Vector3)．脚座標系
		designlab::Vector3 kine_tibia_joint_vec;	//!< 間接角度から順運動学で求めた第３関節の位置(designlab::Vector3)．脚座標系
		designlab::Vector3 kine_leg_end_vec;		//!< 間接角度から順運動学で求めた脚先の位置(designlab::Vector3)．脚座標系

		VECTOR kine_coxa_joint_pos;		//!< 間接角度から順運動学で求めた第１関節の位置(VECTOR)
		VECTOR kine_femur_joint_pos;	//!< 間接角度から順運動学で求めた第２関節の位置(VECTOR)
		VECTOR kine_tibia_joint_pos;	//!< 間接角度から順運動学で求めた第３関節の位置(VECTOR)
		VECTOR kine_leg_end_pos;		//!< 間接角度から順運動学で求めた脚先の位置(VECTOR)

		float coxa_link_length;		//!< 第１リンクの長さ
		float femur_link_length;	//!< 第２リンクの長さ
		float tibia_link_length;	//!< 第３リンクの長さ

		bool is_able_coxa_angle;	//!< 第１関節の角度が有効かどうか
		bool is_able_femur_angle;	//!< 第２関節の角度が有効かどうか
		bool is_able_tibia_angle;	//!< 第３関節の角度が有効かどうか
	};


	//! @brief 通常通りにロボットの描画をする
	void DrawHexapodNormal() const;

	//! @brief 低品質にロボットの描画をする
	void DrawHexapodLow() const;

	bool IsAbleCoxaLeg(const designlab::Vector3& coxa_joint, const designlab::Vector3& femur_joint) const;
	bool IsAbleFemurLeg(const designlab::Vector3& femur_joint, const designlab::Vector3& tibia_joint) const;
	bool IsAbleTibiaLeg(const designlab::Vector3& tibia_joint, const designlab::Vector3& leg_joint) const;


	const unsigned int kColorBody;			//!< 胴体の色
	const unsigned int kColorLeg;			//!< 脚の色
	const unsigned int kColorLiftedLeg;		//!< 遊脚している脚の色
	const unsigned int kColorJoint;			//!< ジョイントの色
	const unsigned int kColorLiftedJoint;	//!< 遊脚しているジョイントの色
	const unsigned int kColorLegBase;		//!< 脚の基部の色
	const unsigned int kColorKineLeg;
	const unsigned int kColorKineJoint;
	const unsigned int kColorErrorJoint;	//!< 文字の色
	const unsigned int kColorErrorText;		//!< エラーの文字色

	const int kCapsuleDivNum;	//!< ロボットのモデルの円柱をどれだけ細かく描画するか．4 ～ 20ぐらいがちょうどよいと思う．
	const int kSphereDivNum;	//!< ロボットのモデルの球をどれだけ細かく描画するか．16 ～ 32ぐらいがちょうどよいと思う．
	const float kLegRadius;		//!< 脚の半径．このクラスでは脚を円柱に近似して描画している．描画時のデータのため，これを変化させてもシミュレーションに影響はない．
	const float kJointRadius;	//!< ジョイントの半径．描画時のデータのため，これを変化させてもシミュレーションに影響はない．

	const bool kDoOutputDebugLog = false;	//!< 脚状態を文字列で出力するかどうか


	std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	//!< ロボットの状態を計算するクラス

	RobotStateNode draw_node_;						//!< 描画するロボットの状態

	std::array<HexapodJointState, HexapodConst::kLegNum> draw_joint_state_;	//!< 描画するロボットのジョイントの状態

	std::array<PhantomXDrawData, HexapodConst::kLegNum> draw_data_;			//!< 描画に必要なデータ

	DisplayQuality display_quality_;	//!< 描画品質
};

#endif

#endif