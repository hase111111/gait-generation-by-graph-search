#pragma once

#include <vector>
#include <map>
#include <memory>

#include "button_controller.h"


//! @class DisplayNodeSwitchGUI
//! @date 2023/08/30
//! @author 長谷川
//! @brief ノードの表示切り替えGUI
class DisplayNodeSwitchGUI final
{
public:
	DisplayNodeSwitchGUI(const int x, const int y);
	DisplayNodeSwitchGUI();


	//! @brief GUIに表示するノードの情報を設定する
	//! @param[in] node_num 全ノード数
	//! @param[in] simu_end_index シミュレーションの終了ノード番号
	void setGraphData(const size_t node_num, const std::vector<size_t>& simu_end_index);


	//! @brief 現在表示するノードの番号を取得する
	//! @return size_t 現在表示するノードの番号
	size_t getDisplayNodeNum() const;

	//! @brief 現在表示するシミュレーションの番号を取得する
	//! @return int 現在表示するシミュレーションの番号
	int getSimulationNum() const;


	//! @brief GUIの更新，毎フレーム実行すること
	void Update();

	//! @brief GUIの描画
	void Draw() const;


	const static int GUI_WIDTH = 275;	//!< GUIの幅

	const static int GUI_HEIGHT = 250;	//!< GUIの高さ

private:

	//! @brief ボタンの種類
	enum class EButtonType : int
	{
		PrevNode,		//!< 前のノード
		NextNode,		//!< 次のノード
		MostPrevNode,	//!< 最も前のノードへ
		MostNextNode,	//!< 最も後ろのノードへ
		PrevSimu,		//!< 前のシミュレーション
		NextSimu,		//!< 次のシミュレーション
		PlayStop,		//!< 再生/停止
		SpeedUp,		//!< 速度アップ
		SpeedDown,		//!< 速度ダウン
	};


	// シミュレーションの中で最初のノードに移動する関数
	void moveMostPrevNode();

	// シミュレーションの中で前のノードに移動する関数
	void movePrevNode();

	// シミュレーションの中で最後のノードに移動する関数
	void moveMostNextNode();

	// シミュレーションの中で次のノードに移動する関数
	void moveNextNode();

	//前のシミュレーションに移動する関数
	void movePrevSimulation();

	//次のシミュレーションに移動する関数
	void moveNextSimulation();

	//全シミュレーション数を求める
	int getAllSimulationNum() const;



	const int kGUILeftPosX;		//!< GUIの左上のX座標

	const int kGUITopPosY;		//!< GUIの左上のY座標

	const int kAnimeSpeedMax = 120;	//!< アニメーション速度の最大値

	const int kAnimeSpeedMin = 1;	//!< アニメーション速度の最小値


	std::map<EButtonType, std::unique_ptr<ButtomController>> m_button;	//!< ボタン


	size_t m_all_node_num;			//!< 全ノード数

	std::vector<size_t> m_simu_end_index;	//!< シミュレーションの終了ノード番号

	size_t m_display_node_num;		//!< 表示するノードの番号

	int m_simulation_num;			//!< 表示するシミュレーションの番号


	bool m_do_auto_animation;		//!< 自動再生中かどうか

	int m_animation_speed;			//!< 再生速度

	int m_counter;					//!< カウンター
};



//! @file display_node_switch_gui.h
//! @date 2023/08/30
//! @author 長谷川
//! @brief ノードの表示切り替えGUI
//! @n 行数 : @lineinfo
