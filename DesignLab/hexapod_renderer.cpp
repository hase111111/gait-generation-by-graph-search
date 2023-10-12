#include "hexapod_renderer.h"

#include <array>

#include "dxlib_util.h"
#include "leg_state.h"
#include "designlab_math_util.h"


namespace dldu = designlab::dxlib_util;
namespace dllf = designlab::leg_func;
namespace dlm = designlab::math_util;


HexapodRenderer::HexapodRenderer(const std::shared_ptr<const AbstractHexapodStateCalculator>& calc) :
	calculator_ptr_(calc),
	kColorBody(GetColor(23, 58, 235)),
	kColorLeg(GetColor(23, 58, 235)),
	kColorLiftedLeg(GetColor(240, 30, 60)),
	kColorJoint(GetColor(100, 100, 200)),
	kColorLiftedJoint(GetColor(200, 100, 100)),
	kCapsuleDivNum(6),
	kSphereDivNum(16),
	kLegRadius(10.0f),
	kJointRadius(20.0f),
	kColorLegBase(GetColor(100, 200, 100)),
	kColorKineLeg(GetColor(45, 45, 100)),
	kColorKineJoint(GetColor(60, 60, 115)),
	kColorErrorText(GetColor(32, 32, 32)),
	kColorErrorJoint(GetColor(180, 180, 64))
{
	SetDrawNode(RobotStateNode{});
}


void HexapodRenderer::SetDrawNode(const RobotStateNode& node)
{
	if (!calculator_ptr_) { return; }	//åvéZäÌÇ™Ç»Ç¢Ç»ÇÁÇŒâΩÇ‡ÇµÇ»Ç¢

	draw_node_ = node;

	calculator_ptr_->CalculateAllJointState(node, &draw_joint_state_);

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		kCoxaJointPos[i] = dldu::ConvertToDxlibVec(draw_joint_state_[i].global_joint_position[0]);
		kFemurJointPos[i] = dldu::ConvertToDxlibVec(draw_joint_state_[i].global_joint_position[1]);
		kTibiaJointPos[i] = dldu::ConvertToDxlibVec(draw_joint_state_[i].global_joint_position[2]);
		kLegEndPos[i] = dldu::ConvertToDxlibVec(draw_joint_state_[i].global_joint_position[3]);
		kLegBasePos[i] = dldu::ConvertToDxlibVec(calculator_ptr_->GetGlobalLegPosition(i, draw_node_.leg_reference_pos[i], draw_node_.global_center_of_mass, draw_node_.rot, true));

		kCoxaCos[i] = std::cos(draw_joint_state_[i].joint_angle[0]);
		kCoxaSin[i] = std::sin(draw_joint_state_[i].joint_angle[0]);
		kFemurCos[i] = std::cos(draw_joint_state_[i].joint_angle[1]);
		kFemurSin[i] = std::sin(draw_joint_state_[i].joint_angle[1]);
		kTibiaCos[i] = std::cos(draw_joint_state_[i].joint_angle[1] + draw_joint_state_[i].joint_angle[2]);
		kTibiaSin[i] = std::sin(draw_joint_state_[i].joint_angle[1] + draw_joint_state_[i].joint_angle[2]);

		kKineCoxaJointVec[i] = draw_joint_state_[i].local_joint_position[0];
		kKineFemurJointVec[i] = kKineCoxaJointVec[i] + HexapodConst::PHANTOMX_COXA_LENGTH * designlab::Vector3{kCoxaCos[i], kCoxaSin[i], 0};
		kKineTibiaJointVec[i] = kKineFemurJointVec[i] + HexapodConst::PHANTOMX_FEMUR_LENGTH * designlab::Vector3{kCoxaCos[i] * kFemurCos[i], kCoxaSin[i] * kFemurCos[i], kFemurSin[i]};
		kKineLegVec[i] = kKineTibiaJointVec[i] + HexapodConst::PHANTOMX_TIBIA_LENGTH * designlab::Vector3{kCoxaCos[i] * kTibiaCos[i], kCoxaSin[i] * kTibiaCos[i], kTibiaSin[i]};

		kKineCoxaJointPos[i] = dldu::ConvertToDxlibVec(calculator_ptr_->GetGlobalLegPosition(i, kKineCoxaJointVec[i], draw_node_.global_center_of_mass, draw_node_.rot, true));
		kKineFemurJointPos[i] = dldu::ConvertToDxlibVec(calculator_ptr_->GetGlobalLegPosition(i, kKineFemurJointVec[i], draw_node_.global_center_of_mass, draw_node_.rot, true));
		kKineTibiaJointPos[i] = dldu::ConvertToDxlibVec(calculator_ptr_->GetGlobalLegPosition(i, kKineTibiaJointVec[i], draw_node_.global_center_of_mass, draw_node_.rot, true));
		kKineLegPos[i] = dldu::ConvertToDxlibVec(calculator_ptr_->GetGlobalLegPosition(i, kKineLegVec[i], draw_node_.global_center_of_mass, draw_node_.rot, true));

		kCoxaLinkLength[i] = (draw_joint_state_[i].local_joint_position[0] - draw_joint_state_[i].local_joint_position[1]).GetLength();
		kFemurLinkLength[i] = (draw_joint_state_[i].local_joint_position[1] - draw_joint_state_[i].local_joint_position[2]).GetLength();
		kTibiaLinkLength[i] = (draw_joint_state_[i].local_joint_position[2] - draw_joint_state_[i].local_joint_position[3]).GetLength();

		kIsAbleCoxaAngle[i] = !(draw_joint_state_[i].joint_angle[0] < HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[i] + HexapodConst::PHANTOMX_COXA_ANGLE_MIN ||
			HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[i] + HexapodConst::PHANTOMX_COXA_ANGLE_MAX < draw_joint_state_[i].joint_angle[0]);
		kIsAbleFemurAngle[i] = !(draw_joint_state_[i].joint_angle[1] < HexapodConst::PHANTOMX_FEMUR_ANGLE_MIN || HexapodConst::PHANTOMX_FEMUR_ANGLE_MAX < draw_joint_state_[i].joint_angle[1]);
		kIsAbleTibiaAngle[i] = !(draw_joint_state_[i].joint_angle[2] < HexapodConst::PHANTOMX_TIBIA_ANGLE_MIN || HexapodConst::PHANTOMX_TIBIA_ANGLE_MAX < draw_joint_state_[i].joint_angle[2]);
	}
}


void HexapodRenderer::Draw() const
{
	//ì∑ëÃÇï`âÊÇ∑ÇÈÅD
	std::array<VECTOR, HexapodConst::kLegNum> vertex;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		vertex[i] = dldu::ConvertToDxlibVec(draw_joint_state_[i].global_joint_position[0]);
	}

	dldu::DrawHexagonalPrism(vertex, HexapodConst::BODY_HEIGHT, kColorBody);

	//ãrÇï`âÊÇ∑ÇÈÅD
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		//ãrÇÃêFÇóVãrÅEê⁄ínÇ≈ïœçXÇ∑ÇÈÅD
		const unsigned int kLegBaseColor = dllf::IsGrounded(draw_node_.leg_state, i) ? kColorLeg : kColorLiftedLeg;
		const unsigned int kJointColor = dllf::IsGrounded(draw_node_.leg_state, i) ? kColorJoint : kColorLiftedJoint;

		//äeãrÇÃï`âÊ
		DrawCapsule3D(kCoxaJointPos[i], kFemurJointPos[i], kLegRadius, kCapsuleDivNum, kLegBaseColor, kLegBaseColor, TRUE);	//coxa
		DrawCapsule3D(kFemurJointPos[i], kTibiaJointPos[i], kLegRadius, kCapsuleDivNum, kLegBaseColor, kLegBaseColor, TRUE);	//femur
		DrawCone3D(kLegEndPos[i], kTibiaJointPos[i], kLegRadius, kCapsuleDivNum, kLegBaseColor, kLegBaseColor, TRUE);	//tibia 

		//ä‘ê⁄ÇÃï`âÊ
		DrawSphere3D(kCoxaJointPos[i], kJointRadius, kSphereDivNum, kIsAbleCoxaAngle[i] ? kJointColor : kColorErrorJoint, kIsAbleCoxaAngle[i] ? kJointColor : kColorErrorJoint, TRUE);
		DrawSphere3D(kFemurJointPos[i], kJointRadius, kSphereDivNum, kIsAbleFemurAngle[i] ? kJointColor : kColorErrorJoint, kIsAbleFemurAngle[i] ? kJointColor : kColorErrorJoint, TRUE);
		DrawSphere3D(kTibiaJointPos[i], kJointRadius, kSphereDivNum, kIsAbleTibiaAngle[i] ? kJointColor : kColorErrorJoint, kIsAbleTibiaAngle[i] ? kJointColor : kColorErrorJoint, TRUE);

		//ãrêÊÇÃï`âÊ
		DrawSphere3D(kLegEndPos[i], kJointRadius / 2, kSphereDivNum, kJointColor, kJointColor, TRUE);

		//ãrÇÃÉxÅ[ÉXç¿ïWÇÃï`âÊ
		DrawSphere3D(kLegBasePos[i], kJointRadius / 3, kSphereDivNum, kColorLegBase, kColorLegBase, TRUE);


		//â^ìÆäwÇ≈åvéZÇµÇΩãrÇÃï`âÊ
		DrawCapsule3D(kKineCoxaJointPos[i], kKineFemurJointPos[i], kLegRadius - 5, kCapsuleDivNum, kColorKineLeg, kColorKineLeg, TRUE);	//coxa
		DrawCapsule3D(kKineFemurJointPos[i], kKineTibiaJointPos[i], kLegRadius - 5, kCapsuleDivNum, kColorKineLeg, kColorKineLeg, TRUE);	//femur
		DrawCone3D(kKineLegPos[i], kKineTibiaJointPos[i], kLegRadius - 5, kCapsuleDivNum, kColorKineLeg, kColorKineLeg, TRUE);	//tibia

		//â^ìÆäwÇ≈åvéZÇµÇΩä‘ê⁄ÇÃï`âÊ
		DrawSphere3D(kKineCoxaJointPos[i], kJointRadius - 5, kSphereDivNum, kColorKineJoint, kColorKineJoint, TRUE);
		DrawSphere3D(kKineFemurJointPos[i], kJointRadius - 5, kSphereDivNum, kColorKineJoint, kColorKineJoint, TRUE);
		DrawSphere3D(kKineTibiaJointPos[i], kJointRadius - 5, kSphereDivNum, kColorKineJoint, kColorKineJoint, TRUE);

		//â^ìÆäwÇ≈åvéZÇµÇΩãrêÊÇÃï`âÊ
		DrawSphere3D(kKineLegPos[i], kJointRadius / 2 - 2, kSphereDivNum, kColorKineLeg, kColorKineLeg, TRUE);


		// ÉGÉâÅ[èoóÕÅD 
		if (!dlm::IsEqual(kCoxaLinkLength[i], HexapodConst::PHANTOMX_COXA_LENGTH))
		{
			DrawString(
				static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[0] + draw_joint_state_[i].global_joint_position[1]) / 2)).x),
				static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[0] + draw_joint_state_[i].global_joint_position[1]) / 2)).y),
				"Error : Coxa Length", kColorErrorText);
		}

		if (!dlm::IsEqual(kFemurLinkLength[i], HexapodConst::PHANTOMX_FEMUR_LENGTH))
		{
			DrawString(
				static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[1] + draw_joint_state_[i].global_joint_position[2]) / 2)).x),
				static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[1] + draw_joint_state_[i].global_joint_position[2]) / 2)).y),
				"Error : Femur Length", kColorErrorText);
		}

		if (!dlm::IsEqual(kTibiaLinkLength[i], HexapodConst::PHANTOMX_TIBIA_LENGTH))
		{
			DrawString(
				static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[2] + draw_joint_state_[i].global_joint_position[3]) / 2)).x),
				static_cast<int>(ConvWorldPosToScreenPos(dldu::ConvertToDxlibVec((draw_joint_state_[i].global_joint_position[2] + draw_joint_state_[i].global_joint_position[3]) / 2)).y),
				"Error : Tibia Length", kColorErrorText);
		}



		//if (kDoOutputDebugLog)
		//{
		//	if (m_HexaCalc.IsLegInRange(node, i))printfDx("ÅZ");
		//	else printfDx("Å~");

		//	printfDx(" LegNum: %d \t", i);
		//	printfDx("Max : %.3f, min : %.3f\t", m_HexaCalc.getMaxLegR(node.leg_pos[i].z), m_HexaCalc.getMinLegR(node.leg_pos[i].z));
		//	printfDx("%.3f\t", node.leg_pos[i].GetLength());

		//	if (m_HexaCalc.IsLegInRange(node, i))printfDx("is in range   ");
		//	else printfDx("isnot in range");

		//	if (node.leg_reference_pos[i].ProjectedXY().Cross(node.leg_pos[i].ProjectedXY()) * node.leg_pos[i].ProjectedXY().Cross({ 1,0 }) > 0)
		//	{
		//		printfDx("front - 567\n");
		//	}
		//	else
		//	{
		//		printfDx("back - 123\n");
		//	}
		//}
	}

	//if (kDoOutputDebugLog)
	//{
	//	//is Able Pause
	//	if (m_HexaCalc.isAblePause(node)) { printfDx("ÅZ is Able Pause\n"); }
	//	else { printfDx("Å~ isnot Able Pause\n"); }

	//	// leg Interfering
	//	if (m_HexaCalc.IsLegInterfering(node)) { printfDx("Å~ is Leg Interfering\n"); }
	//	else { printfDx("ÅZ isnot Leg Interfering\n"); }
	//}
}


bool HexapodRenderer::IsAbleCoxaLeg(const designlab::Vector3& coxa_joint, const designlab::Vector3& femur_joint) const
{
	if (abs((coxa_joint - femur_joint).GetLength() - HexapodConst::PHANTOMX_COXA_LENGTH) < dlm::kAllowableError) { return true; }
	return false;
}


bool HexapodRenderer::IsAbleFemurLeg(const designlab::Vector3& femur_joint, const designlab::Vector3& tibia_joint) const
{
	if (abs((femur_joint - tibia_joint).GetLength() - HexapodConst::PHANTOMX_FEMUR_LENGTH) < dlm::kAllowableError) { return true; }
	return false;
}


bool HexapodRenderer::IsAbleTibiaLeg(const designlab::Vector3& tibia_joint, const designlab::Vector3& leg_joint) const
{
	if (abs((tibia_joint - leg_joint).GetLength() - HexapodConst::PHANTOMX_TIBIA_LENGTH) < 10) { return true; }
	return false;
}
