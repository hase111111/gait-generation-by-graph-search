//! @file display_node_switch_gui.h
//! @brief ノードの表示切り替えGUI


#ifndef DESIGNLAB_DISPLAY_NODE_SWITCH_GUI_H_
#define DESIGNLAB_DISPLAY_NODE_SWITCH_GUI_H_

#include <memory>
#include <vector>

#include "button_controller.h"


//! @class DisplayNodeSwitchGui
//! @brief ノードの表示切り替えGUI
class DisplayNodeSwitchGui final
{
public:
	DisplayNodeSwitchGui(int x, int y);
	DisplayNodeSwitchGui();


	//! @brief GUIに表示するノードの情報を設定する
	//! @param[in] node_num 全ノード数
	//! @param[in] simu_end_index シミュレーションの終了ノード番号
	void SetGraphData(size_t node_num, const std::vector<size_t>& simu_end_index);


	//! @brief 現在表示するノードの番号を取得する
	//! @return size_t 現在表示するノードの番号
	size_t GetDisplayNodeNum() const;

	//! @brief 現在表示するシミュレーションの番号を取得する
	//! @return int 現在表示するシミュレーションの番号
	int GetSimulationNum() const;


	//! @brief GUIの更新，毎フレーム実行すること
	void Update();

	//! @brief GUIの描画
	void Draw() const;


	const static int kGuiWidth = 275;	//!< GUIの幅

	const static int kGuiHeight = 250;	//!< GUIの高さ

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



	const int kGuiLeftPosX;		//!< GUIの左上のX座標

	const int kGuiTopPosY;		//!< GUIの左上のY座標

	const int kAnimeSpeedMax;	//!< アニメーション速度の最大値

	const int kAnimeSpeedMin;	//!< アニメーション速度の最小値


	std::vector<std::unique_ptr<SimpleButton>> button_;	//!< ボタン


	size_t all_node_num_;			//!< 全ノード数

	std::vector<size_t> simu_end_index_;	//!< シミュレーションの終了ノード番号

	size_t display_node_num_;		//!< 表示するノードの番号

	int simulation_num_;			//!< 表示するシミュレーションの番号


	bool do_auto_animation_;		//!< 自動再生中かどうか

	int animation_speed_;			//!< 再生速度

	int counter_;					//!< カウンター
};


#endif	// DESIGNLAB_DISPLAY_NODE_SWITCH_GUI_H_