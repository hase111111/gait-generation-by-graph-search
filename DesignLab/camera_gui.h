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
class CameraGUI final
{
public:

	CameraGUI(const int left_x, const int top_y);
	CameraGUI();

	//! @brief カメラの注視する位置を設定する
	void setHexapodPos(const dl_vec::SVector pos);

	//! @brief GUIやカメラの更新を行う
	void Update();

	//! @brief GUIの描画を行う
	void Draw() const;

	const static int GUI_SIZE_X = 235;
	const static int GUI_SIZE_Y = 410;
	const static int CLOSED_GUI_SIZE_Y = 40;

private:

	//! @brief GUIの背景を描画する
	void drawBackground() const;

	//! @brief 最小化時のGUIの背景を描画する
	void drawClosedBackground() const;

	//! @brief GUIの文字を描画する
	void drawString() const;


	enum class EButtonType : int
	{
		LENGH_RESET,
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		TOP,
		TARGET_RESET,
		CLOSED
	};


	CameraController m_camera_controller;	//!< カメラの操作を行うクラス

	CameraManager m_camera_manager;			//!< カメラの管理を行うクラス

	std::map<EButtonType, std::unique_ptr<ButtomController>> m_buttons;


	const int kButtonDistance = 10; // ボタン同士の間隔

	const int kButtonSize = 60;		// ボタンのサイズ

	const int kGUILeftPosX;			// GUIの左端の位置

	const int kGUITopPosY;			// GUIの上端の位置


	bool m_is_closed = false;
};


//! @file camera_gui.h
//! @date 2023/08/21
//! @auther 長谷川
//! @brief カメラの操作・管理を行うGUIを行うクラス
//! @n 行数 : @lineinfo
