#include "camera_input_controller.h"

#include "cassert_define.h"
#include "mouse.h"
#include "dxlib_util.h"


namespace dl = designlab;
namespace dldu = designlab::dxlib_util;


CameraInputController::CameraInputController() : 
	kCameraZoomSpeed(50.0f),
	kCameraMoveSpeed(0.007f),
	kCameraTargetMoveSpeed(3.0f),
	kMouseMoveMargin(0.5)
{
}


void CameraInputController::ChangeCameraState(CameraStateManager* camera_manager)
{
	assert(camera_manager != nullptr);

	//ホイールが動いていたらカメラ距離を変更する
	if (Mouse::GetIns()->GetWheelRot() != 0)
	{
		camera_manager->AddCameraToTargetLength(kCameraZoomSpeed * Mouse::GetIns()->GetWheelRot() * -1);
	}

	// カーソルが動いていたら，カメラの回転を変更する
	if (Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_MIDDLE) > 0)
	{
		//ホイールクリックをしていたらカメラを回転させる．XとYのどちらかの移動量が大きい方を処理する

		if (abs(Mouse::GetIns()->GetDiffPosX()) > 0)
		{
			//カメラの回転をマウスの横移動量に合わせて変更
			dl::Quaternion move_quatx = {0, 0, 0, 0};

			move_quatx = dl::Quaternion::MakeByAngleAxis(Mouse::GetIns()->GetDiffPosX() * kCameraMoveSpeed * -1, { 0,0,1 });

			dl::Quaternion res = camera_manager->GetCameraQuat() * move_quatx;

			res = res.GetNormalized();

			camera_manager->SetCameraQuat(res);
		}

		if (abs(Mouse::GetIns()->GetDiffPosY()) > 0)
		{
			//カメラの回転をマウスの縦移動量に合わせて変更
			dl::Quaternion move_quaty = dl::Quaternion::MakeByAngleAxis(Mouse::GetIns()->GetDiffPosY() * kCameraMoveSpeed * -1, { 0,1,0 });

			dl::Quaternion res = camera_manager->GetCameraQuat() * move_quaty;

			res = res.GetNormalized();

			camera_manager->SetCameraQuat(res);
		}

	}
	else if (Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_LEFT) > 0)
	{
		//左クリックしていたらカメラのビュー視点の中心軸を回転軸とした回転

		int mouse_move = (abs(Mouse::GetIns()->GetDiffPosX()) > abs(Mouse::GetIns()->GetDiffPosY())) ? Mouse::GetIns()->GetDiffPosX() : Mouse::GetIns()->GetDiffPosY();

		dl::Quaternion move_quat = dl::Quaternion::MakeByAngleAxis(mouse_move * kCameraMoveSpeed * -1.0f, { 1,0,0 });

		dl::Quaternion res = camera_manager->GetCameraQuat() * move_quat;

		res = res.GetNormalized();

		camera_manager->SetCameraQuat(res);

	}
	else if (Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_RIGHT) > 0 && Mouse::GetIns()->GetDiffPos() > kMouseMoveMargin)
	{

		//右クリックしていたらカメラの平行移動

		if (camera_manager->GetCameraViewMode() != CameraViewMode::kFreeControlledAndMovableTarget)
		{
			//表示モードを注視点を自由に動かせるモードに変更
			camera_manager->SetCameraViewMode(CameraViewMode::kFreeControlledAndMovableTarget);
		}


		dl::Vector3 move_vec_x;

		if (abs(Mouse::GetIns()->GetDiffPosX()) > 0)
		{
			//Xの移動量が大きい場合は横移動量を移動量とする

			move_vec_x = { 0, Mouse::GetIns()->GetDiffPosX() * kCameraTargetMoveSpeed * -1, 0 };

			move_vec_x = dl::RotateVector3(move_vec_x, camera_manager->GetCameraQuat(), true);
		}

		dl::Vector3 move_vec_y;

		if (abs(Mouse::GetIns()->GetDiffPosY()) > 0)
		{
			//Yの移動量が大きい場合は縦移動量を移動量とする

			move_vec_y = { 0, 0, Mouse::GetIns()->GetDiffPosY() * kCameraTargetMoveSpeed };

			move_vec_y = dl::RotateVector3(move_vec_y, camera_manager->GetCameraQuat(), true);
		}

		dl::Vector3 now_target_pos = camera_manager->GetFreeTargetPos();	//現在のターゲット座標を取得

		now_target_pos = now_target_pos + move_vec_x + move_vec_y;			//移動量を加算

		camera_manager->SetFreeTargetPos(now_target_pos);					//ターゲット座標を更新	
	}
}