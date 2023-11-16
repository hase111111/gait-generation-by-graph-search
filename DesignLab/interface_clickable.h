//! @file interface_clickable.h
//! @brief クリック可能なGUIのインターフェース．

#ifndef DESIGNLAB_INTERFACE_CLICKABLE_H_
#define DESIGNLAB_INTERFACE_CLICKABLE_H_


#include "interface_dxlib_gui.h"


class IClickable
{
public:

	virtual ~IClickable() = default;

	//! @brief GUIがクリックされたときに実行される関数．
	virtual void Activate() = 0;

	//! @brief GUIがクリックされたかどうかを返す．
	//! @return bool GUIがクリックされたかどうか．
	virtual bool IsCursorInGui() const noexcept = 0;
};

#endif	// DESIGNLAB_INTERFACE_CLICKABLE_H_