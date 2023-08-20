#include "camera_controller.h"

#include <cmath>

#include "DxLib.h"

#include "graphic_const.h"
#include "designlab_dxlib.h"
#include "Define.h"


CameraController::CameraController() :
	m_camera_view_mode(ECameraMode::TOP_VIEW),
	m_goal_target_pos(VGet(0, 0, 0)), m_goal_length_camera_to_target(GraphicConst::CAMERA_TO_TARGET_MAX),
	m_target_pos(VGet(0, 0, 0)), m_length_camera_to_target(GraphicConst::CAMERA_TO_TARGET_MAX)
{
	setCameraViewMode(ECameraMode::TOP_VIEW);

	//カメラ位置をセットする．
	VECTOR camera_target_dif = dl_dxlib::convertToDxVec(dl_vec::rotVecByQuat(kDefaultCameraFrontVec, m_camera_rot_quat) * m_length_camera_to_target);
	VECTOR camera_pos = VAdd(camera_target_dif, m_target_pos);
	VECTOR camera_upvec = dl_dxlib::convertToDxVec(dl_vec::rotVecByQuat(kDefaultCameraUpVec, m_camera_rot_quat));

	SetCameraPositionAndTargetAndUpVec(camera_pos, m_target_pos, camera_upvec);
}


void CameraController::update()
{
	m_length_camera_to_target = approachTargetValue(m_length_camera_to_target, m_goal_length_camera_to_target);		//カメラの距離を目標値に近づける．
	m_target_pos = approachTargetVECTOR(m_target_pos, m_goal_target_pos);			//カメラの注視点を目標値に近づける．
	m_camera_rot_quat = approachTargetQuat(m_camera_rot_quat, m_goal_camera_rot_quat);

	//カメラ位置をセットする．
	VECTOR camera_target_dif = dl_dxlib::convertToDxVec(dl_vec::rotVecByQuat(kDefaultCameraFrontVec, m_camera_rot_quat) * m_length_camera_to_target);
	VECTOR camera_pos = VAdd(camera_target_dif, m_target_pos);
	VECTOR camera_upvec = dl_dxlib::convertToDxVec(dl_vec::rotVecByQuat(kDefaultCameraUpVec, m_camera_rot_quat));

	SetCameraPositionAndTargetAndUpVec(camera_pos, m_target_pos, camera_upvec);
}


void CameraController::setCameraViewMode(const ECameraMode mode)
{
	m_camera_view_mode = mode;

	dl_vec::SQuaternion quat1{0, 0, 0, 0}, quat2{ 0,0,0,0 };

	switch (mode)
	{
	case ECameraMode::FRONT_VIEW:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dl_math::convertDegToRad(0.0f), dl_vec::SVector{ 0, 0, 1 });
		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
		break;

	case ECameraMode::BACK_VIEW:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dl_math::convertDegToRad(180.0f), dl_vec::SVector{ 0, 0, 1 });
		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
		break;

	case ECameraMode::TOP_VIEW:
		quat1.setRotAngleAndAxis(dl_math::convertDegToRad(-90.0f), dl_vec::SVector{ 0, 1, 0 });
		quat2.setRotAngleAndAxis(dl_math::convertDegToRad(180.0f), dl_vec::SVector{ 1, 0, 0 });
		m_goal_camera_rot_quat = quat1 * quat2;

		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
		break;

	case ECameraMode::RIGHT_SIDE_VIEW:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dl_math::convertDegToRad(-90.0f), dl_vec::SVector{ 0, 0, 1 });
		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
		break;

	case ECameraMode::LEFT_SIDE_VIEW:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dl_math::convertDegToRad(90.0f), dl_vec::SVector{ 0, 0, 1 });
		m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX;
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


dl_vec::SQuaternion CameraController::approachTargetQuat(const dl_vec::SQuaternion& current, const dl_vec::SQuaternion& target) const
{
	const float dif = 0.2f;
	return (1 - dif) * current + dif * target;
}


VECTOR CameraController::approachTargetVECTOR(const VECTOR& current, const VECTOR& target) const
{
	return VGet(approachTargetValue(current.x, target.x), approachTargetValue(current.y, target.y), approachTargetValue(current.z, target.z));
}


float CameraController::approachTargetValue(const float current, const float target) const
{
	const float dif = 0.2f;
	return (1 - dif) * current + dif * target;
}
