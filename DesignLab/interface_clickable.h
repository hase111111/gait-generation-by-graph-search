//! @file interface_clickable.h
//! @brief クリック可能なGUIのインターフェース．

#ifndef DESIGNLAB_INTERFACE_CLICK_HANDLER_H_
#define DESIGNLAB_INTERFACE_CLICK_HANDLER_H_


#include "interface_dxlib_gui.h"


class IClickHandler
{
public:

	virtual ~IClickHandler() = default;

	//! @brief GUIがクリックされたときに実行される関数．
	virtual void Activate() = 0;

	//! @brief GUIがクリックされたかどうかを返す．
	//! @return bool GUIがクリックされたかどうか．
	virtual bool OnCursor() const noexcept = 0;
};

#endif	// DESIGNLAB_INTERFACE_CLICK_HANDLER_H_