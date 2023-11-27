//! @file camera_gui.h
//! @brief カメラの操作・管理を行うGuiの処理，表示を行うクラス


#ifndef DESIGNLAB_CAMERA_GUI_H_
#define DESIGNLAB_CAMERA_GUI_H_


#include <map>
#include <memory>

#include "dxlib_camera.h"
#include "simple_button.h"
#include "designlab_vector3.h"


//! @class CameraGui
//! @brief カメラの操作・管理を行うGUIを行うクラス．
//! @details 使用したい場合は，メンバにこいつを持たせて，UpdateとDrawを呼び出すだけでよい．
class CameraGui final : public IDxlibGui, public IDxlibClickable
{
public:

	//! @brief GUIを表示する位置を指定しない場合のコンストラクタ，左上 (0, 0) を起点に表示する．
	CameraGui(const std::shared_ptr<DxlibCamera> camera);

	void SetPos(int pos_x, int pos_y, unsigned int option = designlab::kOptionLeftTop);

	//! @brief カメラが注視するロボットの座標を設定する
	//! @param[in] pos ロボットの座標
	void SetHexapodPos(const designlab::Vector3& pos);

	void Update() override;

	void Draw() const override;

	void SetVisible(bool visible) override;

	bool IsVisible() const override { return visible_; }

	void ClickedAction(int cursor_x, int cursor_y, int left_pushing_count, int middle_pushing_count, int right_pushing_count) override;

	bool CursorOnGui(int cursor_x, int cursor_y) const noexcept override;

private:

	//! @brief GUIの背景を描画する
	void DrawBackground() const;

	//! @brief GUIの文字を描画する
	void DrawString() const;


	const int kWidth{ 235 };		//!< GUIの横幅
	const int kHeight{ 410 };		//!< GUIの縦幅

	int gui_left_pos_x_{ 0 };	//!< GUIの左端の位置
	int gui_top_pos_y_{ 0 };		//!< GUIの上端の位置
	
	const std::shared_ptr<DxlibCamera> camera_;			//!< カメラの管理を行うクラス

	std::vector<std::unique_ptr<SimpleButton>> button_;	//!< ボタンのリスト

	bool visible_{ true };
};


#endif	// DESIGNLAB_CAMERA_GUI_H_