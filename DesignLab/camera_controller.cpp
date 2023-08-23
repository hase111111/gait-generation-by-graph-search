#include "camera_controller.h"

#include "mouse.h"
#include "designlab_dxlib.h"


CameraController::CameraController(CameraManager* p_camera_manager) : mp_camera_manager(p_camera_manager) {}


void CameraController::update()
{
	//早期リターン．ポインタがnullptrなら何もしない
	if (mp_camera_manager == nullptr) { return; }


	//ホイールが動いていたらカメラ距離を変更する
	if (Mouse::getIns()->getWheelRot() != 0)
	{
		mp_camera_manager->addCameraToTargetLength(kCameraZoomSpeed * Mouse::getIns()->getWheelRot() * -1);
	}



	if (Mouse::getIns()->getPushingCountMiddle() > 0)
	{

		//ホイールクリックをしていたらカメラを回転させる．XとYのどちらかの移動量が大きい方を処理する

		if (abs(Mouse::getIns()->getDiffPosX()) > abs(Mouse::getIns()->getDiffPosY()))
		{
			//カメラの回転をマウスの横移動量に合わせて変更
			dl_vec::SQuaternion move_quatx{0, 0, 0, 0};

			move_quatx.setRotAngleAndAxis(Mouse::getIns()->getDiffPosX() * kCameraMoveSpeed * -1, { 0,0,1 });

			dl_vec::SQuaternion res = mp_camera_manager->getCameraRotQuat() * move_quatx;

			res = res.normalize();

			mp_camera_manager->setCameraRotQuat(res);
		}
		else
		{
			//カメラの回転をマウスの縦移動量に合わせて変更
			dl_vec::SQuaternion move_quaty{ 0, 0, 0, 0 };

			move_quaty.setRotAngleAndAxis(Mouse::getIns()->getDiffPosY() * kCameraMoveSpeed * -1, { 0,1,0 });

			dl_vec::SQuaternion res = mp_camera_manager->getCameraRotQuat() * move_quaty;

			res = res.normalize();

			mp_camera_manager->setCameraRotQuat(res);
		}

	}
	else if (Mouse::getIns()->getPushingCountLeft() > 0)
	{

		//左クリックしていたらカメラのビュー視点の中心軸を回転軸とした回転

		dl_vec::SQuaternion move_quat{ 0, 0, 0, 0 };

		int mouse_move = (abs(Mouse::getIns()->getDiffPosX()) > abs(Mouse::getIns()->getDiffPosY())) ? Mouse::getIns()->getDiffPosX() : Mouse::getIns()->getDiffPosY();

		move_quat.setRotAngleAndAxis(mouse_move * kCameraMoveSpeed * -1.0f, { 1,0,0 });

		dl_vec::SQuaternion res = mp_camera_manager->getCameraRotQuat() * move_quat;

		res = res.normalize();

		mp_camera_manager->setCameraRotQuat(res);

	}
	else if (Mouse::getIns()->getPushingCountRight() > 0 && Mouse::getIns()->getDiffPos() > kMouseMoveMargin)
	{

		//右クリックしていたらカメラの平行移動

		if (mp_camera_manager->getCameraViewMode() != ECameraMode::FREE_CONTROLLED_TARGET)
		{
			//表示モードを注視点を自由に動かせるモードに変更
			mp_camera_manager->setCameraViewMode(ECameraMode::FREE_CONTROLLED_TARGET);
		}


		dl_vec::SVector move_vec;

		if (abs(Mouse::getIns()->getDiffPosX()) > abs(Mouse::getIns()->getDiffPosY()))
		{
			//Xの移動量が大きい場合は横移動量を移動量とする

			move_vec = { 0, Mouse::getIns()->getDiffPosX() * kCameraTargetMoveSpeed * -1, 0 };

			move_vec = dl_vec::rotVecByQuat(move_vec, mp_camera_manager->getCameraRotQuat());
		}
		else
		{
			//Yの移動量が大きい場合は縦移動量を移動量とする

			move_vec = { 0, 0, Mouse::getIns()->getDiffPosY() * kCameraTargetMoveSpeed };

			move_vec = dl_vec::rotVecByQuat(move_vec, mp_camera_manager->getCameraRotQuat());
		}


		VECTOR now_target_pos = mp_camera_manager->getFreeTargetPos();				//現在のターゲット座標を取得

		now_target_pos = VAdd(now_target_pos, dl_dxlib::convertToDxVec(move_vec));	//移動量を加算

		mp_camera_manager->setFreeTargetPos(now_target_pos);						//ターゲット座標を更新	

	}
}
