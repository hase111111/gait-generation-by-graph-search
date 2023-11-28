//! @file node_display_gui.h
//! @brief ノードの情報を表示するGUI


#ifndef DESIGNLAB_NODE_DISPLAY_GUI_H_
#define DESIGNLAB_NODE_DISPLAY_GUI_H_

#include <array>
#include <memory>
#include <vector>

#include "simple_button.h"
#include "interface_dxlib_clickable.h"
#include "interface_dxlib_draggable.h"
#include "interface_dxlib_gui.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_vaild_checker.h"
#include "robot_state_node.h"


//! @class NodeDisplayGui
//! @brief ノードの情報を表示するGUI
class NodeDisplayGui final : public IDxlibGui, public IDxlibClickable, public IDxlibDraggable
{
public:

	NodeDisplayGui(
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr
	);

	void SetPos(int pos_x, int pos_y, unsigned int option = designlab::kOptionLeftTop);

	//! @brief 表示するノードを設定する，その後関節の角度を計算し，セットする
	//! @param [in] node 表示するノード
	void SetDisplayNode(const RobotStateNode& node);


	void Update() override;

	void Draw() const override;

	void SetVisible(bool visible) override;

	inline bool IsVisible() const override { return visible_; }

	void ClickedAction(int cursor_x, int cursor_y, int left_pushing_count, int middle_pushing_count, int right_pushing_count) override;

	bool CursorOnGui(int cursor_x, int cursor_y) const noexcept override;

	bool IsDraggable(int cursor_x, int cursor_y) override;

	bool IsDragged() const override { return is_dragging_; };

	void SetDragged(const bool is_dragged) override { is_dragging_ = is_dragged; };

	void DraggedAction(int cursor_dif_x, int cursor_dif_y, unsigned int mouse_key_bit) override;

private:

	enum class DisplayMode : int
	{
		kDefualt,		//!< デフォルト
		kJointState,	//!< 角度
		kGlobalPos,		//!< グローバル座標
	};

	static constexpr int kWidth{ 470 };			//!< GUIの幅
	static constexpr int kHeight{ 680 };		//!< GUIの高さ
	static constexpr int kTitleBarHeight{ 32 };	//!< タイトルバーの高さ

	void DrawBackground() const;

	void DrawNodeInfo() const;

	void DrawJointInfo() const;

	void DrawGlobalPosInfo() const;

	std::vector<std::unique_ptr<SimpleButton> > buttons_;				//!< ボタン．

	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;	//!< 座標変換を行うクラス．
	const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;		//!< 六脚歩行ロボットの状態を計算するクラス．
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;

	RobotStateNode display_node_;										//!< 表示するノード
	std::array<HexapodJointState, HexapodConst::kLegNum> joint_state_;	//!< 関節の角度
	DisplayMode display_type_;	//!< 表示する情報の種類

	int gui_left_pos_x_{ 0 };
	int gui_top_pos_y_{ 0 };
	bool is_dragging_{ false };	//!< ドラッグ中かどうか
	bool visible_{ true };		//!< ボタンの表示を行うかどうか．

	const int kFontSize{ 16 };		//!< フォントのサイズ
	const std::string kFontPath{ "font/Yu_Gothic_UI.dft" };	//!< フォントへのパス
	int font_handle_;	//!< フォントのハンドル
};


#endif // DESIGNLAB_NODE_DISPLAY_GUI_H_