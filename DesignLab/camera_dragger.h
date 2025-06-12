﻿
//! @file camera_dragger.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_CAMERA_DRAGGER_H_
#define DESIGNLAB_CAMERA_DRAGGER_H_

#include <memory>

#include "dxlib_camera.h"
#include "interface_dxlib_draggable.h"
#include "interface_dxlib_wheel_handler.h"


namespace designlab
{

//! @class CameraDragger
//! @brief マウスの入力でカメラを動かすクラス.
class CameraDragger final : public IDxlibDraggable, public IDxlibWheelHandler
{
public:
    //! @param[in] camera_manager カメラの状態を管理するクラスをポインタで受け取る.
    explicit CameraDragger(const std::shared_ptr<DxlibCamera> camera);

    bool IsDraggable(
        [[maybe_unused]] int cursor_x,
        [[maybe_unused]] int cursor_y) const override
    {
        // カメラの操作はどこをクリックしても可能.
        return true;
    };

    bool IsDragged() const override { return is_dragged_; };

    void SetDragged(bool is_dragged) override { is_dragged_ = is_dragged; };

    void DraggedAction(
        int cursor_dif_x, int cursor_dif_y,
        unsigned int mouse_key_bit) override;

    bool CanHandleWheel(
        [[maybe_unused]] int cursor_x,
        [[maybe_unused]] int cursor_y) const override
    {
        // カメラの操作はどこをクリックしても可能.
        return true;
    };

    void RotMouseWheel(int rot) const override;

private:
    const float kCameraZoomSpeed{ 50.f };  //!< カメラのズーム速度.

    const float kCameraMoveSpeed{ 0.007f };  //!< カメラの移動速度.

    const float kCameraTargetMoveSpeed{ 3.f };  //!< カメラの注視点の移動速度.

    //! マウスの移動量がこの量以下ならば0とみなす.
    const double kMouseMoveMargin{ 0.5 };

    //! カメラの状態を管理するクラスのポインタ.
    const std::shared_ptr<DxlibCamera> camera_ptr_;

    bool is_dragged_{ false };  //!< ドラッグ中かどうかのフラグ.
};

}  // namespace designlab


#endif  // DESIGNLAB_CAMERA_DRAGGER_H_
