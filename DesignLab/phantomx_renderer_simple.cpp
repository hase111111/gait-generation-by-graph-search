#include "phantomx_renderer_simple.h"

#include <array>

#include "dxlib_util.h"
#include "designlab_math_util.h"
#include "leg_state.h"
#include "phantomx_const.h"


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
	SetDrawNode(RobotStateNode{});
}


void PhantomXRendererSimple::SetDrawNode(const RobotStateNode& node)
{
	draw_node_ = node;

	if (!calculator_ptr_) { return; }	//åvéZäÌÇ™Ç»Ç¢Ç»ÇÁÇŒâΩÇ‡ÇµÇ»Ç¢

	draw_joint_state_ = calculator_ptr_->CalculateAllJointState(node);
}


void PhantomXRendererSimple::Draw() const
{
	// ÉçÉ{ÉbÉgÇÃï`âÊ
	DrawHexapodNormal();


	//// ÉGÉâÅ[èoóÕÅD
	//for (int i = 0; i < HexapodConst::kLegNum; i++)
	//{
	//	if (!dlm::IsEqual(draw_data_[i].coxa_link_length.value(), PhantomXConst::kCoxaLength))
	//	{
	//		DrawString(
	//			static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[0] + draw_joint_state_[i].global_joint_position[1]) / 2)).x),
	//			static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[0] + draw_joint_state_[i].global_joint_position[1]) / 2)).y),
	//			"Error : Coxa Length", kColorErrorText
	//		);
	//	}
	//
	//	if (!dlm::IsEqual(draw_data_[i].femur_link_length, PhantomXConst::kFemurLength))
	//	{
	//		DrawString(
	//			static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[1] + draw_joint_state_[i].global_joint_position[2]) / 2)).x),
	//			static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[1] + draw_joint_state_[i].global_joint_position[2]) / 2)).y),
	//			"Error : Femur Length", kColorErrorText
	//		);
	//	}
	//
	//	if (!dlm::IsEqual(draw_data_[i].tibia_link_length, PhantomXConst::kTibiaLength))
	//	{
	//		DrawString(
	//			static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[2] + draw_joint_state_[i].global_joint_position[3]) / 2)).x),
	//			static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[2] + draw_joint_state_[i].global_joint_position[3]) / 2)).y),
	//			"Error : Tibia Length", kColorErrorText
	//		);
	//	}
	//}
}


void PhantomXRendererSimple::DrawHexapodNormal() const
{
	//ì∑ëÃÇï`âÊÇ∑ÇÈÅD
	std::array<VECTOR, HexapodConst::kLegNum> vertex;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		vertex[i] = dldu::ConvertToDxlibVec(
			converter_ptr_->ConvertLegToGlobalCoordinate(
				draw_joint_state_[i].joint_pos_leg_coordinate[0], 
				i, 
				draw_node_.global_center_of_mass, 
				draw_node_.rot, 
				true
			)
		);
	}

	dldu::DrawHexagonalPrism(vertex, PhantomXConst::kBodyHeight, kColorBody);

	//èdêSÇÃï`âÊ
	DrawSphere3D(dldu::ConvertToDxlibVec(draw_node_.global_center_of_mass), kJointRadius * 1.5f, kSphereDivNum, kColorJoint, kColorJoint, TRUE);

	//ãrÇï`âÊÇ∑ÇÈÅD
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		//ãrÇÃêFÇóVãrÅEê⁄ínÇ≈ïœçXÇ∑ÇÈÅD
		const unsigned int kLegBaseColor = dllf::IsGrounded(draw_node_.leg_state, i) ? kColorLeg : kColorLiftedLeg;
		const unsigned int kJointColor = dllf::IsGrounded(draw_node_.leg_state, i) ? kColorJoint : kColorLiftedJoint;

		if (draw_joint_state_[i].joint_pos_leg_coordinate.size() != 4) { continue; }
		if (draw_joint_state_[i].joint_angle.size() != 3) { continue; }

		//äeãrÇÃï`âÊ
		for (int j = 0; j < 3; j++)
		{
			const VECTOR start = dldu::ConvertToDxlibVec(
				converter_ptr_->ConvertLegToGlobalCoordinate(
					draw_joint_state_[i].joint_pos_leg_coordinate[j], 
					i, 
					draw_node_.global_center_of_mass, 
					draw_node_.rot, 
					true
				)
			);

			const VECTOR end = dldu::ConvertToDxlibVec(
				converter_ptr_->ConvertLegToGlobalCoordinate(
					draw_joint_state_[i].joint_pos_leg_coordinate[j + 1],
					i,
					draw_node_.global_center_of_mass,
					draw_node_.rot,
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

		
		//ä‘ê⁄ÇÃï`âÊ
		for (int j = 0; j < 4; j++) 
		{
			unsigned int color = kJointColor;

			if (j == 0 && !PhantomXConst::IsVaildCoxaAngle(i, draw_joint_state_[i].joint_angle[0])) { color = kColorErrorJoint; }
			if (j == 1 && !PhantomXConst::IsVaildFemurAngle(draw_joint_state_[i].joint_angle[1])) { color = kColorErrorJoint; }
			if (j == 2 && !PhantomXConst::IsVaildTibiaAngle(draw_joint_state_[i].joint_angle[2])) { color = kColorErrorJoint; }

			VECTOR pos = dldu::ConvertToDxlibVec(
				converter_ptr_->ConvertLegToGlobalCoordinate(
					draw_joint_state_[i].joint_pos_leg_coordinate[j], 
					i, 
					draw_node_.global_center_of_mass, 
					draw_node_.rot, 
					true
				)
			);

			DrawSphere3D(pos, kJointRadius, kSphereDivNum, color, color, TRUE);
		}
		
		DrawSphere3D(
			dldu::ConvertToDxlibVec(converter_ptr_->ConvertLegToGlobalCoordinate(draw_node_.leg_pos[i], i, draw_node_.global_center_of_mass, draw_node_.rot, true)),
			kJointRadius / 2,
			kSphereDivNum, 
			kJointColor, 
			kJointColor, 
			TRUE
		);

		//ãrÇÃê⁄ínÇÃäÓèÄínì_ÇÃï`âÊ
		DrawSphere3D(dldu::ConvertToDxlibVec(draw_node_.leg_reference_pos[i]), kJointRadius / 3, kSphereDivNum, kColorLegBase, kColorLegBase, TRUE);
	}
}

bool PhantomXRendererSimple::IsAbleCoxaLeg(const designlab::Vector3& coxa_joint, const designlab::Vector3& femur_joint) const
{
	if (abs((coxa_joint - femur_joint).GetLength() - PhantomXConst::kCoxaLength) < dlm::kAllowableError) { return true; }
	return false;
}


bool PhantomXRendererSimple::IsAbleFemurLeg(const designlab::Vector3& femur_joint, const designlab::Vector3& tibia_joint) const
{
	if (abs((femur_joint - tibia_joint).GetLength() - PhantomXConst::kFemurLength) < dlm::kAllowableError) { return true; }
	return false;
}


bool PhantomXRendererSimple::IsAbleTibiaLeg(const designlab::Vector3& tibia_joint, const designlab::Vector3& leg_joint) const
{
	if (abs((tibia_joint - leg_joint).GetLength() - PhantomXConst::kTibiaLength) < 10) { return true; }
	return false;
}