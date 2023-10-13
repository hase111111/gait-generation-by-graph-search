//! @file phantomx_renderer_model.h
//! @brief ロボットの描画を行うクラス．

#ifndef DESIGNLAB_PHANTOMX_RENDERER_MODEL_H_
#define DESIGNLAB_PHANTOMX_RENDERER_MODEL_H_


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


//! @class PhantomXRendererModel
//! @brief PhantomXの描画を行うクラス．3Dモデルを使用する
class PhantomXRendererModel final : public IHexapodRenderer
{
public:

	PhantomXRendererModel(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr);

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

	bool IsAbleCoxaLeg(const designlab::Vector3& coxa_joint, const designlab::Vector3& femur_joint) const;
	bool IsAbleFemurLeg(const designlab::Vector3& femur_joint, const designlab::Vector3& tibia_joint) const;
	bool IsAbleTibiaLeg(const designlab::Vector3& tibia_joint, const designlab::Vector3& leg_joint) const;

	void DrawBody() const;

	void DrawCoxaLink(int leg_index) const;


	std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	//!< ロボットの状態を計算するクラス

	RobotStateNode draw_node_;						//!< 描画するロボットの状態

	std::array<HexapodJointState, HexapodConst::kLegNum> draw_joint_state_;	//!< 描画するロボットのジョイントの状態

	std::array<PhantomXDrawData, HexapodConst::kLegNum> draw_data_;			//!< 描画に必要なデータ

	DisplayQuality display_quality_;	//!< 描画品質
};

#endif

#endif