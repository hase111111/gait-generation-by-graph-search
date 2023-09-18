#pragma once

#include "DxLib.h"

#include "designlab_quaternion.h"
#include "designlab_math.h"


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
	FREE_CONTROLLED,	//!< 自由に操作可能
	FREE_CONTROLLED_TARGET	//!< 自由に操作可能かつ注視点を設定可能
};


//! @class CameraStateManager
//! @brief Dxlibの3Dのカメラの処理を行うクラス
//! @details カメラを，上から見るのか，横から見るのか，切り替えるのはDXlibの場合操作が少々難しい
//! @n このクラスはその処理をまとめたことで，処理を簡単にしている
//! @n また，カメラの姿勢はクォータニオンで表している．

class CameraStateManager final
{
public:

	CameraStateManager();

	//! @brief カメラの位置などの更新を行う．毎フレーム実行すること
	void Update();


	//! @brief カメラのモードをセットする．同時にカメラの目標回転角度などを設定する
	//! @param [in] mode カメラの視点のモード
	void setCameraViewMode(const CameraViewMode mode);

	//! @brief カメラのモードを取得する
	//! @return CameraViewMode カメラの視点のモード
	CameraViewMode getCameraViewMode() const { return m_camera_view_mode; }


	//! @brief カメラと注視点との距離を初期化する
	void initCaneraTargetLength();

	//! @brief カメラの注視する目標の座標とカメラの距離を増やす
	//! @param [in] length_dif 増やす距離
	void addCameraToTargetLength(const float length_dif);


	//! @brief カメラの注視する目標の座標をセットする
	//! @n cameraのmodeがFREE_CONTROLLED_TARGETの時はセットできない
	//! @param [in] pos カメラの注視する目標の座標
	void setTargetPos(const VECTOR pos) { m_goal_target_pos = pos; }


	//! @brief カメラのクォータニオンを取得する
	//! @return カメラのクォータニオン
	dl_vec::SQuaternion getCameraRotQuat() const { return m_goal_camera_rot_quat; }

	//! @brief カメラのクォータニオンをセットする
	//! @param [in] quat カメラのクォータニオン
	void setCameraRotQuat(const dl_vec::SQuaternion& quat) { m_goal_camera_rot_quat = quat; }


	//! @brief 注視点を操作する際の，カメラの注視する座標をセットする
	//! @param [in] pos カメラの注視する座標
	void setFreeTargetPos(const VECTOR pos) { m_free_controlled_target = pos; }

	//! @brief 注視点を操作する際の，カメラの注視する座標を取得する
	VECTOR getFreeTargetPos() const { return m_free_controlled_target; }


private:

	//カメラの位置と姿勢をdxlibの関数でセットする	
	void setCameraPosAndRot();

	//引数のクォータニオンを徐々に目標値に近づける．補間方法は線形補間 https://www.f-sp.com/entry/2017/06/30/221124
	dl_vec::SQuaternion approachTargetQuat(const dl_vec::SQuaternion& current, const dl_vec::SQuaternion& target) const;

	//引数のベクトルを徐々に目標値に近づける
	VECTOR approachTargetVECTOR(const VECTOR& current, const VECTOR& target) const;

	//引数の値を徐々に目標値に近づける．
	float approachTargetValue(const float current, const float target) const;


	const dl_vec::SVector kDefaultCameraFrontVec = { 1.0f, 0.0f, 0.0f };	//デフォルトのカメラの方向を表す単位ベクトル

	const dl_vec::SVector kDefaultCameraUpVec = { 0.0f, 0.0f, 1.0f };		//デフォルトのカメラの方向を表す単位ベクトル


	CameraViewMode m_camera_view_mode;				//カメラの視点を決定する．

	VECTOR m_goal_target_pos;					//カメラが注視する座標の目標値

	VECTOR m_target_pos;						//カメラが現在注視している座標

	dl_vec::SQuaternion m_goal_camera_rot_quat;	//カメラの回転を表すクォータニオン

	dl_vec::SQuaternion m_camera_rot_quat;		//カメラの回転を表すクォータニオンの現在値

	float m_goal_length_camera_to_target;		//カメラと注視する対象との距離の目標値

	float m_length_camera_to_target;			//カメラと注視する対象との距離の現在値

	VECTOR m_free_controlled_target;			//カメラの注視点を操作する際の注視点の座標
};


//! @file camera_manager.h
//! @date 2023/08/14
//! @author 長谷川
//! @brief 画像表示処理においてカメラを制御するクラス．
//! @n 行数 : @lineinfo
