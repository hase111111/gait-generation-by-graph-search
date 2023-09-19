#include "camera_controller.h"

#include "mouse.h"
#include "dxlib_util.h"


CameraController::CameraController(CameraStateManager& camera_manager_ref) : camera_manager_ref_(camera_manager_ref) {}


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
			designlab::SQuaternion move_quatx{0, 0, 0, 0};

			move_quatx.setRotAngleAndAxis(Mouse::GetIns()->GetDiffPosX() * kCameraMoveSpeed * -1, { 0,0,1 });

			designlab::SQuaternion res = camera_manager_ref_.getCameraRotQuat() * move_quatx;

			res = res.normalize();

			camera_manager_ref_.setCameraRotQuat(res);
		}
		else
		{
			//カメラの回転をマウスの縦移動量に合わせて変更
			designlab::SQuaternion move_quaty{ 0, 0, 0, 0 };

			move_quaty.setRotAngleAndAxis(Mouse::GetIns()->GetDiffPosY() * kCameraMoveSpeed * -1, { 0,1,0 });

			designlab::SQuaternion res = camera_manager_ref_.getCameraRotQuat() * move_quaty;

			res = res.normalize();

			camera_manager_ref_.setCameraRotQuat(res);
		}

	}
	else if (Mouse::GetIns()->left_pushing_counter() > 0)
	{

		//左クリックしていたらカメラのビュー視点の中心軸を回転軸とした回転

		designlab::SQuaternion move_quat{ 0, 0, 0, 0 };

		int mouse_move = (abs(Mouse::GetIns()->GetDiffPosX()) > abs(Mouse::GetIns()->GetDiffPosY())) ? Mouse::GetIns()->GetDiffPosX() : Mouse::GetIns()->GetDiffPosY();

		move_quat.setRotAngleAndAxis(mouse_move * kCameraMoveSpeed * -1.0f, { 1,0,0 });

		designlab::SQuaternion res = camera_manager_ref_.getCameraRotQuat() * move_quat;

		res = res.normalize();

		camera_manager_ref_.setCameraRotQuat(res);

	}
	else if (Mouse::GetIns()->right_pushing_counter() > 0 && Mouse::GetIns()->getDiffPos() > kMouseMoveMargin)
	{

		//右クリックしていたらカメラの平行移動

		if (camera_manager_ref_.getCameraViewMode() != CameraViewMode::FREE_CONTROLLED_TARGET)
		{
			//表示モードを注視点を自由に動かせるモードに変更
			camera_manager_ref_.setCameraViewMode(CameraViewMode::FREE_CONTROLLED_TARGET);
		}


		designlab::Vector3 move_vec;

		if (abs(Mouse::GetIns()->GetDiffPosX()) > abs(Mouse::GetIns()->GetDiffPosY()))
		{
			//Xの移動量が大きい場合は横移動量を移動量とする

			move_vec = { 0, Mouse::GetIns()->GetDiffPosX() * kCameraTargetMoveSpeed * -1, 0 };

			move_vec = designlab::rotVecByQuat(move_vec, camera_manager_ref_.getCameraRotQuat());
		}
		else
		{
			//Yの移動量が大きい場合は縦移動量を移動量とする

			move_vec = { 0, 0, Mouse::GetIns()->GetDiffPosY() * kCameraTargetMoveSpeed };

			move_vec = designlab::rotVecByQuat(move_vec, camera_manager_ref_.getCameraRotQuat());
		}

		namespace dldu = designlab::dxlib_util;

		VECTOR now_target_pos = camera_manager_ref_.getFreeTargetPos();				//現在のターゲット座標を取得

		now_target_pos = VAdd(now_target_pos, dldu::ConvertToDxlibVec(move_vec));	//移動量を加算

		camera_manager_ref_.setFreeTargetPos(now_target_pos);						//ターゲット座標を更新	

	}
}
