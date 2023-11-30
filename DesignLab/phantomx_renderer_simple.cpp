#include "phantomx_renderer_simple.h"

#include <array>

#include "dxlib_util.h"
#include "designlab_math_util.h"
#include "leg_state.h"
#include "phantomx_mk2_const.h"


namespace dldu = designlab::dxlib_util;
namespace dllf = designlab::leg_func;
namespace dlm = designlab::math_util;


PhantomXRendererSimple::PhantomXRendererSimple(
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
	DisplayQuality display_quality
) :
	kColorBody(GetColor(23, 58, 235)),
	kColorLeg(GetColor(23, 58, 235)),
	kColorLiftedLeg(GetColor(240, 30, 60)),
	kColorJoint(GetColor(100, 100, 200)),
	kColorLiftedJoint(GetColor(200, 100, 100)),
	kColorLegBase(GetColor(100, 200, 100)),
	kColorKineLeg(GetColor(45, 45, 100)),
	kColorKineJoint(GetColor(60, 60, 115)),
	kColorErrorText(GetColor(32, 32, 32)),
	kColorErrorJoint(GetColor(180, 180, 64)),
	kCapsuleDivNum(6),
	kSphereDivNum(16),
	kLegRadius(10.0f),
	kJointRadius(20.0f),
	converter_ptr_(converter_ptr),
	calculator_ptr_(calculator_ptr),
	display_quality_(display_quality)
{
	SetNode(RobotStateNode{});
}


void PhantomXRendererSimple::SetNode(const RobotStateNode& node)
{
	draw_node_ = node;

	if (!calculator_ptr_) { return; }	//計算器がないならば何もしない

	draw_joint_state_ = calculator_ptr_->CalculateAllJointState(node);
}


void PhantomXRendererSimple::Draw() const
{
	// ロボットの描画
	DrawHexapodNormal();
}


void PhantomXRendererSimple::DrawHexapodNormal() const
{
	//胴体を描画する．
	std::array<VECTOR, HexapodConst::kLegNum> vertex;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		vertex[i] = dldu::ConvertToDxlibVec(
			converter_ptr_->ConvertLegToGlobalCoordinate(
				draw_joint_state_[i].joint_pos_leg_coordinate[0],
				i,
				draw_node_.global_center_of_mass,
				draw_node_.quat,
				true
			)
		);
	}

	dldu::DrawHexagonalPrism(vertex, PhantomXMkIIConst::kBodyHeight, kColorBody);

	//重心の描画
	DrawSphere3D(dldu::ConvertToDxlibVec(draw_node_.global_center_of_mass), kJointRadius * 1.5f, kSphereDivNum, kColorJoint, kColorJoint, TRUE);

	//脚を描画する．
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		//脚の色を遊脚・接地で変更する．
		const unsigned int kLegBaseColor = dllf::IsGrounded(draw_node_.leg_state, i) ? kColorLeg : kColorLiftedLeg;
		const unsigned int kJointColor = dllf::IsGrounded(draw_node_.leg_state, i) ? kColorJoint : kColorLiftedJoint;

		if (draw_joint_state_[i].joint_pos_leg_coordinate.size() != 4) { continue; }
		if (draw_joint_state_[i].joint_angle.size() != 3) { continue; }

		//各脚の描画
		for (int j = 0; j < 3; j++)
		{
			const VECTOR start = dldu::ConvertToDxlibVec(
				converter_ptr_->ConvertLegToGlobalCoordinate(
					draw_joint_state_[i].joint_pos_leg_coordinate[j],
					i,
					draw_node_.global_center_of_mass,
					draw_node_.quat,
					true
				)
			);

			const VECTOR end = dldu::ConvertToDxlibVec(
				converter_ptr_->ConvertLegToGlobalCoordinate(
					draw_joint_state_[i].joint_pos_leg_coordinate[j + 1],
					i,
					draw_node_.global_center_of_mass,
					draw_node_.quat,
					true
				)
			);

			if (draw_joint_state_[i].is_in_range)
			{
				DrawCapsule3D(start, end, kLegRadius, kCapsuleDivNum, kLegBaseColor, kLegBaseColor, TRUE);
			}
			else
			{
				DrawCapsule3D(start, end, kLegRadius, kCapsuleDivNum, kColorErrorJoint, kColorErrorJoint, TRUE);
			}
		}


		//間接の描画
		for (int j = 0; j < 4; j++)
		{
			unsigned int color = kJointColor;

			if (j == 0 && !PhantomXMkIIConst::IsVaildCoxaAngle(i, draw_joint_state_[i].joint_angle[0])) { color = kColorErrorJoint; }
			if (j == 1 && !PhantomXMkIIConst::IsVaildFemurAngle(draw_joint_state_[i].joint_angle[1])) { color = kColorErrorJoint; }
			if (j == 2 && !PhantomXMkIIConst::IsVaildTibiaAngle(draw_joint_state_[i].joint_angle[2])) { color = kColorErrorJoint; }

			VECTOR pos = dldu::ConvertToDxlibVec(
				converter_ptr_->ConvertLegToGlobalCoordinate(
					draw_joint_state_[i].joint_pos_leg_coordinate[j],
					i,
					draw_node_.global_center_of_mass,
					draw_node_.quat,
					true
				)
			);

			DrawSphere3D(pos, kJointRadius, kSphereDivNum, color, color, TRUE);
		}

		DrawSphere3D(
			dldu::ConvertToDxlibVec(converter_ptr_->ConvertLegToGlobalCoordinate(draw_node_.leg_pos[i], i, draw_node_.global_center_of_mass, draw_node_.quat, true)),
			kJointRadius / 2,
			kSphereDivNum,
			kJointColor,
			kJointColor,
			TRUE
		);

		//脚の接地の基準地点の描画
		DrawSphere3D(
			dldu::ConvertToDxlibVec(converter_ptr_->ConvertLegToGlobalCoordinate(draw_node_.leg_reference_pos[i], i, draw_node_.global_center_of_mass, draw_node_.quat, true)),
			kJointRadius / 3,
			kSphereDivNum,
			kColorLegBase,
			kColorLegBase,
			TRUE
		);
	}
}