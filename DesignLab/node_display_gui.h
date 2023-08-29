#pragma once

#include "node.h"


//! @class NodeDisplayGUI
//! @date 2023/08/23
//! @author 長谷川
//! @brief ノードの情報を表示するGUI
class NodeDisplayGUI
{
public:
	NodeDisplayGUI() = default;
	NodeDisplayGUI(const int x_pos, const int y_pos);


	//! @brief 表示するノードを設定する
	//! @param [in] node 表示するノード
	void setDisplayNode(const SNode& node);

	//! @brief GUIの表示を行う
	void draw() const;

	const static int BOX_SIZE_X;
	const static int BOX_SIZE_Y;

private:

	const int kGUILeftPosX = 0;
	const int kGUITopPosY = 0;


	SNode m_node;	//!< 表示するノード
};


//! @file node_display_gui.h
//! @date 2023/08/23
//! @author 長谷川
//! @brief ノードの情報を表示するGUI