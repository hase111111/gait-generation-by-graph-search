#pragma once

#include <memory>
#include <vector>

#include "button_controller.h"
#include "graphic_const.h"
#include "Node.h"
#include "CameraController.h"


//! @enum ENodeDisplayNode
//! @date 2023/08/09
//! @auther 長谷川
//! @brief ノードの表示方法を表す列挙型．
enum class ENodeDisplayNode : int
{
	AUTO_UPDATE,
	ALWAYS_NEW,
	SELECTABLE
};


//! @class GUIController
//! @date 2023/08/09
//! @author 長谷川 
//! @brief UIを表示する関数．表示するものを変更したい場合は編集してください．
class GUIController
{
public:
	GUIController();

	void update(CameraController& camera, const int max_node, int& display_node, const int counter);

	void draw(const SNode& node) const;

private:

	//左側にノードの状態を描画する関数．大変汚い関数です．申し訳ない
	void drawNodeByStr(const SNode node) const;

	//右側にボタンの使い方を描画する関数．同上
	void drawExplanationByStr() const;

	const int BOX_X = 300;
	const int BOX_Y = GraphicConst::WIN_Y - 25 * 2;
	const int CENTER_X = 25 + BOX_X / 2;
	const int CENTER_Y = GraphicConst::WIN_Y / 2;

	const int CHANGE_NEXT_NODE = GraphicConst::GRAPHIC_FPS / 5;	//次のノードを再生するまでの時間．

	bool m_is_displayed = true;		//UIを表示するかどうか．

	ENodeDisplayNode m_mode = ENodeDisplayNode::SELECTABLE;		//どのようにノードを表示するか

	// GUIに表示するボタンを管理するクラス
	std::vector<std::unique_ptr<ButtomController>> m_buttom;
};


//! @file gui_controller.h
//! @date 2023/08/09
//! @auther 長谷川
//! @brief GUIを表示するクラス．
//! @n 行数 : @lineinfo
