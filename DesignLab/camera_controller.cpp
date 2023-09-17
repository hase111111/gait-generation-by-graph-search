#include "camera_controller.h"

#include "mouse.h"
#include "designlab_dxlib.h"


CameraController::CameraController(CameraManager& p_camera_manager) : camera_manager_ref_(p_camera_manager) {}


void CameraController::Update()
{
	//ホイールが動いていたらカメラ距離を変更する
	if (Mouse::GetIns()->wheel_rot() != 0)
	{
		camera_manager_ref_.addCameraToTargetLength(kCameraZoomSpeed * Mouse::GetIns()->wheel_rot() * -1);
	}



	if (Mouse::GetIns()->middle_pushing_counter() > 0)
	{

		//ホイールクリックをしていたらカメラを回転させる．XとYのどちらかの移動量が大きい方を処理する

		if (abs(Mouse::GetIns()->GetDiffPosX()) > abs(Mouse::GetIns()->GetDiffPosY()))
		{
			//カメラの回転をマウスの横移動量に合わせて変更
			dl_vec::SQuaternion move_quatx{0, 0, 0, 0};

			move_quatx.setRotAngleAndAxis(Mouse::GetIns()->GetDiffPosX() * kCameraMoveSpeed * -1, { 0,0,1 });

			dl_vec::SQuaternion res = camera_manager_ref_.getCameraRotQuat() * move_quatx;

			res = res.normalize();

			camera_manager_ref_.setCameraRotQuat(res);
		}
		else
		{
			//カメラの回転をマウスの縦移動量に合わせて変更
			dl_vec::SQuaternion move_quaty{ 0, 0, 0, 0 };

			move_quaty.setRotAngleAndAxis(Mouse::GetIns()->GetDiffPosY() * kCameraMoveSpeed * -1, { 0,1,0 });

			dl_vec::SQuaternion res = camera_manager_ref_.getCameraRotQuat() * move_quaty;

			res = res.normalize();

			camera_manager_ref_.setCameraRotQuat(res);
		}

	}
	else if (Mouse::GetIns()->left_pushing_counter() > 0)
	{

		//左クリックしていたらカメラのビュー視点の中心軸を回転軸とした回転

		dl_vec::SQuaternion move_quat{ 0, 0, 0, 0 };

		int mouse_move = (abs(Mouse::GetIns()->GetDiffPosX()) > abs(Mouse::GetIns()->GetDiffPosY())) ? Mouse::GetIns()->GetDiffPosX() : Mouse::GetIns()->GetDiffPosY();

		move_quat.setRotAngleAndAxis(mouse_move * kCameraMoveSpeed * -1.0f, { 1,0,0 });

		dl_vec::SQuaternion res = camera_manager_ref_.getCameraRotQuat() * move_quat;

		res = res.normalize();

		camera_manager_ref_.setCameraRotQuat(res);

	}
	else if (Mouse::GetIns()->right_pushing_counter() > 0 && Mouse::GetIns()->getDiffPos() > kMouseMoveMargin)
	{

		//右クリックしていたらカメラの平行移動

		if (camera_manager_ref_.getCameraViewMode() != ECameraMode::FREE_CONTROLLED_TARGET)
		{
			//表示モードを注視点を自由に動かせるモードに変更
			camera_manager_ref_.setCameraViewMode(ECameraMode::FREE_CONTROLLED_TARGET);
		}


		dl_vec::SVector move_vec;

		if (abs(Mouse::GetIns()->GetDiffPosX()) > abs(Mouse::GetIns()->GetDiffPosY()))
		{
			//Xの移動量が大きい場合は横移動量を移動量とする

			move_vec = { 0, Mouse::GetIns()->GetDiffPosX() * kCameraTargetMoveSpeed * -1, 0 };

			move_vec = dl_vec::rotVecByQuat(move_vec, camera_manager_ref_.getCameraRotQuat());
		}
		else
		{
			//Yの移動量が大きい場合は縦移動量を移動量とする

			move_vec = { 0, 0, Mouse::GetIns()->GetDiffPosY() * kCameraTargetMoveSpeed };

			move_vec = dl_vec::rotVecByQuat(move_vec, camera_manager_ref_.getCameraRotQuat());
		}


		VECTOR now_target_pos = camera_manager_ref_.getFreeTargetPos();				//現在のターゲット座標を取得

		now_target_pos = VAdd(now_target_pos, dl_dxlib::convertToDxVec(move_vec));	//移動量を加算

		camera_manager_ref_.setFreeTargetPos(now_target_pos);						//ターゲット座標を更新	

	}
}
