//! @file camera_input_controller.h
//! @brief マウスの入力でカメラを動かすクラス

#ifndef DESIGNLAB_CAMERA_INPUT_CONTROLLER_H_
#define DESIGNLAB_CAMERA_INPUT_CONTROLLER_H_


#include "camera_state_manager.h"


//! @class CameraInputController
//! @brief マウスの入力でカメラを動かすクラス
class CameraInputController
{
public:

	CameraInputController();

	//! @brief キー入力でカメラを動かす．
	//! @n 基本的には毎フレーム呼び出す
	//! @param [out] camera_manager カメラの状態を管理するクラスのポインタ．受け取った値を書き換える
	void ChangeCameraState(CameraStateManager* camera_manager);

private:

	const float kCameraZoomSpeed;		//!< カメラのズーム速度

	const float kCameraMoveSpeed;		//!< カメラの移動速度

	const float kCameraTargetMoveSpeed;	//!< カメラの注視点の移動速度

	const double kMouseMoveMargin;		//!< マウスの移動量がこの量以下ならば0とみなす
};


#endif	// DESIGNLAB_CAMERA_INPUT_CONTROLLER_H_