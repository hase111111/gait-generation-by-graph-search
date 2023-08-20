#pragma once

#include "DxLib.h"

#include "my_rotator.h"
#include "my_math.h"


//! @enum ECameraMode
//! @date 2023/08/14
//! @author 長谷川
//! @brief カメラの視点を表す列挙体．
//! @details CameraControllerクラスで使用されている．
//! 具体的な処理はCameraControllerクラスを参照すること．
enum class ECameraMode : int
{
	FRONT_VIEW,			//!< 正面図
	BACK_VIEW,			//!< 背面図
	TOP_VIEW,			//!< 上からの見下ろし視点．
	RIGHT_SIDE_VIEW,	//!< 右から真横の視点．
	LEFT_SIDE_VIEW,		//!< 右から真横の視点．
	FREE_CONTROLLED,	//!< 自由に操作可能
};


//! @class CameraController
//! @date 2023/08/14
//! @author 長谷川
//! @brief Dxlibの3Dのカメラの処理を行うクラス
//! @details カメラを，上から見るのか，横から見るのか，切り替えるのはDXlibの場合操作が少々難しい
//! @n このクラスはその処理をまとめたことで，処理を簡単にしている
class CameraController final
{
public:

	CameraController();
	~CameraController() = default;

	//! @brief カメラの位置などの更新を行う．毎フレーム実行すること
	void update();

	//! @brief カメラの注視する目標の座標をセットする
	//! @param [in] pos カメラの注視する目標の座標
	void setTargetPos(const VECTOR pos) { m_goal_target_pos = pos; }

	//! @brief カメラのモードをセットする
	//! @param [in] mode カメラの視点のモード
	void setCameraViewMode(const ECameraMode mode);

	//! @brief カメラの注視する目標の座標とカメラの距離を増やす
	//! @param [in] length_dif 増やす距離
	void addCameraToTargetLength(const float length_dif);

private:

	//カメラを目標値に徐々に近づけるのに使用する．戻り値は移動後のVECTOR
	VECTOR approachTargetVECTOR(const VECTOR& current, const VECTOR& target) const;

	//カメラを目標値に徐々に近づけるのに使用する．上の関数より更にゆっくり近づく．
	VECTOR approachSlowlyTargetVECTOR(const VECTOR& current, const VECTOR& target) const;

	my_vec::SRotator approachTargetRotator(const my_vec::SRotator& current, const my_vec::SRotator& target) const;

	//引数の値を徐々に目標値に近づける．
	float approachTargetValue(const float current, const float target) const;

	//引数の値を徐々に目標値に近づける．上の関数より更にゆっくり近づく．
	float approachSlowlyTargetValue(const float current, const float target) const;


	const float CAMERA_SLOW_SPEED = 0.1f;				//カメラの上方向のベクトルは単位ベクトルであるので，下2つの値だと速すぎる．
	const float CAMERA_SPEED = 3.0f;					//カメラが移動する速さ．
	const float CAMERA_HIGH_SPEED = 25.0f;				//カメラが高速で移動するときの速さ.
	const float CAMERA_HIGH_DIF = 150.0f;				//目標座標とこの値以上に離れているならば，CAMERA_HIGH_SPEEDの値で急いで近づく．
	const float CAMERA_ANGLE = my_math::MY_FLT_PI / 4;	//斜めからのカメラの角度
	const my_vec::SVector kDefaultCameraFrontVec = { 1.0f, 0.0f, 0.0f };	//デフォルトのカメラの方向を表す単位ベクトル

	ECameraMode m_camera_view_mode;			//カメラの視点を決定する．

	VECTOR m_goal_target_pos;	//カメラが注視する座標の目標値
	float m_goal_length_camera_to_target;	//カメラと注視する対象との距離の目標値
	VECTOR m_goal_upvec;		//カメラの上方向のベクトルの目標値
	my_vec::SRotator m_goal_camera_rot;	//カメラの回転の目標値

	VECTOR m_target_pos;	//カメラが現在注視している座標
	float m_length_camera_to_target;	//カメラと注視する対象との距離の現在値
	VECTOR m_camera_upvec;	//カメラの上方向のベクトルの現在値
	my_vec::SRotator m_camera_rot;	//カメラの回転の目標値
};


//! @file camera_controller.h
//! @date 2023/08/14
//! @author 長谷川
//! @brief 画像表示処理においてカメラを制御するクラス．
//! @n 行数 : @lineinfo
