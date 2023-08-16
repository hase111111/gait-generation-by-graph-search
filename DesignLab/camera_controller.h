#pragma once

#include "DxLib.h"

#include "my_vector.h"
#include "my_math.h"


//! @enum ECameraMode
//! @date 2023/08/14
//! @author 長谷川
//! @brief カメラの視点を表現する列挙体．
//! @details CameraControllerクラスでのみ使用されている．
//! 具体的な処理はCameraControllerクラスを参照すること．
enum class ECameraMode : int
{
	TOP_VIEW,			//!< 上からの見下ろし視点．
	OVERHEAD_VIEW,		//!< 右斜め上からの見下ろし視点．
	SIDE_VIEW,			//!< 右から真横の視点．
	OVERHEAD_VIEW_FLIP,	//!< 左斜め上からの見下ろし視点．
	SIDE_VIEW_FLIP,		//!< 左から真横の視点．
};


//! @class CameraController
//! @date 2023/08/14
//! @author 長谷川
//! @brief Dxlibの3Dのカメラの処理を行うクラス．
//! @details カメラを，上から見るのか，横から見るのか，切り替えるのはDXlibの場合操作が少々難しい．
//! @n このクラスはその処理をまとめたことで，処理を簡単にしている．
class CameraController final
{
public:

	CameraController();
	~CameraController() = default;

	//! @brief カメラの位置などの更新を行う．毎フレーム実行すること．
	void update();

	//! @brief カメラの注視する目標の座標をセットする．
	//! @param [in] pos カメラの注視する目標の座標
	inline void setTargetPos(const VECTOR pos) { m_goal_target_pos = pos; }

	//! @brief カメラのモードをセットする．
	//! @param [in] mode カメラの視点のモード
	inline void setCameraMode(const ECameraMode mode) { m_mode = mode; }

private:

	const float CAMERA_SLOW_SPEED = 0.02f;				//カメラの上方向のベクトルは単位ベクトルであるので，下2つの値だと速すぎる．
	const float CAMERA_SPEED = 3.0f;					//カメラが移動する速さ．
	const float CAMERA_HIGH_SPEED = 25.0f;				//カメラが高速で移動するときの速さ.
	const float CAMERA_HIGH_DIF = 150.0f;				//目標座標とこの値以上に離れているならば，CAMERA_HIGH_SPEEDの値で急いで近づく．
	const float CAMERA_ANGLE = my_math::MY_FLT_PI / 4;	//斜めからのカメラの角度


	//カメラの目標座標を設定する．その座標に徐々に近づくようにカメラは移動する．
	void setGoalCameraPos();

	//カメラを目標値に徐々に近づけるのに使用する．戻り値は移動後のVECTOR
	VECTOR approachTargetVECTOR(const VECTOR& current, const VECTOR& target) const;

	//カメラを目標値に徐々に近づけるのに使用する．上の関数より更にゆっくり近づく．
	VECTOR approachSlowlyTargetVECTOR(const VECTOR& current, const VECTOR& target) const;

	//引数の値を徐々に目標値に近づける．
	float approachTargetValue(const float current, const float target) const;

	//引数の値を徐々に目標値に近づける．上の関数より更にゆっくり近づく．
	float approachSlowlyTargetValue(const float current, const float target) const;


	ECameraMode m_mode;			//カメラの視点を決定する．

	VECTOR m_goal_upvec;		//カメラの上方向のベクトルの目標値
	VECTOR m_goal_pos;			//カメラの位置の目標値
	VECTOR m_goal_target_pos;	//カメラが注視する目標の座標．

	VECTOR m_now_target_pos;	//カメラが注視する目標の座標．
	VECTOR m_now_camera_upvec;	//カメラの現在の上方向．
	VECTOR m_now_camera_pos;	//カメラの現在の座標．
	float m_length_camera_to_target;	//カメラと目標物との距離．
};


//! @file camera_controller.h
//! @date 2023/08/14
//! @author 長谷川
//! @brief 画像表示処理においてカメラを制御するクラス．
//! @n 行数 : @lineinfo
