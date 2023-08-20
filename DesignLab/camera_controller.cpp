#include "camera_controller.h"

#include <cmath>

#include "DxLib.h"

#include "graphic_const.h"
#include "designlab_dxlib.h"
#include "Define.h"


CameraController::CameraController() :
	m_camera_view_mode(ECameraMode::TOP_VIEW),
	m_goal_target_pos(VGet(0, 0, 0)), m_goal_length_camera_to_target(GraphicConst::CAMERA_TO_TARGET_MAX), m_goal_upvec(VGet(0, 0, 0)), m_goal_camera_rot({ 0.0f, 0.0f, 0.0f }),
	m_target_pos(VGet(0, 0, 0)), m_length_camera_to_target(GraphicConst::CAMERA_TO_TARGET_MAX), m_camera_upvec(VGet(0, 0, 0)), m_camera_rot({ 0.0f, 0.0f, 0.0f })
{
	//カメラ位置をセットする．
	VECTOR camera_pos = VAdd(dl_dxlib::convertToDxVec(my_vec::rotVector(kDefaultCameraFrontVec, m_camera_rot) * m_length_camera_to_target), m_target_pos);

	SetCameraPositionAndTargetAndUpVec(camera_pos, m_target_pos, m_camera_upvec);
}


void CameraController::update()
{
	m_length_camera_to_target = approachTargetValue(m_length_camera_to_target, m_goal_length_camera_to_target);		//カメラの距離を目標値に近づける．
	m_target_pos = approachTargetVECTOR(m_target_pos, m_goal_target_pos);			//カメラの注視点を目標値に近づける．
	m_camera_upvec = approachSlowlyTargetVECTOR(m_camera_upvec, m_goal_upvec);		//カメラの上方向を目標値に近づける．
	m_camera_rot = approachTargetRotator(m_camera_rot, m_goal_camera_rot);			//カメラの回転角を目標値に近づける．

	VECTOR camera_pos = VAdd(dl_dxlib::convertToDxVec(my_vec::rotVector(kDefaultCameraFrontVec, m_camera_rot) * m_length_camera_to_target), m_target_pos);

	SetCameraPositionAndTargetAndUpVec(camera_pos, m_target_pos, m_camera_upvec);
}


void CameraController::setCameraViewMode(const ECameraMode mode)
{
	m_camera_view_mode = mode;

	switch (mode)
	{
	case ECameraMode::FRONT_VIEW:
		m_goal_upvec = VGet(0.0f, 0.0f, 1.0f);
		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
		m_goal_camera_rot = { 0.0f, 0.0f, my_math::convertDegToRad(180.0f) };
		break;

	case ECameraMode::BACK_VIEW:
		m_goal_upvec = VGet(0.0f, 0.0f, 1.0f);
		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
		m_goal_camera_rot = { 0.0f, 0.0f, 0.0f };
		break;

	case ECameraMode::TOP_VIEW:
		m_goal_upvec = VGet(1.0f, 0.0f, 0.0f);
		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
		m_goal_camera_rot = { 0.0f, my_math::convertDegToRad(-90.0f), 0.0f };
		break;

	case ECameraMode::RIGHT_SIDE_VIEW:
		m_goal_upvec = VGet(0.0f, 0.0f, 1.0f);
		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
		m_goal_camera_rot = { 0.0f, 0.0f, my_math::convertDegToRad(90.0f) };
		break;

	case ECameraMode::LEFT_SIDE_VIEW:
		m_goal_upvec = VGet(0.0f, 0.0f, 1.0f);
		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
		m_goal_camera_rot = { 0.0f, 0.0f, my_math::convertDegToRad(-90.0f) };
		break;

	default:
		break;
	}
}


void CameraController::addCameraToTargetLength(const float length_dif)
{
	m_camera_view_mode = ECameraMode::FREE_CONTROLLED;	//カメラを操作モードに変更

	m_goal_length_camera_to_target = length_dif;

	if (GraphicConst::CAMERA_TO_TARGET_MAX < m_goal_length_camera_to_target) { m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX; }

	if (m_goal_length_camera_to_target < GraphicConst::CAMERA_NEAR) { m_goal_length_camera_to_target = GraphicConst::CAMERA_NEAR; }
}



VECTOR CameraController::approachTargetVECTOR(const VECTOR& current, const VECTOR& target) const
{
	return VGet(approachTargetValue(current.x, target.x), approachTargetValue(current.y, target.y), approachTargetValue(current.z, target.z));
}


VECTOR CameraController::approachSlowlyTargetVECTOR(const VECTOR& current, const VECTOR& target) const
{
	return VGet(approachSlowlyTargetValue(current.x, target.x), approachSlowlyTargetValue(current.y, target.y), approachSlowlyTargetValue(current.z, target.z));
}

my_vec::SRotator CameraController::approachTargetRotator(const my_vec::SRotator& current, const my_vec::SRotator& target) const
{
	return my_vec::SRotator(approachSlowlyTargetValue(current.roll, target.roll), approachSlowlyTargetValue(current.pitch, target.pitch), approachSlowlyTargetValue(current.yaw, target.yaw));
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
