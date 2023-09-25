#include "camera_controller.h"

#include "mouse.h"
#include "dxlib_util.h"

namespace dl = designlab;
namespace dldu = designlab::dxlib_util;


CameraController::CameraController(CameraStateManager& camera_manager_ref) : 
	camera_manager_ref_(camera_manager_ref) 
{
}


void CameraController::Update()
{
	//ホイールが動いていたらカメラ距離を変更する
	if (Mouse::GetIns()->GetWheelRot() != 0)
	{
		camera_manager_ref_.AddCameraToTargetLength(kCameraZoomSpeed * Mouse::GetIns()->GetWheelRot() * -1);
	}


	if (Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_MIDDLE) > 0)
	{
		//ホイールクリックをしていたらカメラを回転させる．XとYのどちらかの移動量が大きい方を処理する

		if (abs(Mouse::GetIns()->GetDiffPosX()) > abs(Mouse::GetIns()->GetDiffPosY()))
		{
			//カメラの回転をマウスの横移動量に合わせて変更
			dl::Quaternion move_quatx = {0, 0, 0, 0};

			move_quatx = dl::Quaternion::MakeByRotAngleAndAxis(Mouse::GetIns()->GetDiffPosX() * kCameraMoveSpeed * -1, { 0,0,1 });

			dl::Quaternion res = camera_manager_ref_.GetCameraRotQuat() * move_quatx;

			res = res.Normalize();

			camera_manager_ref_.SetCameraRotQuat(res);
		}
		else
		{
			//カメラの回転をマウスの縦移動量に合わせて変更
			dl::Quaternion move_quaty{ 0, 0, 0, 0 };

			move_quaty = dl::Quaternion::MakeByRotAngleAndAxis(Mouse::GetIns()->GetDiffPosY() * kCameraMoveSpeed * -1, { 0,1,0 });

			dl::Quaternion res = camera_manager_ref_.GetCameraRotQuat() * move_quaty;

			res = res.Normalize();

			camera_manager_ref_.SetCameraRotQuat(res);
		}

	}
	else if (Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_LEFT) > 0)
	{

		//左クリックしていたらカメラのビュー視点の中心軸を回転軸とした回転

		dl::Quaternion move_quat = { 0, 0, 0, 0 };

		int mouse_move = (abs(Mouse::GetIns()->GetDiffPosX()) > abs(Mouse::GetIns()->GetDiffPosY())) ? Mouse::GetIns()->GetDiffPosX() : Mouse::GetIns()->GetDiffPosY();

		move_quat = dl::Quaternion::MakeByRotAngleAndAxis(mouse_move * kCameraMoveSpeed * -1.0f, { 1,0,0 });

		dl::Quaternion res = camera_manager_ref_.GetCameraRotQuat() * move_quat;

		res = res.Normalize();

		camera_manager_ref_.SetCameraRotQuat(res);

	}
	else if (Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_RIGHT) > 0 && Mouse::GetIns()->GetDiffPos() > kMouseMoveMargin)
	{

		//右クリックしていたらカメラの平行移動

		if (camera_manager_ref_.GetCameraViewMode() != CameraViewMode::kFreeControlledAndMovableTarget)
		{
			//表示モードを注視点を自由に動かせるモードに変更
			camera_manager_ref_.SetCameraViewMode(CameraViewMode::kFreeControlledAndMovableTarget);
		}


		dl::Vector3 move_vec;

		if (abs(Mouse::GetIns()->GetDiffPosX()) > abs(Mouse::GetIns()->GetDiffPosY()))
		{
			//Xの移動量が大きい場合は横移動量を移動量とする

			move_vec = { 0, Mouse::GetIns()->GetDiffPosX() * kCameraTargetMoveSpeed * -1, 0 };

			move_vec = dl::rotVecByQuat(move_vec, camera_manager_ref_.GetCameraRotQuat());
		}
		else
		{
			//Yの移動量が大きい場合は縦移動量を移動量とする

			move_vec = { 0, 0, Mouse::GetIns()->GetDiffPosY() * kCameraTargetMoveSpeed };

			move_vec = dl::rotVecByQuat(move_vec, camera_manager_ref_.GetCameraRotQuat());
		}

		dl::Vector3 now_target_pos = camera_manager_ref_.GetFreeTargetPos();	//現在のターゲット座標を取得

		now_target_pos = now_target_pos + move_vec;								//移動量を加算

		camera_manager_ref_.SetFreeTargetPos(now_target_pos);					//ターゲット座標を更新	

	}
}
