#include "camera_state_manager.h"

#include <cmath>

#include <Dxlib.h>

#include "cassert_define.h"
#include "designlab_math_util.h"
#include "dxlib_util.h"
#include "graphic_const.h"


namespace dl = designlab;
namespace dlm = designlab::math_util;
namespace dldu = designlab::dxlib_util;


CameraStateManager::CameraStateManager() :
	kDefaultCameraFrontVec{ dl::Vector3::GetFrontVec() },
	kDefaultCameraUpVec{ dl::Vector3::GetUpVec() },
	camera_view_mode_(CameraViewMode::kTopView),
	free_controlled_target_pos_{},
	now_camera_state_{},
	goal_camera_state_{}
{
	SetCameraViewMode(CameraViewMode::kTopView);	//カメラの初期位置をセットする．

	SetCameraPosAndRot();		//カメラ位置をセットする．

	InitCaneraTargetLength();	//カメラの距離を初期化する．

	now_camera_state_.camera_quat = dl::Quaternion::MakeByAngleAxis(dlm::ConvertDegToRad(-90.0f), dl::Vector3::GetLeftVec()) * 
		dl::Quaternion::MakeByAngleAxis(dlm::ConvertDegToRad(180.0f), dl::Vector3::GetFrontVec());
}


void CameraStateManager::Update()
{
	//カメラの距離を目標値に近づける．
	now_camera_state_.length_camera_to_target = dlm::ApproachTarget(
		now_camera_state_.length_camera_to_target, 
		goal_camera_state_.length_camera_to_target,
		0.1f
	);	

	//カメラの回転を目標値に近づける．
	now_camera_state_.camera_quat = dl::SlerpQuaternion(
		now_camera_state_.camera_quat, 
		goal_camera_state_.camera_quat,
		0.1f
	);

	now_camera_state_.camera_quat = now_camera_state_.camera_quat.GetNormalized();


	//カメラの注視点を目標値に近づける．
	if (camera_view_mode_ != CameraViewMode::kFreeControlledAndMovableTarget)
	{
		now_camera_state_.target_pos = dlm::ApproachTarget(
			now_camera_state_.target_pos, 
			goal_camera_state_.target_pos,
			0.1f
		);
	}
	else
	{
		now_camera_state_.target_pos = dlm::ApproachTarget(
			now_camera_state_.target_pos, 
			free_controlled_target_pos_,
			0.1f
		);
	}


	//カメラ位置をセットする．
	SetCameraPosAndRot();

	if (kOutputDebugLog) 
	{
		printfDx("cameraの姿勢(クォータニオン) w = %f, v= { %f, %f, %f }\n",
			now_camera_state_.camera_quat.w,
			now_camera_state_.camera_quat.v.x,
			now_camera_state_.camera_quat.v.y,
			now_camera_state_.camera_quat.v.z
		);

		printfDx("cameraの注視点 x = %f, y = %f, z = %f\n",
			now_camera_state_.target_pos.x,
			now_camera_state_.target_pos.y,
			now_camera_state_.target_pos.z
		);

		printfDx("cameraの距離 %f\n", now_camera_state_.length_camera_to_target);
	}
}


void CameraStateManager::SetCameraViewMode(const CameraViewMode mode)
{
	camera_view_mode_ = mode;

	// ゴール座標を更新する
	switch (mode)
	{
	case CameraViewMode::kFrontView:
	{
		goal_camera_state_.camera_quat =
			dl::Quaternion::MakeByAngleAxis(dlm::ConvertDegToRad(0.0f), dl::Vector3::GetUpVec());
		return;
	}

	case CameraViewMode::kBackView:
	{
		goal_camera_state_.camera_quat =
			dl::Quaternion::MakeByAngleAxis(dlm::ConvertDegToRad(180.0f), dl::Vector3::GetUpVec());
		return;
	}

	case CameraViewMode::kTopView:
	{
		dl::Quaternion quat1 = dl::Quaternion::MakeByAngleAxis(dlm::ConvertDegToRad(-90.0f), dl::Vector3::GetLeftVec());
		dl::Quaternion quat2 = dl::Quaternion::MakeByAngleAxis(dlm::ConvertDegToRad(180.0f), dl::Vector3::GetFrontVec());
		goal_camera_state_.camera_quat = quat1 * quat2;
		return;
	}

	case CameraViewMode::kRightSideView:
	{
		goal_camera_state_.camera_quat =
			dl::Quaternion::MakeByAngleAxis(dlm::ConvertDegToRad(-90.0f), dl::Vector3::GetUpVec());
		return;
	}

	case CameraViewMode::kLeftSideView:
	{
		goal_camera_state_.camera_quat =
			dl::Quaternion::MakeByAngleAxis(dlm::ConvertDegToRad(90.0f), dl::Vector3::GetUpVec());
		return;
	}

	case CameraViewMode::kFreeControlled:
	{
		return;
	}

	case CameraViewMode::kFreeControlledAndMovableTarget:
	{
		free_controlled_target_pos_ = goal_camera_state_.target_pos;
		return;
	}

	default:
		assert(false);	//ここに来ることはない．
		break;
	}
}


void CameraStateManager::InitCaneraTargetLength()
{
	//最大と最小の中間値を初期値とする．
	goal_camera_state_.length_camera_to_target = (GraphicConst::kCameraToTargetMin + GraphicConst::kCameraToTargetMax) * 0.5f;
}


void CameraStateManager::AddCameraToTargetLength(const float length_dif)
{
	goal_camera_state_.length_camera_to_target += length_dif;

	// カメラの距離が最大値を超えたら最大値にする．
	if (GraphicConst::kCameraToTargetMax < goal_camera_state_.length_camera_to_target)
	{
		goal_camera_state_.length_camera_to_target = GraphicConst::kCameraToTargetMax;
	}

	// カメラの距離が最小値を下回ったら最小値にする．
	if (goal_camera_state_.length_camera_to_target < GraphicConst::kCameraToTargetMin)
	{
		goal_camera_state_.length_camera_to_target = GraphicConst::kCameraToTargetMin;
	}
}


void CameraStateManager::SetCameraPosAndRot()
{
	//カメラの位置をセットする．クォータニオンを用いて回転させ，dl_vec::vectorからdxlib::VECTORに変換する．

	dl::Vector3 camera_target_dif = dl::RotateVector3(kDefaultCameraFrontVec, now_camera_state_.camera_quat) * now_camera_state_.length_camera_to_target;
	VECTOR camera_pos = dldu::ConvertToDxlibVec(camera_target_dif + now_camera_state_.target_pos);

	VECTOR camera_upvec = dldu::ConvertToDxlibVec(dl::RotateVector3(kDefaultCameraUpVec, now_camera_state_.camera_quat));

	SetCameraPositionAndTargetAndUpVec(
		camera_pos, 
		dldu::ConvertToDxlibVec(now_camera_state_.target_pos), 
		camera_upvec
	);

	// ChangeLightTypeDirを使っているので，カメラ向きに合わせてライトの向きも変更する．
	VECTOR light_dir = dldu::ConvertToDxlibVec(-dl::RotateVector3(kDefaultCameraFrontVec, now_camera_state_.camera_quat));
	ChangeLightTypeDir(light_dir);
}