#include "hexapod_renderer.h"

#include "DxLib.h"

#include "designlab_dxlib.h"
#include "leg_state.h"
#include "designlab_math.h"


HexapodRenderer::HexapodRenderer(std::shared_ptr<AbstractHexapodStateCalculator> calc) :
	mp_calculator(calc),
	COLOR_BODY(GetColor(23, 58, 235)), COLOR_LEG(GetColor(23, 58, 235)), COLOR_LIFTED_LEG(GetColor(240, 30, 60)),
	COLOR_JOINT(GetColor(100, 100, 200)), COLOR_LIFTED_JOINT(GetColor(200, 100, 100)), CAPSULE_DIV_NUM(6), SPHERE_DIV_NUM(16),
	COLOR_LEG_BASE(GetColor(100, 200, 100))
{
}


void HexapodRenderer::setNode(const SNode& node)
{
	m_draw_node = node;

	mp_calculator->calculateAllJointState(node, m_draw_joint_state);
}


void HexapodRenderer::draw(const SNode& node) const
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
		const VECTOR kCoxaJointPos = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[0]);
		const VECTOR kFemurJointPos = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[1]);
		const VECTOR kTibiaJointPos = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[2]);
		const VECTOR kLegEndPos = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[3]);

		const VECTOR kLegBasePos = dl_dxlib::convertToDxVec(m_draw_joint_state[i].global_joint_position[3]); //dl_dxlib::convertToDxVec(m_HexaCalc.getGlobalLegBasePos(node, i, true));


		//ãrÇÃêFÇóVãrÅEê⁄ínÇ≈ïœçXÇ∑ÇÈÅD
		const unsigned int kLegBaseColor = dl_leg::isGrounded(node.leg_state, i) ? COLOR_LEG : COLOR_LIFTED_LEG;
		const unsigned int kJointColor = dl_leg::isGrounded(node.leg_state, i) ? COLOR_JOINT : COLOR_LIFTED_JOINT;


		//äeãrÇÃï`âÊ
		DrawCapsule3D(kCoxaJointPos, kFemurJointPos, LEG_R, CAPSULE_DIV_NUM, kLegBaseColor, kLegBaseColor, TRUE);	//coxa
		DrawCapsule3D(kFemurJointPos, kTibiaJointPos, LEG_R, CAPSULE_DIV_NUM, kLegBaseColor, kLegBaseColor, TRUE);	//femur
		DrawCone3D(kLegEndPos, kTibiaJointPos, LEG_R, CAPSULE_DIV_NUM, kLegBaseColor, kLegBaseColor, TRUE);	//tibia 

		//ä‘ê⁄ÇÃï`âÊ
		DrawSphere3D(kCoxaJointPos, JOINT_R, SPHERE_DIV_NUM, kJointColor, kJointColor, TRUE);
		DrawSphere3D(kFemurJointPos, JOINT_R, SPHERE_DIV_NUM, kJointColor, kJointColor, TRUE);
		DrawSphere3D(kTibiaJointPos, JOINT_R, SPHERE_DIV_NUM, kJointColor, COLOR_LIFTED_JOINT, TRUE);

		//ãrêÊÇÃï`âÊ
		DrawSphere3D(kLegEndPos, JOINT_R / 2, SPHERE_DIV_NUM, kJointColor, kJointColor, TRUE);

		//ãrÇÃÉxÅ[ÉXç¿ïWÇÃï`âÊ
		DrawSphere3D(kLegBasePos, JOINT_R / 3, SPHERE_DIV_NUM, COLOR_LEG_BASE, COLOR_LEG_BASE, TRUE);


		////ÉGÉâÅ[èoóÕ
		//if (isAbleCoxaLeg(m_HexaCalc.getGlobalCoxaJointPos(node, i, true), m_HexaCalc.getGlobalFemurJointPos(node, i)) == false)
		//{
		//	DrawString((int)ConvWorldPosToScreenPos(kCoxaJointPos).x, (int)ConvWorldPosToScreenPos(kCoxaJointPos).y, "Error:Coxa", GetColor(255, 64, 64));
		//}

		//if (isAbleFemurLeg(m_HexaCalc.getGlobalFemurJointPos(node, i), m_HexaCalc.getGlobalTibiaJointPos(node, i)) == false)
		//{
		//	DrawString((int)ConvWorldPosToScreenPos(kFemurJointPos).x, (int)ConvWorldPosToScreenPos(kFemurJointPos).y, "Error:Femur", GetColor(64, 255, 64));
		//}

		//if (isAbleTibiaLeg(m_HexaCalc.getGlobalTibiaJointPos(node, i), m_HexaCalc.getGlobalLegPos(node, i, true)) == false)
		//{
		//	DrawString((int)ConvWorldPosToScreenPos(kFemurJointPos).x, (int)ConvWorldPosToScreenPos(kFemurJointPos).y, "Error:Tibia", GetColor(64, 64, 255));
		//}

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
