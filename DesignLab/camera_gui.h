//! @file camera_gui.h
//! @brief カメラの操作・管理を行うGuiの処理，表示を行うクラス


#ifndef DESIGNLAB_CAMERA_GUI_H_
#define DESIGNLAB_CAMERA_GUI_H_


#include <map>
#include <memory>

#include "camera_input_controller.h"
#include "dxlib_camera.h"
#include "simple_button.h"
#include "designlab_vector3.h"


//! @class CameraGui
//! @brief カメラの操作・管理を行うGUIを行うクラス．
//! @details 使用したい場合は，メンバにこいつを持たせて，UpdateとDrawを呼び出すだけでよい．
class CameraGui final
{
public:

	//! @brief GUIを表示する位置を指定しない場合のコンストラクタ，左上 (0, 0) を起点に表示する．
	CameraGui();

	//! @param [in] x_pos GUIを表示するx座標
	//! @param [in] y_pos GUIを表示するy座標
	//! @param [in] option GUIのどの地点を起点に表示位置を指定するか
	CameraGui(int x_pos, int y_pos, unsigned int option);

	//! @brief カメラが注視するロボットの座標を設定する
	//! @param[in] pos ロボットの座標
	void SetHexapodPos(const designlab::Vector3& pos);

	//! @brief GUIやカメラの更新を行う
	void Update();

	//! @brief GUIの描画を行う
	void Draw() const;

	
	constexpr static unsigned int kOptionLeftTop = 0b00;		//!< 左上を起点に座標を設定する
	constexpr static unsigned int kOptionLeftBottom = 0b10;		//!< 左下を起点に座標を設定する
	constexpr static unsigned int kOptionRightTop = 0b01;		//!< 右上を起点に座標を設定する
	constexpr static unsigned int kOptionRightBottom = 0b11;	//!< 右下を起点に座標を設定する

private:

	//! @brief GUIの背景を描画する
	void DrawBackground() const;

	//! @brief 最小化時のGUIの背景を描画する
	void DrawClosedBackground() const;

	//! @brief GUIの文字を描画する
	void DrawString() const;


	const int kGuiSizeX;		//!< GUIの横幅
	const int kGuiSizeY;		//!< GUIの縦幅
	const int kGuiSizeYClosed;	//!< GUIの縦幅(最小化時)

	const int kGuiLeftPosX;		//!< GUIの左端の位置
	const int kGuiTopPosY;		//!< GUIの上端の位置

	const int kButtonDistance;	//!< ボタン同士の間隔
	const int kButtonSize;		//!< ボタンのサイズ


	CameraInputController camera_controller_;	//!< カメラの操作を行うクラス
	
	DxlibCamera camera_manager_;			//!< カメラの管理を行うクラス

	std::vector<std::unique_ptr<SimpleButton>> button_list_;	//!< ボタンのリスト


	bool is_closed_;
};


#endif	// DESIGNLAB_CAMERA_GUI_H_