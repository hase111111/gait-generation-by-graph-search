#pragma once
#include "DxLib.h"
#include "vectorFunc.h"
#include "Define.h"

//カメラの視点を表現する列挙子
enum class ECameraMode : int
{
	TopView,
	OverheadView,
	SideView
};


//Dxlibの3Dのカメラの処理を行うクラス．
class CameraController final
{
private:
	ECameraMode m_mode;			//カメラの視点を決定する．

	VECTOR m_goal_upvec;	//カメラの上方向のベクトルの目標値
	VECTOR m_goal_pos;		//カメラの位置の目標値
	VECTOR m_goal_target_pos;		//カメラが注視する目標の座標．

	VECTOR m_now_target_pos;		//カメラが注視する目標の座標．
	VECTOR m_now_camera_upvec;	//カメラの現在の上方向．
	VECTOR m_now_camera_pos;	//カメラの現在の座標．
	float m_length_camera_to_target;	//カメラと目標物との距離．

public:

	CameraController();
	~CameraController() = default;

	//カメラの位置などの更新を行う．
	void update();

	//カメラの注視する目標の座標．
	inline void setTargetPos(const VECTOR _pos) { m_goal_target_pos = _pos; }

	//カメラのモードをセットする．
	inline void setCameraMode(const ECameraMode _mode) { m_mode = _mode; }

private:
	const float CAMERA_SLOW_SPEED = 0.02f;
	const float CAMERA_SPEED = 3.0f;
	const float CAMERA_HIGH_SPEED = 25.0f;
	const float CAMERA_HIGH_DIF = 150.0f;
	const float CAMERA_ANGLE = Define::MY_PI / 4;	//斜めからのカメラの角度

	void setGoalCameraPos();

	//カメラを目標値に徐々に近づけるのに使用する．戻り値は移動後のVECTOR
	VECTOR approachTargetVECTOR(const VECTOR _current, const VECTOR _target) const;

	//カメラを目標値に徐々に近づけるのに使用する．上の関数より更にゆっくり近づく．
	VECTOR approachSlowlyTargetVECTOR(const VECTOR _current, const VECTOR _target) const;

	//引数の値を徐々に目標値に近づける．
	float approachTargetValue(const float _current, const float _target) const;

	//引数の値を徐々に目標値に近づける．上の関数より更にゆっくり近づく．
	float approachSlowlyTargetValue(const float _current, const float _target) const;
};
