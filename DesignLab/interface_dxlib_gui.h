//! @file interface_dxlib_gui.h
//! @brief Dxlibの画面に表示するGUIのインターフェース．

#ifndef DESIGNLAB_INTERFACE_DXLIB_GUI_H_
#define DESIGNLAB_INTERFACE_DXLIB_GUI_H_


namespace designlab
{

constexpr unsigned int kDxlibGuiAnchorLeft = 1 << 0;
constexpr unsigned int kDxlibGuiAnchorMidleX = 1 << 1;
constexpr unsigned int kDxlibGuiAnchorRight = 1 << 2;
constexpr unsigned int kDxlibGuiAnchorTop = 1 << 3;
constexpr unsigned int kDxlibGuiAnchorMidleY = 1 << 4;
constexpr unsigned int kDxlibGuiAnchorBottom = 1 << 5;

constexpr unsigned int kDxlibGuiAnchorLeftTop = kDxlibGuiAnchorLeft | kDxlibGuiAnchorTop;
constexpr unsigned int kDxlibGuiAnchorLeftMidleY = kDxlibGuiAnchorLeft | kDxlibGuiAnchorMidleY;
constexpr unsigned int kDxlibGuiAnchorLeftBottom = kDxlibGuiAnchorLeft | kDxlibGuiAnchorBottom;
constexpr unsigned int kDxlibGuiAnchorMidleXTop = kDxlibGuiAnchorMidleX | kDxlibGuiAnchorTop;
constexpr unsigned int kDxlibGuiAnchorMidleXMidleY = kDxlibGuiAnchorMidleX | kDxlibGuiAnchorMidleY;
constexpr unsigned int kDxlibGuiAnchorMidleXBottom = kDxlibGuiAnchorMidleX | kDxlibGuiAnchorBottom;
constexpr unsigned int kDxlibGuiAnchorRightTop = kDxlibGuiAnchorRight | kDxlibGuiAnchorTop;
constexpr unsigned int kDxlibGuiAnchorRightMidleY = kDxlibGuiAnchorRight | kDxlibGuiAnchorMidleY;
constexpr unsigned int kDxlibGuiAnchorRightBottom = kDxlibGuiAnchorRight | kDxlibGuiAnchorBottom;


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

}	// namespace designlab


#endif	// DESIGNLAB_INTERFACE_DXLIB_GUI_H_