#include "camera_manager.h"

#include <cmath>

#include <Dxlib.h>

#include "designlab_math_util.h"
#include "graphic_const.h"
#include "dxlib_util.h"

namespace dlm = designlab::math_util;


CameraStateManager::CameraStateManager() :
	m_camera_view_mode(CameraViewMode::kTopView),
	m_goal_target_pos(VGet(0, 0, 0)), m_goal_length_camera_to_target(GraphicConst::kCameraToTargetMax),
	m_target_pos(VGet(0, 0, 0)), m_length_camera_to_target(GraphicConst::kCameraToTargetMax)
{
	setCameraViewMode(CameraViewMode::kTopView);	//カメラの初期位置をセットする．

	setCameraPosAndRot();		//カメラ位置をセットする．

	initCaneraTargetLength();	//カメラの距離を初期化する．
}


void CameraStateManager::Update()
{
	m_length_camera_to_target = approachTargetValue(m_length_camera_to_target, m_goal_length_camera_to_target);	//カメラの距離を目標値に近づける．
	m_camera_rot_quat = approachTargetQuat(m_camera_rot_quat, m_goal_camera_rot_quat);							//カメラの回転を目標値に近づける．

	//カメラの注視点を目標値に近づける．
	if (m_camera_view_mode != CameraViewMode::FREE_CONTROLLED_TARGET)
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


void CameraStateManager::setCameraViewMode(const CameraViewMode mode)
{
	m_camera_view_mode = mode;	//一応カメラのモードをメンバで持っているが，今のところ使っていない．必要ないかも

	designlab::SQuaternion quat1{0, 0, 0, 0}, quat2{ 0,0,0,0 };	//switch文の中で宣言するとエラーが出るので，前方宣言する．


	switch (mode)
	{
	case CameraViewMode::kFrontView:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dlm::ConvertDegToRad(0.0f), designlab::Vector3{ 0, 0, 1 });
		break;

	case CameraViewMode::kBackView:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dlm::ConvertDegToRad(180.0f), designlab::Vector3{ 0, 0, 1 });
		break;

	case CameraViewMode::kTopView:
		quat1.setRotAngleAndAxis(dlm::ConvertDegToRad(-90.0f), designlab::Vector3{ 0, 1, 0 });
		quat2.setRotAngleAndAxis(dlm::ConvertDegToRad(180.0f), designlab::Vector3{ 1, 0, 0 });
		m_goal_camera_rot_quat = (quat1 * quat2).normalize();
		break;

	case CameraViewMode::kRightSideView:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dlm::ConvertDegToRad(270.0f), designlab::Vector3{ 0, 0, 1 });
		break;

	case CameraViewMode::kLeftSideView:
		m_goal_camera_rot_quat.setRotAngleAndAxis(dlm::ConvertDegToRad(90.0f), designlab::Vector3{ 0, 0, 1 });
		break;

	case CameraViewMode::FREE_CONTROLLED_TARGET:
		m_free_controlled_target = m_goal_target_pos;
		break;

	default:
		break;
	}
}


void CameraStateManager::initCaneraTargetLength()
{
	//最大と最小の中間値を初期値とする．
	m_goal_length_camera_to_target = (GraphicConst::kCameraToTargetMin + GraphicConst::kCameraToTargetMax) * 0.5f;
}


void CameraStateManager::addCameraToTargetLength(const float length_dif)
{
	m_goal_length_camera_to_target += length_dif;

	if (GraphicConst::kCameraToTargetMax < m_goal_length_camera_to_target) { m_goal_length_camera_to_target = GraphicConst::kCameraToTargetMax; }

	if (m_goal_length_camera_to_target < GraphicConst::kCameraToTargetMin) { m_goal_length_camera_to_target = GraphicConst::kCameraToTargetMin; }
}


void CameraStateManager::setCameraPosAndRot()
{
	//カメラの位置をセットする．クォータニオンを用いて回転させ，dl_vec::vectorからdxlib::VECTORに変換する．
	namespace dldu = designlab::dxlib_util;

	VECTOR camera_target_dif = dldu::ConvertToDxlibVec(designlab::rotVecByQuat(kDefaultCameraFrontVec, m_camera_rot_quat) * m_length_camera_to_target);
	VECTOR camera_pos = VAdd(camera_target_dif, m_target_pos);

	VECTOR camera_upvec = dldu::ConvertToDxlibVec(designlab::rotVecByQuat(kDefaultCameraUpVec, m_camera_rot_quat));

	SetCameraPositionAndTargetAndUpVec(camera_pos, m_target_pos, camera_upvec);
}


designlab::SQuaternion CameraStateManager::approachTargetQuat(const designlab::SQuaternion& current, const designlab::SQuaternion& target) const
{
	const float dif = 0.2f;
	return ((1 - dif) * current + dif * target).normalize();
}


VECTOR CameraStateManager::approachTargetVECTOR(const VECTOR& current, const VECTOR& target) const
{
	return VGet(approachTargetValue(current.x, target.x), approachTargetValue(current.y, target.y), approachTargetValue(current.z, target.z));
}


float CameraStateManager::approachTargetValue(const float current, const float target) const
{
	const float dif = 0.2f;
	return (1 - dif) * current + dif * target;
}
