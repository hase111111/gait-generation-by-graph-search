#include "camera_dragger.h"

#include "cassert_define.h"
#include "dxlib_util.h"


namespace dl = ::designlab;
namespace dlm = ::designlab::math_util;
namespace dldu = ::designlab::dxlib_util;


CameraDragger::CameraDragger(const std::shared_ptr<DxlibCamera> camera) : camera_ptr_(camera)
{
	assert(camera_ptr_ != nullptr);
}

void CameraDragger::DraggedAction(const int cursor_dif_x, const int cursor_dif_y, const unsigned int mouse_key_bit)
{
	assert(camera_ptr_ != nullptr);

	// カーソルが動いていたら，カメラの回転を変更する．
	if (mouse_key_bit & MOUSE_INPUT_MIDDLE)
	{
		//ホイールクリックをしていたらカメラを回転させる．XとYのどちらかの移動量が大きい方を処理する．

		if (abs(cursor_dif_x) > 0)
		{
			//カメラの回転をマウスの横移動量に合わせて変更．
			dl::Quaternion move_quatx = { 0, 0, 0, 0 };

			move_quatx = dl::Quaternion::MakeByAngleAxis(cursor_dif_x * kCameraMoveSpeed * -1, { 0,0,1 });

			dl::Quaternion res = camera_ptr_->GetCameraQuat() * move_quatx;

			res = res.GetNormalized();

			camera_ptr_->SetCameraQuat(res);
		}

		if (abs(cursor_dif_y) > 0)
		{
			//カメラの回転をマウスの縦移動量に合わせて変更．
			dl::Quaternion move_quaty = dl::Quaternion::MakeByAngleAxis(cursor_dif_y * kCameraMoveSpeed * -1, { 0,1,0 });

			dl::Quaternion res = camera_ptr_->GetCameraQuat() * move_quaty;

			res = res.GetNormalized();

			camera_ptr_->SetCameraQuat(res);
		}

	}
	else if (mouse_key_bit & MOUSE_INPUT_LEFT)
	{
		//左クリックしていたらカメラのビュー視点の中心軸を回転軸とした回転．
		const int mouse_move = abs(cursor_dif_x) > abs(cursor_dif_y) ? cursor_dif_x : cursor_dif_y;

		if (mouse_move != 0)
		{
			const dl::Quaternion move_quat = dl::Quaternion::MakeByAngleAxis(mouse_move * kCameraMoveSpeed * -1.0f, { 1,0,0 });

			const dl::Quaternion res = (camera_ptr_->GetCameraQuat() * move_quat).GetNormalized();

			camera_ptr_->SetCameraQuat(res);
		}
	}
	else if (mouse_key_bit & MOUSE_INPUT_RIGHT && dlm::Squared(cursor_dif_x) + dlm::Squared(cursor_dif_y) > dlm::Squared(kMouseMoveMargin))
	{
		//右クリックしていたらカメラの平行移動
		if (camera_ptr_->GetCameraViewMode() != CameraViewMode::kFreeControlledAndMovableTarget)
		{
			//表示モードを注視点を自由に動かせるモードに変更
			camera_ptr_->SetCameraViewMode(CameraViewMode::kFreeControlledAndMovableTarget);
		}


		dl::Vector3 move_vec_x = { 0,0,0 };

		if (abs(cursor_dif_x) > 0)
		{
			//_Xの移動量が大きい場合は横移動量を移動量とする

			move_vec_x = { 0, cursor_dif_x * kCameraTargetMoveSpeed * -1, 0 };

			move_vec_x = dl::RotateVector3(move_vec_x, camera_ptr_->GetCameraQuat());
		}

		dl::Vector3 move_vec_y = { 0,0,0 };

		if (abs(cursor_dif_y) > 0)
		{
			//Yの移動量が大きい場合は縦移動量を移動量とする

			move_vec_y = { 0, 0, cursor_dif_y * kCameraTargetMoveSpeed };

			move_vec_y = dl::RotateVector3(move_vec_y, camera_ptr_->GetCameraQuat());
		}

		dl::Vector3 now_target_pos = camera_ptr_->GetFreeTargetPos();	//現在のターゲット座標を取得

		now_target_pos = now_target_pos + move_vec_x + move_vec_y;			//移動量を加算

		camera_ptr_->SetFreeTargetPos(now_target_pos);					//ターゲット座標を更新	
	}
}

void CameraDragger::RotMouseWheel(const int rot) const
{
	camera_ptr_->AddCameraToTargetLength(kCameraZoomSpeed * rot * -1);
}