//! @file interface_dxlib_draggable.h
//! @brief ドラッグ可能なGUIのインターフェース．


#ifndef	INTERFACE_DXLIB_DRAGGABLE_H_
#define	INTERFACE_DXLIB_DRAGGABLE_H_


//! @brief ドラッグ可能なGUIのインターフェース．
//! @n ドラッグ動作を実装したいGUIはこのインターフェースを継承する．
class IDxlibDraggable
{
public:
	virtual ~IDxlibDraggable() = default;

	//! @brief ドラッグ可能な位置にあるかを判定する．
	//! @param[in] cursor_x マウスカーソルのX座標．左端が0．右方向に正．
	//! @param[in] cursor_y マウスカーソルのY座標．上端が0．下方向に正．
	//! @return GUI状にマウスカーネルがあって，ドラッグ可能であるならば	true．
	virtual bool IsDraggable(int cursor_x, int cursor_y) = 0;

	//! @brief ドラッグ中かどうかを取得する．
	//! @return ドラッグ中ならばtrue．
	virtual bool IsDragged() const = 0;

	//! @brief ドラッグ中かどうかを設定する．
	//! @param[in] is_dragged ドラッグ中にしたいならtrue．
	virtual void SetDragged(bool is_dragged) = 0;

	//! @brief ドラッグ中の処理を行う．
	//! @n カーソルは上にあるけど，ドラッグ中ではない場合でも呼び出される．
	//! @param[in] cursor_dif_x 前回のフレームからのマウスカーソルのX座標の差分．
	//! @param[in] cursor_dif_y 前回のフレームからのマウスカーソルのY座標の差分．
	//! @param[in] mouse_key_bit 押されているマウスのボタン，Dxlibの定数，MOUSE_INPUT_LEFTなどで指定する．
	virtual void DraggedAction(int cursor_dif_x, int cursor_dif_y, unsigned int mouse_key_bit) = 0;
};


#endif	// INTERFACE_DXLIB_DRAGGABLE_H_