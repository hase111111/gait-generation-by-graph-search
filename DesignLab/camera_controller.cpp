#include "camera_controller.h"

#include "mouse.h"


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


	//ホイールクリックをしていたらカメラを回転させる
	if (Mouse::getIns()->getPushingCountMiddle() > 0)
	{
		if (abs(Mouse::getIns()->getDiffPosX()) > abs(Mouse::getIns()->getDiffPosY()))
		{
			//カメラの回転をマウスの横移動量に合わせて変更
			dl_vec::SQuaternion move_quatx{0, 0, 0, 0};

			move_quatx.setRotAngleAndAxis(Mouse::getIns()->getDiffPosX() * kCameraMoveSpeed * -1, { 0,0,1 });

			dl_vec::SQuaternion res = mp_camera_manager->getCameraRotQuat() * move_quatx;

			mp_camera_manager->setCameraRotQuat(res);
		}
		else
		{
			//カメラの回転をマウスの縦移動量に合わせて変更
			dl_vec::SQuaternion move_quaty{ 0, 0, 0, 0 };

			move_quaty.setRotAngleAndAxis(Mouse::getIns()->getDiffPosY() * kCameraMoveSpeed * -1, { 0,1,0 });

			dl_vec::SQuaternion res = mp_camera_manager->getCameraRotQuat() * move_quaty;

			mp_camera_manager->setCameraRotQuat(res);
		}
	}


	//カメラのビュー視点の中心軸を回転軸とした回転
	if (Mouse::getIns()->getPushingCountRight() > 0)
	{
		dl_vec::SQuaternion move_quat{ 0, 0, 0, 0 };

		int mouse_move = (abs(Mouse::getIns()->getDiffPosX()) > abs(Mouse::getIns()->getDiffPosY())) ? Mouse::getIns()->getDiffPosX() : Mouse::getIns()->getDiffPosY();

		move_quat.setRotAngleAndAxis(mouse_move * kCameraMoveSpeed * -1.0f, { 1,0,0 });

		dl_vec::SQuaternion res = mp_camera_manager->getCameraRotQuat() * move_quat;

		mp_camera_manager->setCameraRotQuat(res);
	}
}
