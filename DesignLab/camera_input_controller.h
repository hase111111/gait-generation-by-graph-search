//! @file camera_input_controller.h
//! @brief マウスの入力でカメラを動かすクラス．

#ifndef DESIGNLAB_CAMERA_INPUT_CONTROLLER_H_
#define DESIGNLAB_CAMERA_INPUT_CONTROLLER_H_

#include <memory>

#include "dxlib_camera.h"
#include "interface_dxlib_clickable.h"


//! @class CameraInputController
//! @brief マウスの入力でカメラを動かすクラス．
class CameraInputController final : public IDxlibClickable
{
public:

	//! @param [in] camera_manager カメラの状態を管理するクラスをポインタで受け取る．
	CameraInputController(const std::shared_ptr<DxlibCamera> camera);

	void Activate(const std::shared_ptr<const Mouse> mouse_ptr) override;

	//! @brief 全ての場所においてクリック可能にするため，常にtrueを返す．
	bool OnCursor([[maybe_unused]] int cursor_x, [[maybe_unused]] int cursor_y) const noexcept override { return true; };

private:

	const float kCameraZoomSpeed{ 50.f };		//!< カメラのズーム速度

	const float kCameraMoveSpeed{ 0.007f };		//!< カメラの移動速度

	const float kCameraTargetMoveSpeed{ 3.f };	//!< カメラの注視点の移動速度

	const double kMouseMoveMargin{ 0.5 };		//!< マウスの移動量がこの量以下ならば0とみなす

	const std::shared_ptr<DxlibCamera> camera_ptr_;		//!< カメラの状態を管理するクラスのポインタ

};


#endif	// DESIGNLAB_CAMERA_INPUT_CONTROLLER_H_