//! @file camera_state_manager.h
//! @brief Dxlibの3Dのカメラの状態を管理するクラス．


#ifndef DESIGNLAB_CAMERA_MANAGER_H_
#define DESIGNLAB_CAMERA_MANAGER_H_


#include <Dxlib.h>

#include "designlab_quaternion.h"


//! @enum CameraViewMode
//! @brief カメラの視点を表す列挙体．
//! @details CameraControllerクラスで使用されている．
//! @n カメラの視点を切り替える際に使用する．
//! @n 具体的な処理はCameraControllerクラスを参照すること．
enum class CameraViewMode
{
	kFrontView,			//!< 正面からの視点．
	kBackView,			//!< 背面からの視点．
	kTopView,			//!< 上からの見下ろし視点．
	kRightSideView,		//!< 右から真横の視点．
	kLeftSideView,		//!< 右から真横の視点．
	kFreeControlled,	//!< 自由に操作可能
	kFreeControlledAndMovableTarget	//!< 自由に操作可能かつ注視点を設定可能
};


//! @class CameraStateManager
//! @brief Dxlibの3Dのカメラの状態を管理するクラス．
//! @details カメラを，上から見るのか，横から見るのか，切り替えるのはDXlibの場合操作が少々難しい．
//! @n また，Dxlibは関数で処理を書くため，現在のカメラの状態を保持してまとめておく処理はデフォルトにない．
//! @n このクラスはその処理をまとめたことで，処理を簡単にしている
//! @n また，カメラの姿勢はクォータニオンで表している．
//! @n デフォルトは kDefaultCameraFrontVecの方を向いて，kDefaultCameraUpVecがカメラの上方向を表す．
class CameraStateManager final
{
public:

	CameraStateManager();

	//! @brief カメラの位置などの更新を行う．毎フレーム実行すること
	void Update();

	//! @brief カメラと注視点との距離を初期化する
	void InitCaneraTargetLength();

	//! @brief カメラの注視する目標の座標からカメラまでの距離を増やす
	//! @param [in] length_dif 増やす距離
	void AddCameraToTargetLength(float length_dif);


	//! @brief カメラのモードをセットする．同時にカメラの目標回転角度などを設定する
	//! @param [in] mode カメラの視点のモード
	void SetCameraViewMode(CameraViewMode mode);

	//! @brief カメラのモードを取得する
	//! @return CameraViewMode カメラの視点のモード
	inline CameraViewMode GetCameraViewMode() const { return camera_view_mode_; }

	//! @brief カメラの注視する目標の座標をセットする
	//! @n cameraのmodeがFREE_CONTROLLED_TARGETの時はセットできない
	//! @param [in] pos カメラの注視する目標の座標
	inline void SetTargetPos(const designlab::Vector3& pos) { goal_camera_state_.target_pos = pos; }

	//! @brief カメラの回転を表すクォータニオンを取得する．
	//! @return カメラのクォータニオン
	inline designlab::Quaternion GetCameraRotQuat() const { return goal_camera_state_.camera_rot_quat; }

	//! @brief カメラのクォータニオンをセットする
	//! @param [in] quat カメラのクォータニオン
	inline void SetCameraRotQuat(const designlab::Quaternion& quat) { goal_camera_state_.camera_rot_quat = quat; }

	//! @brief 注視点を操作する際の，カメラの注視する座標をセットする
	//! @param [in] pos カメラの注視する座標
	inline void SetFreeTargetPos(const designlab::Vector3& pos) { free_controlled_target_pos_ = pos; }

	//! @brief 注視点を操作する際の，カメラの注視する座標を取得する
	//! @return designlab::Vector3 カメラの注視する座標
	inline designlab::Vector3 GetFreeTargetPos() const { return free_controlled_target_pos_; }


private:


	struct CameraState final
	{
		CameraState() : camera_rot_quat{}, target_pos{},length_camera_to_target(0) {}

		designlab::Quaternion camera_rot_quat;	//!< カメラの回転を表すクォータニオン
		designlab::Vector3 target_pos;			//!< カメラの注視点
		float length_camera_to_target;	//!< カメラと注視点との距離
	};


	// now_camera_state_の値で，カメラの位置と姿勢をdxlibの関数でセットする	
	void SetCameraPosAndRot();

	const designlab::Vector3 kDefaultCameraFrontVec;	//!< デフォルトのカメラの方向を表す単位ベクトル

	const designlab::Vector3 kDefaultCameraUpVec;		//!< デフォルトのカメラの方向を表す単位ベクトル


	CameraViewMode camera_view_mode_;	//!< カメラの視点を決定する．

	designlab::Vector3 free_controlled_target_pos_;		//!< カメラの注視点を自由に操作する際の注視点の座標

	CameraState now_camera_state_;		//!< 現在のカメラの状態をまとめた構造体

	CameraState goal_camera_state_;		//!< 目標とするカメラの状態をまとめた構造体
};


#endif // !DESIGNLAB_CAMERA_MANAGER_H_