
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "dxlib_camera.h"

#include <cmath>

#include <Dxlib.h>

#include "cassert_define.h"
#include "math_util.h"
#include "dxlib_util.h"
#include "graphic_const.h"


namespace designlab
{

DxlibCamera::DxlibCamera() :
    kDefaultCameraFrontVec{ Vector3::GetFrontVec() },
    kDefaultCameraUpVec{ Vector3::GetUpVec() },
    camera_view_mode_(enums::CameraViewMode::kTopView),
    free_controlled_target_pos_{},
    now_camera_state_{},
    goal_camera_state_{}
{
    SetCameraViewMode(enums::CameraViewMode::kTopView);  // カメラの初期位置をセットする．

    SetCameraPosAndRot();  // カメラ位置をセットする．

    InitCameraTargetLength();  // カメラの距離を初期化する．

    now_camera_state_.camera_quat =
        Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(-90.0f), Vector3::GetLeftVec()) *
        Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(180.0f), Vector3::GetFrontVec());
}


void DxlibCamera::Update()
{
    // カメラの距離を目標値に近づける．
    now_camera_state_.length_camera_to_target = math_util::ApproachTarget(
        now_camera_state_.length_camera_to_target,
        goal_camera_state_.length_camera_to_target,
        0.1f);

    // カメラの回転を目標値に近づける．
    now_camera_state_.camera_quat = SlerpQuaternion(
        now_camera_state_.camera_quat,
        goal_camera_state_.camera_quat,
        0.1f);

    now_camera_state_.camera_quat = now_camera_state_.camera_quat.GetNormalized();


    // カメラの注視点を目標値に近づける．
    if (camera_view_mode_ != enums::CameraViewMode::kFreeControlledAndMovableTarget)
    {
        now_camera_state_.target_pos = math_util::ApproachTarget(
            now_camera_state_.target_pos,
            goal_camera_state_.target_pos,
            0.1f);
    }
    else
    {
        now_camera_state_.target_pos = math_util::ApproachTarget(
            now_camera_state_.target_pos,
            free_controlled_target_pos_,
            0.1f);
    }


    // カメラ位置をセットする．
    SetCameraPosAndRot();

    if (kOutputDebugLog)
    {
        printfDx("cameraの姿勢(クォータニオン) w = %f, v= { %f, %f, %f }\n",
            now_camera_state_.camera_quat.w,
            now_camera_state_.camera_quat.v.x,
            now_camera_state_.camera_quat.v.y,
            now_camera_state_.camera_quat.v.z);

        printfDx("cameraの注視点 x = %f, y = %f, z = %f\n",
            now_camera_state_.target_pos.x,
            now_camera_state_.target_pos.y,
            now_camera_state_.target_pos.z);

        printfDx("cameraの距離 %f\n", now_camera_state_.length_camera_to_target);
    }
}


void DxlibCamera::SetCameraViewMode(const enums::CameraViewMode mode)
{
    camera_view_mode_ = mode;

    // ゴール座標を更新する．
    switch (mode)
    {
    case enums::CameraViewMode::kFrontView:
    {
        goal_camera_state_.camera_quat =
            Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(0.0f), Vector3::GetUpVec());
        return;
    }

    case enums::CameraViewMode::kBackView:
    {
        goal_camera_state_.camera_quat =
            Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(180.0f), Vector3::GetUpVec());
        return;
    }

    case enums::CameraViewMode::kTopView:
    {
        Quaternion quat1 = Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(-90.0f),
            Vector3::GetLeftVec());

        Quaternion quat2 = Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(180.0f),
            Vector3::GetFrontVec());

        goal_camera_state_.camera_quat = quat1 * quat2;

        return;
    }

    case enums::CameraViewMode::kRightSideView:
    {
        goal_camera_state_.camera_quat =
            Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(-90.0f), Vector3::GetUpVec());
        return;
    }

    case enums::CameraViewMode::kLeftSideView:
    {
        goal_camera_state_.camera_quat =
            Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(90.0f), Vector3::GetUpVec());
        return;
    }

    case enums::CameraViewMode::kFreeControlled:
    {
        return;
    }

    case enums::CameraViewMode::kFreeControlledAndMovableTarget:
    {
        free_controlled_target_pos_ = goal_camera_state_.target_pos;
        return;
    }

    default:
        assert(false);  // ここに来ることはない．
        break;
    }
}


void DxlibCamera::InitCameraTargetLength()
{
    // 最大と最小の中間値を初期値とする．
    goal_camera_state_.length_camera_to_target = kDefaultCameraZoom;
}


void DxlibCamera::AddCameraToTargetLength(const float length_dif)
{
    goal_camera_state_.length_camera_to_target += length_dif;

    // カメラの距離が最大値を超えたら最大値にする．
    if (GraphicConst::kCameraToTargetMax < goal_camera_state_.length_camera_to_target)
    {
        goal_camera_state_.length_camera_to_target = GraphicConst::kCameraToTargetMax;
    }

    // カメラの距離が最小値を下回ったら最小値にする．
    if (goal_camera_state_.length_camera_to_target < GraphicConst::kCameraToTargetMin)
    {
        goal_camera_state_.length_camera_to_target = GraphicConst::kCameraToTargetMin;
    }
}


void DxlibCamera::SetCameraPosAndRot()
{
    // カメラの位置をセットする．クォータニオンを用いて回転させ，
    // Vector3 から dxlib::VECTOR に変換する．

    Vector3 camera_target_dif =
        RotateVector3(kDefaultCameraFrontVec, now_camera_state_.camera_quat) *
        now_camera_state_.length_camera_to_target;

    VECTOR camera_pos =
        dxlib_util::ConvertToDxlibVec(camera_target_dif + now_camera_state_.target_pos);

    VECTOR camera_upvec =
        dxlib_util::ConvertToDxlibVec(
            RotateVector3(kDefaultCameraUpVec, now_camera_state_.camera_quat));

    SetCameraPositionAndTargetAndUpVec(
        camera_pos,
        dxlib_util::ConvertToDxlibVec(now_camera_state_.target_pos),
        camera_upvec);

    // ChangeLightTypeDirを使っているので，カメラ向きに合わせてライトの向きも変更する．
    VECTOR light_dir =
        dxlib_util::ConvertToDxlibVec(
            -RotateVector3(kDefaultCameraFrontVec, now_camera_state_.camera_quat));

    ChangeLightTypeDir(light_dir);
}

}  // namespace designlab
