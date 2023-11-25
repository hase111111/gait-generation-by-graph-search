//! @file interface_dxlib_clickable.h
//! @brief クリック可能なGUIのインターフェース．

#ifndef DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_
#define DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_

#include <memory>

#include "mouse.h"


class IDxlibClickable
{
public:

	virtual ~IDxlibClickable() = default;

	//! @brief GUIがクリックされたときに実行される関数．
	virtual void Activate(const std::shared_ptr<const Mouse> mouse_ptr) = 0;

	//! @brief GUIの上にカーソルがあるかどうかを返す．
	//! @param [in] cursor_x マウスカーソルのX座標．
	//! @param [in] cursor_y マウスカーソルのY座標．
	//! @return bool GUIの上にカーソルがあるかどうか．
	virtual bool OnCursor(int cursor_x, int cursor_y) const noexcept = 0;
};

#endif	// DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_