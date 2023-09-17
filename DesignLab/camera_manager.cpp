#include "camera_manager.h"

#include <cmath>

#include "DxLib.h"

#include "graphic_const.h"
#include "designlab_dxlib.h"


CameraManager::CameraManager() :
	m_camera_view_mode(ECameraMode::TOP_VIEW),
	m_goal_target_pos(VGet(0, 0, 0)), m_goal_length_camera_to_target(GraphicConst::CAMERA_TO_TARGET_MAX),
	m_target_pos(VGet(0, 0, 0)), m_length_camera_to_target(GraphicConst::CAMERA_TO_TARGET_MAX)
{
	setCameraViewMode(ECameraMode::TOP_VIEW);	//カメラの初期位置をセットする．

	setCameraPosAndRot();		//カメラ位置をセットする．

	initCaneraTargetLength();	//カメラの距離を初期化する．
}


void CameraManager::Update()
{
	m_length_camera_to_target = approachTargetValue(m_length_camera_to_target, m_goal_length_camera_to_target);	//カメラの距離を目標値に近づける．
	m_camera_rot_quat = approachTargetQuat(m_camera_rot_quat, m_goal_camera_rot_quat);							//カメラの回転を目標値に近づける．

	//カメラの注視点を目標値に近づける．
	if (m_camera_view_mode != ECameraMode::FREE_CONTROLLED_TARGET)
	{
		m_target_pos = approachTargetVECTOR(m_target_pos, m_goal_target_pos);
	}
	else
	{
		m_target_pos = approachTargetVECTOR(m_target_pos, m_free_controlled_target);
	}


	//カメラ位置をセットする．
	setCameraPosAndRot();
}


void CameraManager::setCameraViewMode(const ECameraMode mode)
{
	m_camera_view_mode = mode;	//一応カメラのモードをメンバで持っているが，今のところ使っていない．必要ないかも

	dl_vec::SQuaternion quat1{0, 0, 0, 0}, quat2{ 0,0,0,0 };	//switch文の中で宣言するとエラーが出るので，前方宣言する．


	switch (mode)
	{
	case ECameraMode::FRONT_VIEW:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dl_math::convertDegToRad(0.0f), dl_vec::SVector{ 0, 0, 1 });
		break;

	case ECameraMode::BACK_VIEW:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dl_math::convertDegToRad(180.0f), dl_vec::SVector{ 0, 0, 1 });
		break;

	case ECameraMode::TOP_VIEW:
		quat1.setRotAngleAndAxis(dl_math::convertDegToRad(-90.0f), dl_vec::SVector{ 0, 1, 0 });
		quat2.setRotAngleAndAxis(dl_math::convertDegToRad(180.0f), dl_vec::SVector{ 1, 0, 0 });
		m_goal_camera_rot_quat = (quat1 * quat2).normalize();
		break;

	case ECameraMode::RIGHT_SIDE_VIEW:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dl_math::convertDegToRad(270.0f), dl_vec::SVector{ 0, 0, 1 });
		break;

	case ECameraMode::LEFT_SIDE_VIEW:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dl_math::convertDegToRad(90.0f), dl_vec::SVector{ 0, 0, 1 });
		break;

	case ECameraMode::FREE_CONTROLLED_TARGET:
		m_free_controlled_target = m_goal_target_pos;
		break;

	default:
		break;
	}
}


void CameraManager::initCaneraTargetLength()
{
	//最大と最小の中間値を初期値とする．
	m_goal_length_camera_to_target = (GraphicConst::CAMERA_TO_TARGET_MIN + GraphicConst::CAMERA_TO_TARGET_MAX) * 0.5f;
}


void CameraManager::addCameraToTargetLength(const float length_dif)
{
	m_goal_length_camera_to_target += length_dif;

	if (GraphicConst::CAMERA_TO_TARGET_MAX < m_goal_length_camera_to_target) { m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MAX; }

	if (m_goal_length_camera_to_target < GraphicConst::CAMERA_TO_TARGET_MIN) { m_goal_length_camera_to_target = GraphicConst::CAMERA_TO_TARGET_MIN; }
}


void CameraManager::setCameraPosAndRot()
{
	//カメラの位置をセットする．クォータニオンを用いて回転させ，dl_vec::vectorからdxlib::VECTORに変換する．

	VECTOR camera_target_dif = dl_dxlib::convertToDxVec(dl_vec::rotVecByQuat(kDefaultCameraFrontVec, m_camera_rot_quat) * m_length_camera_to_target);
	VECTOR camera_pos = VAdd(camera_target_dif, m_target_pos);

	VECTOR camera_upvec = dl_dxlib::convertToDxVec(dl_vec::rotVecByQuat(kDefaultCameraUpVec, m_camera_rot_quat));

	SetCameraPositionAndTargetAndUpVec(camera_pos, m_target_pos, camera_upvec);
}


dl_vec::SQuaternion CameraManager::approachTargetQuat(const dl_vec::SQuaternion& current, const dl_vec::SQuaternion& target) const
{
	const float dif = 0.2f;
	return ((1 - dif) * current + dif * target).normalize();
}


VECTOR CameraManager::approachTargetVECTOR(const VECTOR& current, const VECTOR& target) const
{
	return VGet(approachTargetValue(current.x, target.x), approachTargetValue(current.y, target.y), approachTargetValue(current.z, target.z));
}


float CameraManager::approachTargetValue(const float current, const float target) const
{
	const float dif = 0.2f;
	return (1 - dif) * current + dif * target;
}
