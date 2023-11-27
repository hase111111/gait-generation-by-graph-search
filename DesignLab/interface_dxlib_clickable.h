//! @file interface_dxlib_clickable.h
//! @brief クリック可能なGUIのインターフェース．

#ifndef DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_
#define DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_

#include <memory>

#include "mouse.h"


//! @brief クリック可能なGUIのインターフェース．
//! @n クリック時の動作をGUIに実装したいならばこのインターフェースを継承する．
class IDxlibClickable
{
public:

	virtual ~IDxlibClickable() = default;

	//! @brief GUIがクリックされたときに実行される関数．
	//! @param [in] cursor_x マウスカーソルのX座標．左端を0とし，右に正の値をとる．
	//! @param [in] cursor_y マウスカーソルのY座標．上端を0とし，下に正の値をとる．
	//! @param [in] left_pushing_count マウスの左ボタンが押されているフレーム数．
	//! @param [in] middle_pushing_count マウスの中ボタンが押されているフレーム数．
	//! @param [in] right_pushing_count マウスの右ボタンが押されているフレーム数．
	virtual void ClickedAction(int cursor_x, int cursor_y,
		int left_pushing_count, int middle_pushing_count, int right_pushing_count) = 0;

	//! @brief GUIの上にカーソルがあるかどうかを返す．
	//! @param [in] cursor_x マウスカーソルのX座標．左端を0とし，右に正の値をとる．
	//! @param [in] cursor_y マウスカーソルのY座標．上端を0とし，下に正の値をとる．
	//! @return bool GUIの上にカーソルがあるならばtrueを返す．
	[[nodiscard]] virtual bool CursorOnGui(int cursor_x, int cursor_y) const noexcept = 0;
};


#endif	// DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_