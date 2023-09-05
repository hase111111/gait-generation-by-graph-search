#include "hexapod_renderer.h"

#include "DxLib.h"

#include "designlab_dxlib.h"
#include "leg_state.h"
#include "designlab_math.h"


HexapodRenderer::HexapodRenderer(std::shared_ptr<AbstractHexapodStateCalculator> calc) :
	mp_calculator(calc),
	COLOR_BODY(GetColor(23, 58, 235)), COLOR_LEG(GetColor(23, 58, 235)), COLOR_LIFTED_LEG(GetColor(240, 30, 60)),
	COLOR_JOINT(GetColor(100, 100, 200)), COLOR_LIFTED_JOINT(GetColor(200, 100, 100)), CAPSULE_DIV_NUM(6), SPHERE_DIV_NUM(16),
	COLOR_LEG_BASE(GetColor(100, 200, 100)), COLOR_KINE_LEG(GetColor(45, 45, 100)), COLOR_KINE_JOINT(GetColor(60, 60, 115)),
	COLOR_ERROR_TEXT(GetColor(32, 32, 32)), COLOR_ERROR_JOINT(GetColor(180, 180, 64))
{
}


void HexapodRenderer::setNode(const SNode& node)
{
	if (!mp_calculator) { return; }	//åvéZäÌÇ™Ç»Ç¢Ç»ÇÁÇŒâΩÇ‡ÇµÇ»Ç¢

	m_draw_node = node;

	mp_calculator->calculateAllJointState(node, m_draw_joint_state);

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		kCoxaJointPos[i] = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[0]);
		kFemurJointPos[i] = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[1]);
		kTibiaJointPos[i] = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[2]);
		kLegEndPos[i] = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[3]);
		kLegBasePos[i] = dl_dxlib::convertToDxVec(mp_calculator->getGlobalLegPosition(i, m_draw_node.leg_base_pos[i], m_draw_node.global_center_of_mass, m_draw_node.rot, true));

		kCoxaCos[i] = std::cos(m_draw_joint_state[i].joint_angle[0]);
		kCoxaSin[i] = std::sin(m_draw_joint_state[i].joint_angle[0]);
		kFemurCos[i] = std::cos(m_draw_joint_state[i].joint_angle[1]);
		kFemurSin[i] = std::sin(m_draw_joint_state[i].joint_angle[1]);
		kTibiaCos[i] = std::cos(m_draw_joint_state[i].joint_angle[1] + m_draw_joint_state[i].joint_angle[2]);
		kTibiaSin[i] = std::sin(m_draw_joint_state[i].joint_angle[1] + m_draw_joint_state[i].joint_angle[2]);

		kKineCoxaJointVec[i] = m_draw_joint_state[i].local_joint_position[0];
		kKineFemurJointVec[i] = kKineCoxaJointVec[i] + HexapodConst::PHANTOMX_COXA_LENGTH * dl_vec::SVector{kCoxaCos[i], kCoxaSin[i], 0};
		kKineTibiaJointVec[i] = kKineFemurJointVec[i] + HexapodConst::PHANTOMX_FEMUR_LENGTH * dl_vec::SVector{kCoxaCos[i] * kFemurCos[i], kCoxaSin[i] * kFemurCos[i], kFemurSin[i]};
		kKineLegVec[i] = kKineTibiaJointVec[i] + HexapodConst::PHANTOMX_TIBIA_LENGTH * dl_vec::SVector{kCoxaCos[i] * kTibiaCos[i], kCoxaSin[i] * kTibiaCos[i], kTibiaSin[i]};

		kKineCoxaJointPos[i] = dl_dxlib::convertToDxVec(mp_calculator->getGlobalLegPosition(i, kKineCoxaJointVec[i], m_draw_node.global_center_of_mass, m_draw_node.rot, true));
		kKineFemurJointPos[i] = dl_dxlib::convertToDxVec(mp_calculator->getGlobalLegPosition(i, kKineFemurJointVec[i], m_draw_node.global_center_of_mass, m_draw_node.rot, true));
		kKineTibiaJointPos[i] = dl_dxlib::convertToDxVec(mp_calculator->getGlobalLegPosition(i, kKineTibiaJointVec[i], m_draw_node.global_center_of_mass, m_draw_node.rot, true));
		kKineLegPos[i] = dl_dxlib::convertToDxVec(mp_calculator->getGlobalLegPosition(i, kKineLegVec[i], m_draw_node.global_center_of_mass, m_draw_node.rot, true));

		kCoxaLinkLength[i] = (m_draw_joint_state[i].local_joint_position[0] - m_draw_joint_state[i].local_joint_position[1]).length();
		kFemurLinkLength[i] = (m_draw_joint_state[i].local_joint_position[1] - m_draw_joint_state[i].local_joint_position[2]).length();
		kTibiaLinkLength[i] = (m_draw_joint_state[i].local_joint_position[2] - m_draw_joint_state[i].local_joint_position[3]).length();

		kIsAbleCoxaAngle[i] = !(m_draw_joint_state[i].joint_angle[0] < HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[i] + HexapodConst::PHANTOMX_COXA_ANGLE_MIN ||
			HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[i] + HexapodConst::PHANTOMX_COXA_ANGLE_MAX < m_draw_joint_state[i].joint_angle[0]);
		kIsAbleFemurAngle[i] = !(m_draw_joint_state[i].joint_angle[1] < HexapodConst::PHANTOMX_FEMUR_ANGLE_MIN || HexapodConst::PHANTOMX_FEMUR_ANGLE_MAX < m_draw_joint_state[i].joint_angle[1]);
		kIsAbleTibiaAngle[i] = !(m_draw_joint_state[i].joint_angle[2] < HexapodConst::PHANTOMX_TIBIA_ANGLE_MIN || HexapodConst::PHANTOMX_TIBIA_ANGLE_MAX < m_draw_joint_state[i].joint_angle[2]);
	}
}


void HexapodRenderer::draw() const
{
	//ì∑ëÃÇï`âÊÇ∑ÇÈÅD
	VECTOR vertex[6];

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//vertex[i] = convertToDxVec(_hexapod.getGlobalCoxaJointPos(i));
		vertex[i] = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[0]);
	}

	dl_dxlib::drawHexagonalPrism(vertex, HexapodConst::BODY_HEIGHT, COLOR_BODY);

	//ãrÇï`âÊÇ∑ÇÈÅD
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//ãrÇÃêFÇóVãrÅEê⁄ínÇ≈ïœçXÇ∑ÇÈÅD
		const unsigned int kLegBaseColor = dl_leg::isGrounded(m_draw_node.leg_state, i) ? COLOR_LEG : COLOR_LIFTED_LEG;
		const unsigned int kJointColor = dl_leg::isGrounded(m_draw_node.leg_state, i) ? COLOR_JOINT : COLOR_LIFTED_JOINT;

		//äeãrÇÃï`âÊ
		DrawCapsule3D(kCoxaJointPos[i], kFemurJointPos[i], LEG_R, CAPSULE_DIV_NUM, kLegBaseColor, kLegBaseColor, TRUE);	//coxa
		DrawCapsule3D(kFemurJointPos[i], kTibiaJointPos[i], LEG_R, CAPSULE_DIV_NUM, kLegBaseColor, kLegBaseColor, TRUE);	//femur
		DrawCone3D(kLegEndPos[i], kTibiaJointPos[i], LEG_R, CAPSULE_DIV_NUM, kLegBaseColor, kLegBaseColor, TRUE);	//tibia 

		//ä‘ê⁄ÇÃï`âÊ
		DrawSphere3D(kCoxaJointPos[i], JOINT_R, SPHERE_DIV_NUM, kIsAbleCoxaAngle[i] ? kJointColor : COLOR_ERROR_JOINT, kIsAbleCoxaAngle[i] ? kJointColor : COLOR_ERROR_JOINT, TRUE);
		DrawSphere3D(kFemurJointPos[i], JOINT_R, SPHERE_DIV_NUM, kIsAbleFemurAngle[i] ? kJointColor : COLOR_ERROR_JOINT, kIsAbleFemurAngle[i] ? kJointColor : COLOR_ERROR_JOINT, TRUE);
		DrawSphere3D(kTibiaJointPos[i], JOINT_R, SPHERE_DIV_NUM, kIsAbleTibiaAngle[i] ? kJointColor : COLOR_ERROR_JOINT, kIsAbleTibiaAngle[i] ? kJointColor : COLOR_ERROR_JOINT, TRUE);

		//ãrêÊÇÃï`âÊ
		DrawSphere3D(kLegEndPos[i], JOINT_R / 2, SPHERE_DIV_NUM, kJointColor, kJointColor, TRUE);

		//ãrÇÃÉxÅ[ÉXç¿ïWÇÃï`âÊ
		DrawSphere3D(kLegBasePos[i], JOINT_R / 3, SPHERE_DIV_NUM, COLOR_LEG_BASE, COLOR_LEG_BASE, TRUE);


		//â^ìÆäwÇ≈åvéZÇµÇΩãrÇÃï`âÊ
		DrawCapsule3D(kKineCoxaJointPos[i], kKineFemurJointPos[i], LEG_R - 5, CAPSULE_DIV_NUM, COLOR_KINE_LEG, COLOR_KINE_LEG, TRUE);	//coxa
		DrawCapsule3D(kKineFemurJointPos[i], kKineTibiaJointPos[i], LEG_R - 5, CAPSULE_DIV_NUM, COLOR_KINE_LEG, COLOR_KINE_LEG, TRUE);	//femur
		DrawCone3D(kKineLegPos[i], kKineTibiaJointPos[i], LEG_R - 5, CAPSULE_DIV_NUM, COLOR_KINE_LEG, COLOR_KINE_LEG, TRUE);	//tibia

		//â^ìÆäwÇ≈åvéZÇµÇΩä‘ê⁄ÇÃï`âÊ
		DrawSphere3D(kKineCoxaJointPos[i], JOINT_R - 5, SPHERE_DIV_NUM, COLOR_KINE_JOINT, COLOR_KINE_JOINT, TRUE);
		DrawSphere3D(kKineFemurJointPos[i], JOINT_R - 5, SPHERE_DIV_NUM, COLOR_KINE_JOINT, COLOR_KINE_JOINT, TRUE);
		DrawSphere3D(kKineTibiaJointPos[i], JOINT_R - 5, SPHERE_DIV_NUM, COLOR_KINE_JOINT, COLOR_KINE_JOINT, TRUE);

		//â^ìÆäwÇ≈åvéZÇµÇΩãrêÊÇÃï`âÊ
		DrawSphere3D(kKineLegPos[i], JOINT_R / 2 - 2, SPHERE_DIV_NUM, COLOR_KINE_LEG, COLOR_KINE_LEG, TRUE);


		////ÉGÉâÅ[èoóÕ
		if (!dl_math::isEqual(kCoxaLinkLength[i], HexapodConst::PHANTOMX_COXA_LENGTH))
		{
			DrawString(
				(int)ConvWorldPosToScreenPos(dl_dxlib::convertToDxVec((m_draw_joint_state[i].global_joint_position[0] + m_draw_joint_state[i].global_joint_position[1]) / 2)).x,
				(int)ConvWorldPosToScreenPos(dl_dxlib::convertToDxVec((m_draw_joint_state[i].global_joint_position[0] + m_draw_joint_state[i].global_joint_position[1]) / 2)).y,
				"Error : Coxa Length", COLOR_ERROR_TEXT);
		}

		if (!dl_math::isEqual(kFemurLinkLength[i], HexapodConst::PHANTOMX_FEMUR_LENGTH))
		{
			DrawString(
				(int)ConvWorldPosToScreenPos(dl_dxlib::convertToDxVec((m_draw_joint_state[i].global_joint_position[1] + m_draw_joint_state[i].global_joint_position[2]) / 2)).x,
				(int)ConvWorldPosToScreenPos(dl_dxlib::convertToDxVec((m_draw_joint_state[i].global_joint_position[1] + m_draw_joint_state[i].global_joint_position[2]) / 2)).y,
				"Error : Femur Length", COLOR_ERROR_TEXT);
		}

		if (!dl_math::isEqual(kTibiaLinkLength[i], HexapodConst::PHANTOMX_TIBIA_LENGTH))
		{
			DrawString(
				(int)ConvWorldPosToScreenPos(dl_dxlib::convertToDxVec((m_draw_joint_state[i].global_joint_position[2] + m_draw_joint_state[i].global_joint_position[3]) / 2)).x,
				(int)ConvWorldPosToScreenPos(dl_dxlib::convertToDxVec((m_draw_joint_state[i].global_joint_position[2] + m_draw_joint_state[i].global_joint_position[3]) / 2)).y,
				"Error : Tibia Length", COLOR_ERROR_TEXT);
		}



		//if (DO_OUTPUT_DEBUG_LOG)
		//{
		//	if (m_HexaCalc.isLegInRange(node, i))printfDx("ÅZ");
		//	else printfDx("Å~");

		//	printfDx(" LegNum: %d \t", i);
		//	printfDx("Max : %.3f, min : %.3f\t", m_HexaCalc.getMaxLegR(node.leg_pos[i].z), m_HexaCalc.getMinLegR(node.leg_pos[i].z));
		//	printfDx("%.3f\t", node.leg_pos[i].length());

		//	if (m_HexaCalc.isLegInRange(node, i))printfDx("is in range   ");
		//	else printfDx("isnot in range");

		//	if (node.leg_base_pos[i].projectedXY().cross(node.leg_pos[i].projectedXY()) * node.leg_pos[i].projectedXY().cross({ 1,0 }) > 0)
		//	{
		//		printfDx("front - 567\n");
		//	}
		//	else
		//	{
		//		printfDx("back - 123\n");
		//	}
		//}
	}

	//if (DO_OUTPUT_DEBUG_LOG)
	//{
	//	//is Able Pause
	//	if (m_HexaCalc.isAblePause(node)) { printfDx("ÅZ is Able Pause\n"); }
	//	else { printfDx("Å~ isnot Able Pause\n"); }

	//	// leg Interfering
	//	if (m_HexaCalc.isLegInterfering(node)) { printfDx("Å~ is Leg Interfering\n"); }
	//	else { printfDx("ÅZ isnot Leg Interfering\n"); }
	//}
}


bool HexapodRenderer::isAbleCoxaLeg(const dl_vec::SVector& coxa_joint, const dl_vec::SVector& femur_joint) const
{
	if (abs((coxa_joint - femur_joint).length() - HexapodConst::PHANTOMX_COXA_LENGTH) < dl_math::ALLOWABLE_ERROR) { return true; }
	return false;
}


bool HexapodRenderer::isAbleFemurLeg(const dl_vec::SVector& femur_joint, const dl_vec::SVector& tibia_joint) const
{
	if (abs((femur_joint - tibia_joint).length() - HexapodConst::PHANTOMX_FEMUR_LENGTH) < dl_math::ALLOWABLE_ERROR) { return true; }
	return false;
}


bool HexapodRenderer::isAbleTibiaLeg(const dl_vec::SVector& tibia_joint, const dl_vec::SVector& leg_joint) const
{
	if (abs((tibia_joint - leg_joint).length() - HexapodConst::PHANTOMX_TIBIA_LENGTH) < 10) { return true; }
	return false;
}
