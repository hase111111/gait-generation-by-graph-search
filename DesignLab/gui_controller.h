#pragma once

#include <memory>
#include <vector>

#include "button_controller.h"
#include "graphic_const.h"
#include "application_setting_recorder.h"
#include "node.h"


//! @enum ENodeDisplayNode
//! @date 2023/08/09
//! @author 長谷川
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

	GUIController() = delete;
	GUIController(const SApplicationSettingRecorder* const setting);

	void update(const int max_node, int& display_node, const int counter);

	void draw(const SNode& node) const;

private:

	//左側にノードの状態を描画する関数．大変汚い関数です．申し訳ない
	void drawNodeByStr(const SNode node) const;

	//右側にボタンの使い方を描画する関数．同上
	void drawExplanationByStr() const;

	const int BOX_X;
	const int BOX_Y;
	const int CENTER_X;
	const int CENTER_Y;

	const int CHANGE_NEXT_NODE;	//次のノードを再生するまでの時間．

	const SApplicationSettingRecorder* const mp_setting;


	bool m_is_displayed = true;		//UIを表示するかどうか．

	ENodeDisplayNode m_camera_view_mode = ENodeDisplayNode::SELECTABLE;		//どのようにノードを表示するか

	std::vector<std::unique_ptr<ButtomController>> m_buttom;		//ボタンの管理クラス．
};


//! @file gui_controller.h
//! @date 2023/08/09
//! @author 長谷川
//! @brief GUIを表示するクラス．
//! @n 行数 : @lineinfo
