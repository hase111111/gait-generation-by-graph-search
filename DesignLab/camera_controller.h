//! @file camera_controller.h
//! @brief マウスの入力でカメラを動かすクラス

#ifndef DESIGNLABO_CAMERA_CONTROLLER_H_
#define DESIGNLABO_CAMERA_CONTROLLER_H_

#include "camera_state_manager.h"


//! @class CameraController
//! @brief マウスの入力でカメラを動かすクラス

class CameraController
{
public:
	//! @brief デフォルトコンストラクタ，コピーコンストラクタ，代入演算子は禁止
	CameraController() = delete;
	CameraController(const CameraController&) = delete;
	CameraController& operator=(const CameraController&) = delete;

	//! @brief コンストラクタでマネージャーの参照を受け取る
	CameraController(CameraStateManager& camera_manager_ref);


	//! @brief キー入力でカメラを動かす．
	//! @n 基本的には毎フレーム呼び出す
	void Update();

private:

	const float kCameraZoomSpeed = 50.0f;		//!< カメラのズーム速度

	const float kCameraMoveSpeed = 0.007f;		//!< カメラの移動速度

	const float kCameraTargetMoveSpeed = 3.0f;	//!< カメラの注視点の移動速度

	const double kMouseMoveMargin = 2.0;		//!< マウスの移動量がこの量以下ならば0とみなす


	CameraStateManager& camera_manager_ref_;
};


#endif // !DESIGNLABO_CAMERA_CONTROLLER_H_