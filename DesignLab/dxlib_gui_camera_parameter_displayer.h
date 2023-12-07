//! @file dxlib_gui_camera_displayer.h
//! @brief カメラの情報を表示するGUIのクラス


#ifndef DESIGNLAB_DXLIB_GUI_CAMERA_DISPLAYER_H_
#define DESIGNLAB_DXLIB_GUI_CAMERA_DISPLAYER_H_

#include <array>
#include <memory>
#include <vector>

#include "dxlib_camera.h"
#include "interface_dxlib_clickable.h"
#include "interface_dxlib_draggable.h"
#include "interface_dxlib_gui.h"
#include "simple_button.h"


class DxlibGuiCameraParameterDisplayer final : public IDxlibGui, public IDxlibClickable, public IDxlibDraggable
{
public:

	DxlibGuiCameraParameterDisplayer(
		int window_x,
		int window_y,
		const std::shared_ptr<DxlibCamera> camera_ptr
	);

	void SetPos(int pos_x, int pos_y, unsigned int option = ::designlab::kDxlibGuiAnchorLeftTop, bool this_is_first_time = false);

	void Update() override;

	void Draw() const override;

	void SetVisible(bool visible) override;

	bool IsVisible() const override { return visible_; }

	void ClickedAction(int cursor_x, int cursor_y, int left_pushing_count, int middle_pushing_count, int right_pushing_count) override;

	bool CursorOnGui(int cursor_x, int cursor_y) const noexcept override;

	bool IsDraggable(int cursor_x, int cursor_y) const override;

	bool IsDragged() const override { return is_dragging_; };

	void SetDragged(const bool is_dragged) override { is_dragging_ = is_dragged; };

	void DraggedAction(int cursor_dif_x, int cursor_dif_y, unsigned int mouse_key_bit) override;

private:

	static constexpr int kWidth{ 470 };			//!< GUIの幅．
	static constexpr int kHeight{ 340 };		//!< GUIの高さ．
	static constexpr int kTitleBarHeight{ 32 };	//!< タイトルバーの高さ．

	void DrawBackground() const;

	void DrawCameraParameter() const;

	bool IsInWindow() const;

	int gui_left_pos_x_{ 0 };
	int gui_top_pos_y_{ 0 };

	int set_pos_x_{ 0 };	//!< SetされたGUIの左上のX座標
	int set_pos_y_{ 0 };	//!< SetされたGUIの左上のY座標

	const int window_x_;	//!< ウィンドウのX座標
	const int window_y_;	//!< ウィンドウのY座標

	bool is_dragging_{ false };	//!< ドラッグ中かどうか
	bool visible_{ true };		//!< ボタンの表示を行うかどうか．

	const std::shared_ptr<const DxlibCamera> camera_ptr_;

	std::vector<std::unique_ptr<SimpleButton>> button_;	//!< ボタン

	const int kFontSize{ 16 };		//!< フォントのサイズ
	const std::string kFontPath{ "font/Yu_Gothic_UI.dft" };	//!< フォントへのパス
	int font_handle_;	//!< フォントのハンドル
};

#endif	//DESIGNLAB_DXLIB_GUI_CAMERA_DISPLAYER_H_