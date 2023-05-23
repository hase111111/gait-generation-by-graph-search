#include "CameraController.h"
#include "GraphicConst.h"
#include "DxLib.h"
#include <cmath>
#include "Define.h"

CameraController::CameraController() :
	m_target_pos(VGet(0, 0, 0)), m_camera_up(VGet(0.0f, 1.0f, 0.0f)), m_length_camera_to_target(GraphicConst::CAMERA_TO_TARGET_MAX), m_angle(0)
{
	SetCameraPositionAndTargetAndUpVec(VAdd(m_target_pos, VGet(m_length_camera_to_target * sin(m_angle), 0, m_length_camera_to_target * cos(m_angle))), m_target_pos, m_camera_up);
}

void CameraController::update()
{
}

void CameraController::setTargetPos(const VECTOR _pos)
{
	//新しい目標を設定
	m_target_pos = _pos; 

	//カメラの向きを更新する．
	SetCameraPositionAndTargetAndUpVec(VAdd(m_target_pos, VGet(m_length_camera_to_target * sin(m_angle), 0, m_length_camera_to_target * cos(m_angle))), m_target_pos, m_camera_up);
}
