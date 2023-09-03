#pragma once

#include <memory>
#include <map>

#include "node.h"
#include "button_controller.h"



//! @class NodeDisplayGUI
//! @date 2023/08/23
//! @author 長谷川
//! @brief ノードの情報を表示するGUI
class NodeDisplayGUI final
{
public:
	NodeDisplayGUI() = default;
	NodeDisplayGUI(const int x_pos, const int y_pos);


	//! @brief 表示するノードを設定する
	//! @param [in] node 表示するノード
	void setDisplayNode(const SNode& node);


	//! @brief GUIのボタンの更新を行う
	void update();

	//! @brief GUIの表示を行う
	void draw() const;


	const static int BOX_SIZE_X;
	const static int BOX_SIZE_Y;
	const static int BOX_SIZE_Y_CLOSED;

private:

	void drawBackground() const;

	void drawNodeInfo() const;


	enum class EButtonType
	{
		OPEN_CLOSE,
		SWITCHING
	};

	enum class EDisplayType
	{
		DEFUALT,	//デフォルト
		LEG_STATE		//角度
	};


	const int kGUILeftPosX = 0;
	const int kGUITopPosY = 0;


	std::map<EButtonType, std::unique_ptr<ButtomController>> m_buttons;	//!< ボタン

	SNode m_node;	//!< 表示するノード

	bool m_is_closed = false;	//!< GUIが閉じているかどうか

	EDisplayType m_display_type = EDisplayType::DEFUALT;	//!< 表示する情報の種類
};


//! @file node_display_gui.h
//! @date 2023/08/23
//! @author 長谷川
//! @brief ノードの情報を表示するGUI