#pragma once

#include "camera_manager.h"


//! @class CameraController
//! @date 2023/08/20
//! @author 長谷川
//! @brief キー入力でカメラを動かすクラス
class CameraController
{
public:
	CameraController() = delete;

	//! @brief コンストラクタでマネージャーのポインタを受け取る
	CameraController(CameraManager* p_camera_manager);

	//! @brief キー入力でカメラを動かす．カメラマネージャーのポインタがなければ即終了する
	//! @n 基本的には毎フレーム呼び出す
	void update();

private:

	const float kCameraZoomSpeed = 50.0f;	//!< カメラのズーム速度

	const float kCameraMoveSpeed = 0.007f;	//!< カメラの移動速度


	CameraManager* mp_camera_manager;
};


//! @file camera_controller.h
//! @date 2023/08/20
//! @auther 長谷川
//! @brief カメラを動かすクラス
