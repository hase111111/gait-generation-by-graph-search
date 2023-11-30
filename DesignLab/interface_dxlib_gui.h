//! @file interface_dxlib_gui.h
//! @brief Dxlibの画面に表示するGUIのインターフェース．

#ifndef DESIGNLAB_INTERFACE_DXLIB_GUI_H_
#define DESIGNLAB_INTERFACE_DXLIB_GUI_H_


namespace designlab
{
	static constexpr unsigned int kOptionLeft = 0b1;
	static constexpr unsigned int kOptionMidleX = 0b10;
	static constexpr unsigned int kOptionRight = 0b100;
	static constexpr unsigned int kOptionTop = 0b1000;
	static constexpr unsigned int kOptionMidleY = 0b10000;
	static constexpr unsigned int kOptionBottom = 0b100000;

	static constexpr unsigned int kOptionLeftTop = kOptionLeft | kOptionTop;
	static constexpr unsigned int kOptionLeftMidleY = kOptionLeft | kOptionMidleY;
	static constexpr unsigned int kOptionLeftBottom = kOptionLeft | kOptionBottom;
	static constexpr unsigned int kOptionMidleXTop = kOptionMidleX | kOptionTop;
	static constexpr unsigned int kOptionMidleXMidleY = kOptionMidleX | kOptionMidleY;
	static constexpr unsigned int kOptionMidleXBottom = kOptionMidleX | kOptionBottom;
	static constexpr unsigned int kOptionRightTop = kOptionRight | kOptionTop;
	static constexpr unsigned int kOptionRightMidleY = kOptionRight | kOptionMidleY;
	static constexpr unsigned int kOptionRightBottom = kOptionRight | kOptionBottom;
}

//! @class IDxlibGui
//! @brief Dxlibの画面に表示するGUIのインターフェース．
class IDxlibGui
{
public:

	virtual ~IDxlibGui() = default;

	//! @brief GUIの更新，毎フレーム実行すること．
	virtual void Update() = 0;

	//! @brief GUIの描画．
	virtual void Draw() const = 0;

	//! @brief GUIの表示を行うかどうかを設定する．
	//! @param[in] visible GUIの表示を行うかどうか．
	virtual void SetVisible(bool visible) = 0;

	//! @brief GUIの表示を行うかどうかを返す．
	//! @return bool GUIの表示を行うかどうか．
	[[nodiscard]] virtual bool IsVisible() const = 0;
};

#endif	// DESIGNLAB_INTERFACE_DXLIB_GUI_H_