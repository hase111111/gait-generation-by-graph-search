//! @file node_display_gui.h
//! @brief ノードの情報を表示するGUI


#ifndef DESIGNLAB_NODE_DISPLAY_GUI_H_
#define DESIGNLAB_NODE_DISPLAY_GUI_H_

#include <array>
#include <memory>
#include <vector>

#include "simple_button.h"
#include "interface_dxlib_clickable.h"
#include "interface_dxlib_gui.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_vaild_checker.h"
#include "robot_state_node.h"


//! @class NodeDisplayGui
//! @brief ノードの情報を表示するGUI
class NodeDisplayGui final : public IDxlibGui, public IDxlibClickable
{
public:

	NodeDisplayGui(
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

	void Activate(const std::shared_ptr<const Mouse> mouse_ptr) override;

	bool OnCursor(int cursor_x, int cursor_y) const noexcept override;

private:

	static constexpr int kWidth{ 470 };			//!< GUIの幅
	static constexpr int kHeight{ 600 };		//!< GUIの高さ
	static constexpr int kClosedHeight{ 50 };	//!< GUIが閉じているときの高さ

	enum class DisplayMode : int
	{
		kDefualt,		//!< デフォルト
		kJointState,	//!< 角度
		kGlobalPos,		//!< グローバル座標
		kChecker,		//!< 正しい状態かチェックするモード
	};


	void DrawBackground() const;

	void DrawNodeInfo() const;

	void DrawJointInfo() const;

	void DrawGlobalPosInfo() const;


	int gui_left_pos_x_{ 0 };

	int gui_top_pos_y_{ 0 };


	std::vector<std::unique_ptr<SimpleButton> > buttons_;				//!< ボタン

	const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;	//!< 六脚歩行ロボットの状態を計算するクラス
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;


	RobotStateNode display_node_;										//!< 表示するノード

	std::array<HexapodJointState, HexapodConst::kLegNum> joint_state_;	//!< 関節の角度


	bool is_dragging_{ false };		//!< ドラッグ中かどうか

	bool is_closed_;			//!< GUIが閉じているか(最小化しているか)どうか

	bool visible_{ true };	//!< ボタンの表示を行うかどうか．

	int mouse_cursor_dif_x_{ 0 };	//!< マウスカーソルのX座標
	int mouse_cursor_dif_y_{ 0 };	//!< マウスカーソルのY座標

	DisplayMode display_type_;	//!< 表示する情報の種類

	const int kFontSize{ 16 };		//!< フォントのサイズ
	const std::string kFontPath{ "font/Yu_Gothic_UI.dft" };	//!< フォントへのパス

	int font_handle_;	//!< フォントのハンドル
};


#endif // DESIGNLAB_NODE_DISPLAY_GUI_H_