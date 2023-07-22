#include "HexapodRenderer.h"
#include "DxLib.h"
#include "Dxlib3DFunction.h"
#include "LegState.h"
#include "MyMath.h"


bool HexapodRenderer::isAbleCoxaLeg(const my_vec::SVector _coxa_joint, const my_vec::SVector _femur_joint) const
{
	if (abs((_coxa_joint - _femur_joint).length() - HexapodConst::COXA_LENGTH) < my_math::ALLOWABLE_ERROR) { return true; }
	return false;
}

bool HexapodRenderer::isAbleFemurLeg(const my_vec::SVector _femur_joint, const my_vec::SVector _tibia_joint) const
{
	if (abs((_femur_joint - _tibia_joint).length() - HexapodConst::FEMUR_LENGTH) < my_math::ALLOWABLE_ERROR) { return true; }
	return false;
}

bool HexapodRenderer::isAbleTibiaLeg(const my_vec::SVector _tibia_joint, const my_vec::SVector _leg_joint) const
{
	if (abs((_tibia_joint - _leg_joint).length() - HexapodConst::TIBIA_LENGTH) < 10) { return true; }
	return false;
}

HexapodRenderer::HexapodRenderer() :
	COLOR_BODY(GetColor(23, 58, 235)), COLOR_LEG(GetColor(23, 58, 235)), COLOR_LIFTED_LEG(GetColor(240, 30, 60)),
	COLOR_JOINT(GetColor(100, 100, 200)), COLOR_LIFTED_JOINT(GetColor(200, 100, 100)), CAPSULE_DIV_NUM(6), SPHERE_DIV_NUM(16),
	COLOR_LEG_BASE(GetColor(100, 200, 100))
{
}

void HexapodRenderer::update(const SNode& _node)
{
	m_HexaCalc.calclateJointPos(_node);
}

void HexapodRenderer::draw(const SNode& _node) const
{
	//ãåverÇÃhexapodGraphicÇÃãLèqÇ…ÇµÇΩÇ™Ç¡Çƒï`âÊÇ∑ÇÈÅD
	using namespace myDxlib3DFunc;
	using namespace LegStateEdit;

	//ì∑ëÃÇï`âÊÇ∑ÇÈÅD
	VECTOR _vertex[6];
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//_vertex[i] = convertToDxVec(_hexapod.getGlobalCoxaJointPos(i));
		_vertex[i] = convertToDxVec(m_HexaCalc.getGlobalCoxaJointPos(_node, i, true));
	}

	drawHexagonalPrism(_vertex, HexapodConst::BODY_HEIGHT, COLOR_BODY);

	//ãrÇï`âÊÇ∑ÇÈÅD
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		const VECTOR _leg_end = convertToDxVec(m_HexaCalc.getGlobalLegPos(_node, i, true));
		const VECTOR _leg_base = convertToDxVec(m_HexaCalc.getGlobalLegBasePos(_node, i, true));
		const VECTOR _coxa = convertToDxVec(m_HexaCalc.getGlobalCoxaJointPos(_node, i, true));
		const VECTOR _femur = convertToDxVec(m_HexaCalc.getGlobalFemurJointPos(_node, i));
		const VECTOR _tibia = convertToDxVec(m_HexaCalc.getGlobalTibiaJointPos(_node, i));

		//ãrÇÃêFÇóVãrÅEê⁄ínÇ≈ïœçXÇ∑ÇÈÅD
		const unsigned int _color = isGrounded(_node.leg_state, i) ? COLOR_LEG : COLOR_LIFTED_LEG;
		const unsigned int _joint_color = isGrounded(_node.leg_state, i) ? COLOR_JOINT : COLOR_LIFTED_JOINT;

		//äeãrÇÃï`âÊ
		DrawCapsule3D(_coxa, _femur, LEG_R, CAPSULE_DIV_NUM, _color, _color, TRUE);	//coxa
		DrawCapsule3D(_femur, _tibia, LEG_R, CAPSULE_DIV_NUM, _color, _color, TRUE);	//femur
		DrawCone3D(_leg_end, _tibia, LEG_R, CAPSULE_DIV_NUM, _color, _color, TRUE);	//tibia 

		//ä‘ê⁄ÇÃï`âÊ
		DrawSphere3D(_coxa, JOINT_R, SPHERE_DIV_NUM, _joint_color, _joint_color, TRUE);
		DrawSphere3D(_femur, JOINT_R, SPHERE_DIV_NUM, _joint_color, _joint_color, TRUE);
		DrawSphere3D(_tibia, JOINT_R, SPHERE_DIV_NUM, _joint_color, COLOR_LIFTED_JOINT, TRUE);

		//ãrÇÃÉxÅ[ÉXç¿ïWÇÃï`âÊ
		if (_node.leg_pos[i] != _node.leg_base_pos[i])
		{
			DrawSphere3D(_leg_base, JOINT_R, SPHERE_DIV_NUM, COLOR_LEG_BASE, COLOR_LEG_BASE, TRUE);
		}

		//ÉGÉâÅ[èoóÕ
		if (isAbleCoxaLeg(m_HexaCalc.getGlobalCoxaJointPos(_node, i, true), m_HexaCalc.getGlobalFemurJointPos(_node, i)) == false)
		{
			DrawString((int)ConvWorldPosToScreenPos(_coxa).x, (int)ConvWorldPosToScreenPos(_coxa).y, "Error:Coxa", GetColor(255, 64, 64));
		}

		if (isAbleFemurLeg(m_HexaCalc.getGlobalFemurJointPos(_node, i), m_HexaCalc.getGlobalTibiaJointPos(_node, i)) == false)
		{
			DrawString((int)ConvWorldPosToScreenPos(_femur).x, (int)ConvWorldPosToScreenPos(_femur).y, "Error:Femur", GetColor(64, 255, 64));
		}

		if (isAbleTibiaLeg(m_HexaCalc.getGlobalTibiaJointPos(_node, i), m_HexaCalc.getGlobalLegPos(_node, i, true)) == false)
		{
			DrawString((int)ConvWorldPosToScreenPos(_femur).x, (int)ConvWorldPosToScreenPos(_femur).y, "Error:Tibia", GetColor(64, 64, 255));
		}

		if (DO_OUTPUT_DEBUG_LOG)
		{
			if (m_HexaCalc.isLegInRange(_node, i))printfDx("ÅZ");
			else printfDx("Å~");

			printfDx(" LegNum: %d \t", i);
			printfDx("Max : %.3f, min : %.3f\t", m_HexaCalc.getMaxLegR(abs(_node.leg_pos[i].z)), m_HexaCalc.getMinLegR(abs(_node.leg_pos[i].z)));
			printfDx("%.3f\t", _node.leg_pos[i].length());

			if (m_HexaCalc.isLegInRange(_node, i))printfDx("is in range   ");
			else printfDx("isnot in range");

			if (_node.leg_base_pos[i].projectedXY().cross(_node.leg_pos[i].projectedXY()) * _node.leg_pos[i].projectedXY().cross({ 1,0 }) > 0)
			{
				printfDx("front - 567\n");
			}
			else
			{
				printfDx("back - 123\n");
			}
		}
	}

	if (DO_OUTPUT_DEBUG_LOG)
	{
		//is Able Pause
		if (m_HexaCalc.isAblePause(_node)) { printfDx("ÅZ is Able Pause\n"); }
		else { printfDx("Å~ isnot Able Pause\n"); }

		// leg Interfering
		if (m_HexaCalc.isLegInterfering(_node)) { printfDx("Å~ is Leg Interfering\n"); }
		else { printfDx("ÅZ isnot Leg Interfering\n"); }
	}
}
