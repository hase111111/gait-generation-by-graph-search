#include "camera_controller.h"

#include <cmath>


#include "DxLib.h"

#include "graphic_const.h"
#include "Define.h"


CameraController::CameraController() : m_goal_upvec(VGet(0, 0, 0)), m_goal_pos(VGet(0, 0, 0))
{
	//メンバを初期化する．
	m_mode = ECameraMode::TOP_VIEW;

	setGoalCameraPos();

	m_now_target_pos = m_goal_target_pos = VGet(0, 0, 0);
	m_now_camera_pos = m_goal_pos;
	m_now_camera_upvec = m_goal_upvec;

	m_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;

	//カメラ位置をセットする．
	SetCameraPositionAndTargetAndUpVec(m_now_camera_pos, m_now_target_pos, m_now_camera_upvec);
}


void CameraController::update()
{
	//カメラの設置場所を更新する．
	setGoalCameraPos();

	m_now_target_pos = approachTargetVECTOR(m_now_target_pos, m_goal_target_pos);
	m_now_camera_pos = approachTargetVECTOR(m_now_camera_pos, m_goal_pos);
	m_now_camera_upvec = approachSlowlyTargetVECTOR(m_now_camera_upvec, m_goal_upvec);

	SetCameraPositionAndTargetAndUpVec(m_now_camera_pos, m_now_target_pos, m_now_camera_upvec);
}


void CameraController::setGoalCameraPos()
{
	switch (m_mode)
	{
	case ECameraMode::TOP_VIEW:
		m_goal_upvec = VGet(1.0f, 0.0f, 0.0f);
		m_goal_pos = VAdd(m_now_target_pos, VGet(0, 0, m_length_camera_to_target));
		break;

	case ECameraMode::OVERHEAD_VIEW:
		m_goal_upvec = VGet(0.0f, -1.0f * sin(CAMERA_ANGLE), 1.0f * cos(CAMERA_ANGLE));
		m_goal_pos = VAdd(m_now_target_pos, VGet(-1, m_length_camera_to_target * cos(CAMERA_ANGLE), m_length_camera_to_target * sin(CAMERA_ANGLE)));
		break;

	case ECameraMode::SIDE_VIEW:
		m_goal_upvec = VGet(0.0f, 0.0f, 1.0f);
		m_goal_pos = VAdd(m_now_target_pos, VGet(0, m_length_camera_to_target, 0));
		break;

	case ECameraMode::OVERHEAD_VIEW_FLIP:
		m_goal_upvec = VGet(0.0f, 1.0f * sin(CAMERA_ANGLE), 1.0f * cos(CAMERA_ANGLE));
		m_goal_pos = VAdd(m_now_target_pos, VGet(-1, m_length_camera_to_target * -1 * cos(CAMERA_ANGLE), m_length_camera_to_target * sin(CAMERA_ANGLE)));
		break;

	case ECameraMode::SIDE_VIEW_FLIP:
		m_goal_upvec = VGet(0.0f, 0.0f, 1.0f);
		m_goal_pos = VAdd(m_now_target_pos, VGet(0, -m_length_camera_to_target, 0));
		break;

	default:
		break;
	}
}


VECTOR CameraController::approachTargetVECTOR(const VECTOR& current, const VECTOR& target) const
{
	return VGet(approachTargetValue(current.x, target.x), approachTargetValue(current.y, target.y), approachTargetValue(current.z, target.z));
}


VECTOR CameraController::approachSlowlyTargetVECTOR(const VECTOR& current, const VECTOR& target) const
{
	return VGet(approachSlowlyTargetValue(current.x, target.x), approachSlowlyTargetValue(current.y, target.y), approachSlowlyTargetValue(current.z, target.z));
}


float CameraController::approachTargetValue(const float current, const float target) const
{
	if (abs(current - target) > CAMERA_HIGH_DIF)
	{
		if (current < target) { return current + CAMERA_HIGH_SPEED; }
		else { return current - CAMERA_HIGH_SPEED; }
	}

	if (abs(current - target) < CAMERA_SPEED)
	{
		return target;
	}
	else
	{
		if (current < target) { return current + CAMERA_SPEED; }
		else { return current - CAMERA_SPEED; }
	}
}


float CameraController::approachSlowlyTargetValue(const float current, const float target) const
{
	if (abs(current - target) < CAMERA_SLOW_SPEED)
	{
		return target;
	}
	else
	{
		if (current < target) { return current + CAMERA_SLOW_SPEED; }
		else { return current - CAMERA_SLOW_SPEED; }
	}
}
