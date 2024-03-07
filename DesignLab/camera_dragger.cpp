
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#include "camera_dragger.h"

#include "cassert_define.h"
#include "dxlib_util.h"


namespace designlab
{

CameraDragger::CameraDragger(const std::shared_ptr<DxlibCamera> camera) :
    camera_ptr_(camera)
{
    assert(camera_ptr_ != nullptr);
}

void CameraDragger::DraggedAction(const int cursor_dif_x, const int cursor_dif_y,
                                  const unsigned int mouse_key_bit)
{
    using math_util::Squared;

    assert(camera_ptr_ != nullptr);

    // カーソルが動いていたら，カメラの回転を変更する．
    if ((mouse_key_bit & MOUSE_INPUT_LEFT) && (mouse_key_bit & MOUSE_INPUT_RIGHT))
    {
        if (abs(cursor_dif_x) > abs(cursor_dif_y))
        {
            // カメラの回転をマウスの横移動量に合わせて変更．
            Quaternion move_quaternion_x = { 0, 0, 0, 0 };

            move_quaternion_x = Quaternion::MakeByAngleAxis(
                cursor_dif_x * kCameraMoveSpeed * -1, { 0, 0, 1 });

            Quaternion res = camera_ptr_->GetCameraQuat() * move_quaternion_x;

            res = res.GetNormalized();

            camera_ptr_->SetCameraQuat(res);
        }
        else
        {
            // カメラの回転をマウスの縦移動量に合わせて変更．
            Quaternion move_quaternion_y = Quaternion::MakeByAngleAxis(
                cursor_dif_y * kCameraMoveSpeed * -1, { 0, 1, 0 });

            Quaternion res = camera_ptr_->GetCameraQuat() * move_quaternion_y;

            res = res.GetNormalized();

            camera_ptr_->SetCameraQuat(res);
        }
    }
    else if (mouse_key_bit & MOUSE_INPUT_MIDDLE)
    {
        // ホイールクリックをしていたらカメラを回転させる．
        // XとYのどちらかの移動量が大きい方を処理する．

        if (abs(cursor_dif_x) > 0)
        {
            // カメラの回転をマウスの横移動量に合わせて変更．
            Quaternion move_quaternion_x = { 0, 0, 0, 0 };

            move_quaternion_x = Quaternion::MakeByAngleAxis(
                cursor_dif_x * kCameraMoveSpeed * -1, { 0, 0, 1 });

            Quaternion res = camera_ptr_->GetCameraQuat() * move_quaternion_x;

            res = res.GetNormalized();

            camera_ptr_->SetCameraQuat(res);
        }

        if (abs(cursor_dif_y) > 0)
        {
            // カメラの回転をマウスの縦移動量に合わせて変更．
            Quaternion move_quaternion_y = Quaternion::MakeByAngleAxis(
                cursor_dif_y * kCameraMoveSpeed * -1, { 0, 1, 0 });

            Quaternion res = camera_ptr_->GetCameraQuat() * move_quaternion_y;

            res = res.GetNormalized();

            camera_ptr_->SetCameraQuat(res);
        }
    }
    else if (mouse_key_bit & MOUSE_INPUT_LEFT)
    {
        // 左クリックしていたらカメラのビュー視点の中心軸を回転軸とした回転．
        const int mouse_move =
            abs(cursor_dif_x) > abs(cursor_dif_y) ? cursor_dif_x : cursor_dif_y;

        if (mouse_move != 0)
        {
            const Quaternion move_quaternion = Quaternion::MakeByAngleAxis(
                mouse_move * kCameraMoveSpeed * -1.0f, { 1, 0, 0 });

            const Quaternion res =
                (camera_ptr_->GetCameraQuat() * move_quaternion).GetNormalized();

            camera_ptr_->SetCameraQuat(res);
        }
    }
    else if (
        mouse_key_bit & MOUSE_INPUT_RIGHT &&
        Squared(cursor_dif_x) + Squared(cursor_dif_y) > Squared(kMouseMoveMargin))
    {
        // 右クリックしていたらカメラの平行移動．
        if (camera_ptr_->GetCameraViewMode() !=
            enums::CameraViewMode::kFreeControlledAndMovableTarget)
        {
            // 表示モードを注視点を自由に動かせるモードに変更．
            camera_ptr_->SetCameraViewMode(
                enums::CameraViewMode::kFreeControlledAndMovableTarget);
        }


        Vector3 move_vec_x = { 0, 0, 0 };

        if (abs(cursor_dif_x) > 0)
        {
            move_vec_x = { 0, cursor_dif_x * kCameraTargetMoveSpeed * -1, 0 };

            move_vec_x = RotateVector3(move_vec_x, camera_ptr_->GetCameraQuat());
        }

        Vector3 move_vec_y = { 0, 0, 0 };

        if (abs(cursor_dif_y) > 0)
        {
            move_vec_y = { 0, 0, cursor_dif_y * kCameraTargetMoveSpeed };

            move_vec_y = RotateVector3(move_vec_y, camera_ptr_->GetCameraQuat());
        }

        // 現在のターゲット座標を取得．
        Vector3 now_target_pos = camera_ptr_->GetFreeTargetPos();

        now_target_pos = now_target_pos + move_vec_x + move_vec_y;  // 移動量を加算．

        camera_ptr_->SetFreeTargetPos(now_target_pos);  // ターゲット座標を更新．
    }
}

void CameraDragger::RotMouseWheel(const int rot) const
{
    camera_ptr_->AddCameraToTargetLength(kCameraZoomSpeed * rot * -1);
}

}  // namespace designlab
