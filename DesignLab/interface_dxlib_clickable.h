//! @file interface_dxlib_clickable.h
//! @brief クリック可能なGUIのインターフェース．

#ifndef DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_
#define DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_


class IDxlibClickable
{
public:

	virtual ~IDxlibClickable() = default;

	//! @brief GUIがクリックされたときに実行される関数．
	virtual void Activate() = 0;

	//! @brief GUIの上にカーソルがあるかどうかを返す．
	//! @return bool GUIの上にカーソルがあるかどうか．
	virtual bool OnCursor() const noexcept = 0;
};

#endif	// DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_