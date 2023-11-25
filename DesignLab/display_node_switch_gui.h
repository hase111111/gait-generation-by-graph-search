//! @file display_node_switch_gui.h
//! @brief ノードの表示切り替えGUI


#ifndef DESIGNLAB_DISPLAY_NODE_SWITCH_GUI_H_
#define DESIGNLAB_DISPLAY_NODE_SWITCH_GUI_H_

#include <memory>
#include <vector>

#include "interface_dxlib_clickable.h"
#include "interface_dxlib_gui.h"
#include "simple_button.h"


//! @class DisplayNodeSwitchGui
//! @brief ノードの表示切り替えGUI
class DisplayNodeSwitchGui final : public IDxlibGui, public IDxlibClickable
{
public:

	DisplayNodeSwitchGui();


	void SetPos(int pos_x, int pos_y, unsigned int option = designlab::kOptionLeftTop);

	//! @brief GUIに表示するノードの情報を設定する
	//! @param[in] node_num 全ノード数
	//! @param[in] simu_end_index シミュレーションの終了ノード番号
	void SetGraphData(size_t node_num, const std::vector<size_t>& simu_end_index);

	//! @brief 現在表示するノードの番号を取得する
	//! @return size_t 現在表示するノードの番号
	size_t GetDisplayNodeNum() const;

	//! @brief 現在表示するシミュレーションの番号を取得する
	//! @return int 現在表示するシミュレーションの番号
	constexpr int GetSimulationNum() const { return simulation_num_; }


	void Update() override;

	void Draw() const override;

	void SetVisible(bool visible) override;

	bool IsVisible() const override { return visible_; }

	void Activate(const std::shared_ptr<const Mouse> mouse_ptr) override;

	bool OnCursor(int cursor_x, int cursor_y) const noexcept override;

private:

	//! シミュレーションの中で最初のノードに移動する関数
	void MoveMostPrevNode();

	//! シミュレーションの中で前のノードに移動する関数
	void MovePrevNode();

	//! シミュレーションの中で最後のノードに移動する関数
	void MoveMostNextNode();

	//! シミュレーションの中で次のノードに移動する関数
	void MoveNextNode();

	//! 前のシミュレーションに移動する関数
	void MovePrevSimulation();

	//! 次のシミュレーションに移動する関数
	void MoveNextSimulation();

	//! 全シミュレーション数を求める
	int GetAllSimulationNum() const;


	static constexpr int kWidth{ 275 };			//!< GUIの幅
	static constexpr int kHeight{ 282 };		//!< GUIの高さ
	static constexpr int kTitleBarHeight{ 32 };	//!< タイトルバーの高さ

	const int kAnimeSpeedMax{ 120 };	//!< アニメーション速度の最大値
	const int kAnimeSpeedMin{ 1 };		//!< アニメーション速度の最小値

	int gui_left_pos_x_{ 0 };		//!< GUIの左上のX座標
	int gui_top_pos_y_{ 0 };		//!< GUIの左上のY座標

	std::vector<std::unique_ptr<SimpleButton>> button_;	//!< ボタン


	size_t all_node_num_;			//!< 全ノード数

	std::vector<size_t> simu_end_index_;	//!< シミュレーションの終了ノード番号

	size_t display_node_num_;		//!< 表示するノードの番号

	int simulation_num_;			//!< 表示するシミュレーションの番号


	bool do_auto_animation_;		//!< 自動再生中かどうか

	int animation_speed_;			//!< 再生速度

	int counter_{ 0 };				//!< カウンター


	bool is_dragging_{ false };		//!< ドラッグ中か．

	int mouse_cursor_dif_x_{ 0 };	//!< マウスカーソルのX座標
	int mouse_cursor_dif_y_{ 0 };	//!< マウスカーソルのY座標

	bool visible_{ true };			//!< GUIを表示するか．

	const int kFontSize{ 16 };		//!< フォントのサイズ
	const std::string kFontPath{ "font/Yu_Gothic_UI.dft" };	//!< フォントへのパス

	const int font_handle_;	//!< フォントのハンドル
};


#endif	// DESIGNLAB_DISPLAY_NODE_SWITCH_GUI_H_