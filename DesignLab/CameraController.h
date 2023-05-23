#pragma once
#include "DxLib.h"
#include "vectorFunc.h"

//Dxlibの3Dのカメラの処理を行うクラス．
class CameraController final
{
private:
	VECTOR m_target_pos;	//カメラが注視する目標の座標．
	VECTOR m_camera_up;		//カメラの上方向を設定する．
	float m_length_camera_to_target;	//カメラと目標物との距離．
	float m_angle;			//カメラの回転角度

public:
	CameraController();
	~CameraController() = default;

	//カメラの位置などの更新を行う．
	void update();

	//カメラの注視する目標の座標．
	void setTargetPos(const VECTOR _pos);

};
