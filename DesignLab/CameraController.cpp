#include "CameraController.h"
#include "GraphicConst.h"
#include "DxLib.h"
#include <cmath>
#include "Define.h"

CameraController::CameraController() : m_goal_upvec(VGet(0, 0, 0)), m_goal_pos(VGet(0, 0, 0))
{
	//メンバを初期化する．
	m_mode = ECameraMode::TopView;

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

	m_now_target_pos	= approachTargetVECTOR(m_now_target_pos, m_goal_target_pos);
	m_now_camera_pos	= approachTargetVECTOR(m_now_camera_pos, m_goal_pos);
	m_now_camera_upvec = approachSlowlyTargetVECTOR(m_now_camera_upvec, m_goal_upvec);

	SetCameraPositionAndTargetAndUpVec(m_now_camera_pos, m_now_target_pos, m_now_camera_upvec);
}

void CameraController::setGoalCameraPos()
{
	switch (m_mode)
	{
	case ECameraMode::TopView:
		m_goal_upvec = VGet(0.0f, 1.0f, 0.0f);
		m_goal_pos = VAdd(m_now_target_pos, VGet(0, m_goal_upvec.z * m_length_camera_to_target * -1, m_goal_upvec.y * m_length_camera_to_target));
		break;

	case ECameraMode::OverheadView:
		m_goal_upvec = VGet(1.0f * sin(CAMERA_ANGLE), 0.0f, 1.0f * cos(CAMERA_ANGLE));
		m_goal_pos = VAdd(m_now_target_pos, VGet(m_length_camera_to_target * -1 * cos(CAMERA_ANGLE), 0, m_length_camera_to_target * sin(CAMERA_ANGLE)));
		break;

	case ECameraMode::SideView:
		m_goal_upvec = VGet(0.0f, 0.0f, 1.0f);
		m_goal_pos = VAdd(m_now_target_pos, VGet(m_goal_upvec.z * m_length_camera_to_target * -1, 0, 0));
		break;

	case ECameraMode::OverheadViewFlip:
		m_goal_upvec = VGet(-1.0f * sin(CAMERA_ANGLE), 0.0f, 1.0f * cos(CAMERA_ANGLE));
		m_goal_pos = VAdd(m_now_target_pos, VGet(m_length_camera_to_target * cos(CAMERA_ANGLE), 0, m_length_camera_to_target * sin(CAMERA_ANGLE)));
		break;

	case ECameraMode::SideViewFlip:
		m_goal_upvec = VGet(0.0f, 0.0f, 1.0f);
		m_goal_pos = VAdd(m_now_target_pos, VGet(m_goal_upvec.z * m_length_camera_to_target, 0, 0));
		break;

	default:
		break;
	}
}

VECTOR CameraController::approachTargetVECTOR(const VECTOR _current, const VECTOR _target) const
{
	return VGet(approachTargetValue(_current.x, _target.x), approachTargetValue(_current.y, _target.y), approachTargetValue(_current.z, _target.z));
}

VECTOR CameraController::approachSlowlyTargetVECTOR(const VECTOR _current, const VECTOR _target) const
{
	return VGet(approachSlowlyTargetValue(_current.x, _target.x), approachSlowlyTargetValue(_current.y, _target.y), approachSlowlyTargetValue(_current.z, _target.z));
}

float CameraController::approachTargetValue(const float _current, const float _target) const
{
	if (abs(_current - _target) > CAMERA_HIGH_DIF)
	{
		if (_current < _target) { return _current + CAMERA_HIGH_SPEED; }
		else { return _current - CAMERA_HIGH_SPEED; }
	}

	if (abs(_current - _target) < CAMERA_SPEED)
	{
		return _target;
	}
	else
	{
		if (_current < _target) { return _current + CAMERA_SPEED; }
		else { return _current - CAMERA_SPEED; }
	}
}

float CameraController::approachSlowlyTargetValue(const float _current, const float _target) const
{
	if (abs(_current - _target) < CAMERA_SLOW_SPEED)
	{
		return _target;
	}
	else
	{
		if (_current < _target) { return _current + CAMERA_SLOW_SPEED; }
		else { return _current - CAMERA_SLOW_SPEED; }
	}
}
