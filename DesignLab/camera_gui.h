#pragma once

#include <map>
#include <memory>

#include "camera_controller.h"
#include "camera_manager.h"
#include "button_controller.h"
#include "designlab_vector.h"


//! @class CameraGUI
//! @date 2023/08/21
//! @author 長谷川
//! @brief カメラの操作・管理を行うGUIを行うクラス
class CameraGUI
{
public:

	CameraGUI();

	//! @brief カメラの注視する位置を設定する
	void setHexapodPos(const dl_vec::SVector pos);

	//! @brief GUIやカメラの更新を行う
	void update();

	//! @brief GUIの描画を行う
	void draw() const;

private:

	//! @brief GUIの背景を描画する
	void drawBackground() const;

	//! @brief GUIの文字を描画する
	void drawString() const;


	enum class EButtonType : int
	{
		LENGH_RESET,
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		TOP
	};


	CameraController m_camera_controller;

	CameraManager m_camera_manager;

	std::map<EButtonType, std::unique_ptr<ButtomController>> m_buttons;


	const int kButtonDistance = 10; // ボタン同士の間隔

	const int kButtonSize = 50;		// ボタンのサイズ

	const int kGUILeftPosX = 10;		// GUIの左端の位置

	const int kGUITopPosY = 10;		// GUIの上端の位置
};


//! @file camera_gui.h
//! @date 2023/08/21
//! @auther 長谷川
//! @brief カメラの操作・管理を行うGUIを行うクラス
//! @n 行数 : @lineinfo
