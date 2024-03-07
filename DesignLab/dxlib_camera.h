
//! @file      dxlib_camera.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_DXLIB_CAMERA_H_
#define DESIGNLAB_DXLIB_CAMERA_H_


#include <Dxlib.h>

#include "math_quaternion.h"


namespace designlab::enums
{

//! @enum CameraViewMode
//! @brief カメラの視点を表す列挙体．
//! @details
//! CameraController クラスで使用されている．
//! @n カメラの視点を切り替える際に使用する．
//! @n 具体的な処理は CameraController クラスを参照すること．
enum class CameraViewMode
{
    kFrontView,  //!< 正面からの視点．
    kBackView,  //!< 背面からの視点．
    kTopView,  //!< 上からの見下ろし視点．
    kRightSideView,  //!< 右から真横の視点．
    kLeftSideView,  //!< 右から真横の視点．
    kFreeControlled,  //!< 自由に操作可能．
    kFreeControlledAndMovableTarget,  //!< 自由に操作可能かつ注視点を設定可能．
};

}  // namespace designlab::enums


namespace designlab
{

//! @class DxlibCamera
//! @brief Dxlib の3Dのカメラの状態を管理するクラス．
//! @details
//! カメラを，上から見るのか，横から見るのか，切り替えるのは Dxlib の場合操作が少々難しい．
//! @n また，Dxlib は関数で処理を書くため，
//! 現在のカメラの状態を保持してまとめておく処理はデフォルトにない．
//! @n このクラスはその処理をまとめたことで，処理を簡単にしている
//! @n また，カメラの姿勢はクォータニオンで表している．
//! @n デフォルトは kDefaultCameraFrontVec の方を向いて，
//! kDefaultCameraUpVec がカメラの上方向を表す．
class DxlibCamera final
{
public:
    DxlibCamera();

    //! @brief カメラの位置などの更新を行う．毎フレーム実行すること．
    void Update();

    //! @brief カメラと注視点との距離を初期化する．
    void InitCameraTargetLength();

    //! @brief カメラの注視する目標の座標からカメラまでの距離を増やす．
    //! @param[in] length_dif 増やす距離．
    void AddCameraToTargetLength(float length_dif);


    //! @brief カメラのモードをセットする．同時にカメラの目標回転角度などを設定する．
    //! @param[in] mode カメラの視点のモード
    void SetCameraViewMode(enums::CameraViewMode mode);

    //! @brief カメラのモードを取得する
    //! @return カメラの視点のモード
    inline enums::CameraViewMode GetCameraViewMode() const
    {
        return camera_view_mode_;
    }

    //! @brief カメラの注視する目標の座標をセットする．
    //! @n camera の mode が kFreeControlledAndMovableTarget の時はセットできない．
    //! @param[in] pos カメラの注視する目標の座標．
    inline void SetTargetPos(const Vector3& pos)
    {
        goal_camera_state_.target_pos = pos;
    }

    //! @brief カメラの回転を表すクォータニオンを取得する．
    //! @return カメラのクォータニオン
    inline Quaternion GetCameraQuat() const
    {
        return goal_camera_state_.camera_quat.GetNormalized();
    }

    //! @brief カメラのクォータニオンをセットする．
    //! @param[in] quat カメラのクォータニオン．
    inline void SetCameraQuat(const Quaternion& quat)
    {
        goal_camera_state_.camera_quat = quat.GetNormalized();
    }

    //! @brief 注視点を操作する際の，カメラの注視する座標をセットする．
    //! @param[in] pos カメラの注視する座標．
    inline void SetFreeTargetPos(const Vector3& pos)
    {
        free_controlled_target_pos_ = pos;
    }

    //! @brief 注視点を操作する際の，カメラの注視する座標を取得する．
    //! @return カメラの注視する座標．
    inline Vector3 GetFreeTargetPos() const
    {
        return free_controlled_target_pos_;
    }

    constexpr Quaternion GetNowCameraQuat() const
    {
        return now_camera_state_.camera_quat;
    }

    constexpr Vector3 GetNowTargetPos() const
    {
        return now_camera_state_.target_pos;
    }

    constexpr float GetNowCameraToTargetLength() const
    {
        return now_camera_state_.length_camera_to_target;
    }

    constexpr Vector3 GetNowCameraPos() const
    {
        Vector3 camera_target_dif =
            RotateVector3(kDefaultCameraFrontVec, now_camera_state_.camera_quat) *
            now_camera_state_.length_camera_to_target;

        return camera_target_dif + now_camera_state_.target_pos;
    }

private:
    static constexpr float kDefaultCameraZoom{ 1500.f };  //!< カメラのズームの初期値．

    struct CameraState final
    {
        CameraState() : camera_quat{}, target_pos{}, length_camera_to_target(0) {}

        Quaternion camera_quat;  //!< カメラの回転を表すクォータニオン．
        Vector3 target_pos;      //!< カメラの注視点．
        float length_camera_to_target;  //!< カメラと注視点との距離．
    };


    //  now_camera_state_ の値で，カメラの位置と姿勢を dxlib の関数でセットする．
    void SetCameraPosAndRot();


    constexpr static bool kOutputDebugLog = false;  //!< デバッグログを出力するかどうか．

    const Vector3 kDefaultCameraFrontVec;  //!< デフォルトのカメラの方向を表す単位ベクトル．

    const Vector3 kDefaultCameraUpVec;  //!< デフォルトのカメラの上方向を表す単位ベクトル．


    enums::CameraViewMode camera_view_mode_;  //!< カメラの視点を決定する．

    Vector3 free_controlled_target_pos_;      //!< カメラの注視点を自由に操作する際の注視点の座標．

    CameraState now_camera_state_;            //!< 現在のカメラの状態をまとめた構造体．

    CameraState goal_camera_state_;           //!< 目標とするカメラの状態をまとめた構造体．
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_CAMERA_H_
