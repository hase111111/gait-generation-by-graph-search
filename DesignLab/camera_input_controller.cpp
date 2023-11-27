#include "camera_input_controller.h"

#include "cassert_define.h"
#include "dxlib_util.h"


namespace dl = designlab;
namespace dldu = designlab::dxlib_util;


CameraInputController::CameraInputController(const std::shared_ptr<DxlibCamera> camera) : camera_ptr_(camera)
{
	assert(camera_ptr_ != nullptr);
}


void CameraInputController::Activate(const std::shared_ptr<const Mouse> mouse_ptr)
{
	assert(camera_ptr_ != nullptr);

	//ホイールが動いていたらカメラ距離を変更する
	if (mouse_ptr->GetWheelRot() != 0)
	{
		camera_ptr_->AddCameraToTargetLength(kCameraZoomSpeed * mouse_ptr->GetWheelRot() * -1);
	}

	// カーソルが動いていたら，カメラの回転を変更する
	if (mouse_ptr->GetPressingCount(MOUSE_INPUT_MIDDLE) > 0)
	{
		//ホイールクリックをしていたらカメラを回転させる．XとYのどちらかの移動量が大きい方を処理する

		if (abs(mouse_ptr->GetDiffPosX()) > 0)
		{
			//カメラの回転をマウスの横移動量に合わせて変更
			dl::Quaternion move_quatx = {0, 0, 0, 0};

			move_quatx = dl::Quaternion::MakeByAngleAxis(mouse_ptr->GetDiffPosX() * kCameraMoveSpeed * -1, { 0,0,1 });

			dl::Quaternion res = camera_ptr_->GetCameraQuat() * move_quatx;

			res = res.GetNormalized();

			camera_ptr_->SetCameraQuat(res);
		}

		if (abs(mouse_ptr->GetDiffPosY()) > 0)
		{
			//カメラの回転をマウスの縦移動量に合わせて変更
			dl::Quaternion move_quaty = dl::Quaternion::MakeByAngleAxis(mouse_ptr->GetDiffPosY() * kCameraMoveSpeed * -1, { 0,1,0 });

			dl::Quaternion res = camera_ptr_->GetCameraQuat() * move_quaty;

			res = res.GetNormalized();

			camera_ptr_->SetCameraQuat(res);
		}

	}
	else if (mouse_ptr->GetPressingCount(MOUSE_INPUT_LEFT) > 0)
	{
		//左クリックしていたらカメラのビュー視点の中心軸を回転軸とした回転

		const int mouse_move = abs(mouse_ptr->GetDiffPosX()) > abs(mouse_ptr->GetDiffPosY()) ?
			mouse_ptr->GetDiffPosX() : mouse_ptr->GetDiffPosY();

		if (mouse_move != 0) 
		{
			const dl::Quaternion move_quat = dl::Quaternion::MakeByAngleAxis(mouse_move * kCameraMoveSpeed * -1.0f, { 1,0,0 });

			const dl::Quaternion res = (camera_ptr_->GetCameraQuat() * move_quat).GetNormalized();

			camera_ptr_->SetCameraQuat(res);
		}
	}
	else if (mouse_ptr->GetPressingCount(MOUSE_INPUT_RIGHT) > 0 && mouse_ptr->GetDiffPos() > kMouseMoveMargin)
	{
		//右クリックしていたらカメラの平行移動

		if (camera_ptr_->GetCameraViewMode() != CameraViewMode::kFreeControlledAndMovableTarget)
		{
			//表示モードを注視点を自由に動かせるモードに変更
			camera_ptr_->SetCameraViewMode(CameraViewMode::kFreeControlledAndMovableTarget);
		}


		dl::Vector3 move_vec_x;

		if (abs(mouse_ptr->GetDiffPosX()) > 0)
		{
			//_Xの移動量が大きい場合は横移動量を移動量とする

			move_vec_x = { 0, mouse_ptr->GetDiffPosX() * kCameraTargetMoveSpeed * -1, 0 };

			move_vec_x = dl::RotateVector3(move_vec_x, camera_ptr_->GetCameraQuat());
		}

		dl::Vector3 move_vec_y;

		if (abs(mouse_ptr->GetDiffPosY()) > 0)
		{
			//Yの移動量が大きい場合は縦移動量を移動量とする

			move_vec_y = { 0, 0, mouse_ptr->GetDiffPosY() * kCameraTargetMoveSpeed };

			move_vec_y = dl::RotateVector3(move_vec_y, camera_ptr_->GetCameraQuat());
		}

		dl::Vector3 now_target_pos = camera_ptr_->GetFreeTargetPos();	//現在のターゲット座標を取得

		now_target_pos = now_target_pos + move_vec_x + move_vec_y;			//移動量を加算

		camera_ptr_->SetFreeTargetPos(now_target_pos);					//ターゲット座標を更新	
	}
}