#include "HexapodRenderer.h"
#include "DxLib.h"
#include "Dxlib3DFunction.h"
#include "LegState.h"


bool HexapodRenderer::isAbleCoxaLeg(const myvector::SVector _coxa_joint, const myvector::SVector _femur_joint) const
{
	if (abs((_coxa_joint - _femur_joint).length() - HexapodConst::COXA_LENGTH) < Define::ALLOWABLE_ERROR) { return true; }
	return false;
}

bool HexapodRenderer::isAbleFemurLeg(const myvector::SVector _femur_joint, const myvector::SVector _tibia_joint) const
{
	if (abs((_femur_joint - _tibia_joint).length() - HexapodConst::FEMUR_LENGTH) < Define::ALLOWABLE_ERROR) { return true; }
	return false;
}

bool HexapodRenderer::isAbleTibiaLeg(const myvector::SVector _tibia_joint, const myvector::SVector _leg_joint) const
{
	if (abs((_tibia_joint - _leg_joint).length() - HexapodConst::TIBIA_LENGTH) < 10) { return true; }
	return false;
}

HexapodRenderer::HexapodRenderer() :
	COLOR_BODY(GetColor(23, 58, 235)), COLOR_LEG(GetColor(23, 58, 235)), COLOR_LIFTED_LEG(GetColor(240, 30, 60)), 
	COLOR_JOINT(GetColor(100, 100, 200)), COLOR_LIFTED_JOINT(GetColor(200, 100, 100)), CAPSULE_DIV_NUM(6), SPHERE_DIV_NUM(16)
{
}

void HexapodRenderer::update(const SNode& _node)
{
	m_HexaCalc.calclateJointPos(_node);
}

void HexapodRenderer::draw(const SNode& _node) const
{
	//‹Œver‚ÌhexapodGraphic‚Ì‹Lq‚É‚µ‚½‚ª‚Á‚Ä•`‰æ‚·‚éD
	using namespace myDxlib3DFunc;
	using namespace LegState;

	if (DO_OUTPUT_DEBUG_LOG) { clsDx(); }	// printfDx()‚Ìo—Í‚ð‚«‚ê‚¢‚É‚·‚éD

	//“·‘Ì‚ð•`‰æ‚·‚éD
	VECTOR _vertex[6];
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//_vertex[i] = convertToDxVec(_hexapod.getGlobalCoxaJointPos(i));
		_vertex[i] = convertToDxVec( m_HexaCalc.getGlobalCoxaJointPos(_node, i) );
	}

	drawHexagonalPrism(_vertex, HexapodConst::BODY_HEIGHT, COLOR_BODY);

	//‹r‚ð•`‰æ‚·‚éD
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		const VECTOR _leg_end = convertToDxVec( m_HexaCalc.getGlobalLegPos(_node, i) );
		const VECTOR _coxa = convertToDxVec( m_HexaCalc.getGlobalCoxaJointPos(_node, i) );
		const VECTOR _femur = convertToDxVec( m_HexaCalc.getGlobalFemurJointPos(_node, i) );
		const VECTOR _tibia = convertToDxVec( m_HexaCalc.getGlobalTibiaJointPos(_node, i) );

		//‹r‚ÌF‚ð—V‹rEÚ’n‚Å•ÏX‚·‚éD
		const unsigned int _color = isGrounded(_node.leg_state, i) ? COLOR_LEG : COLOR_LIFTED_LEG;
		const unsigned int _joint_color = isGrounded(_node.leg_state, i) ? COLOR_JOINT : COLOR_LIFTED_JOINT;

		//Še‹r‚Ì•`‰æ
		DrawCapsule3D(_coxa,	_femur, LEG_R, CAPSULE_DIV_NUM, _color, _color, TRUE);	//coxa
		DrawCapsule3D(_femur,	_tibia, LEG_R, CAPSULE_DIV_NUM, _color, _color, TRUE);	//femur
		DrawCone3D(_leg_end,	_tibia, LEG_R, CAPSULE_DIV_NUM, _color, _color, TRUE);	//tibia 

		//ŠÔÚ‚Ì•`‰æ
		DrawSphere3D(_coxa,	 JOINT_R, SPHERE_DIV_NUM, _joint_color, _joint_color, TRUE);
		DrawSphere3D(_femur, JOINT_R, SPHERE_DIV_NUM, _joint_color, _joint_color, TRUE);
		DrawSphere3D(_tibia, JOINT_R, SPHERE_DIV_NUM, _joint_color, COLOR_LIFTED_JOINT, TRUE);

		if (isAbleCoxaLeg(m_HexaCalc.getGlobalCoxaJointPos(_node, i), m_HexaCalc.getGlobalFemurJointPos(_node, i)) == false) 
		{
			DrawString(ConvWorldPosToScreenPos(_coxa).x, ConvWorldPosToScreenPos(_coxa).y, "Error:Coxa", GetColor(255, 64, 64));
		}

		if (isAbleFemurLeg( m_HexaCalc.getGlobalFemurJointPos(_node, i), m_HexaCalc.getGlobalTibiaJointPos(_node, i)) == false)
		{
			DrawString(ConvWorldPosToScreenPos(_femur).x, ConvWorldPosToScreenPos(_femur).y, "Error:Femur", GetColor(64, 255, 64));
		}

		if (isAbleTibiaLeg(m_HexaCalc.getGlobalTibiaJointPos(_node, i), m_HexaCalc.getGlobalLegPos(_node, i)) == false)
		{
			DrawString(ConvWorldPosToScreenPos(_femur).x, ConvWorldPosToScreenPos(_femur).y, "Error:Tibia", GetColor(64, 64, 255));
		}

		if (DO_OUTPUT_DEBUG_LOG) 
		{
			printfDx("Max : %.3f, min : %.3f\t", m_HexaCalc.getMaxLegR(abs(_node.Leg[i].z)), m_HexaCalc.getMinLegR(abs(_node.Leg[i].z))); 
			printfDx("%.3f\n", _node.Leg[i].length());
		}
		
	}
}
