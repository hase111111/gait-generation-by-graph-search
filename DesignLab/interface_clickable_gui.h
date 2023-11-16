//! @file interface_clickable_gui.h
//! @brief クリック可能なGUIのインターフェース

#ifndef DESIGNLAB_INTERFACE_CLICKABLE_GUI_H_
#define DESIGNLAB_INTERFACE_CLICKABLE_GUI_H_


#include "interface_gui.h"


class IClickableGui : public IGui
{
public:

	IClickableGui() : IGui() {}
	virtual ~IClickableGui() = default;

	//! @brief GUIがクリックされたときに実行される関数．
	virtual void Activate(int cursor_x, int cursor_y) = 0;

	//! @brief GUIがクリックされたかどうかを返す．
	//! @param[in] cursor_x マウスのx座標
	//! @param[in] cursor_y マウスのy座標
	//! @return bool GUIがクリックされたかどうか．
	virtual bool IsCursorInGui(int cursor_x, int cursor_y) const noexcept = 0;
};

#endif	// DESIGNLAB_INTERFACE_CLICKABLE_GUI_H_