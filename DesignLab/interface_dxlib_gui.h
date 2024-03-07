
//! @file      interface_dxlib_gui.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_INTERFACE_DXLIB_GUI_H_
#define DESIGNLAB_INTERFACE_DXLIB_GUI_H_


namespace designlab
{

constexpr unsigned int kDxlibGuiAnchorLeft = 1 << 0;
constexpr unsigned int kDxlibGuiAnchorMiddleX = 1 << 1;
constexpr unsigned int kDxlibGuiAnchorRight = 1 << 2;
constexpr unsigned int kDxlibGuiAnchorTop = 1 << 3;
constexpr unsigned int kDxlibGuiAnchorMiddleY = 1 << 4;
constexpr unsigned int kDxlibGuiAnchorBottom = 1 << 5;

constexpr unsigned int
kDxlibGuiAnchorLeftTop = kDxlibGuiAnchorLeft | kDxlibGuiAnchorTop;

constexpr unsigned int
kDxlibGuiAnchorLeftMiddleY = kDxlibGuiAnchorLeft | kDxlibGuiAnchorMiddleY;

constexpr unsigned int
kDxlibGuiAnchorLeftBottom = kDxlibGuiAnchorLeft | kDxlibGuiAnchorBottom;

constexpr unsigned int
kDxlibGuiAnchorMiddleXTop = kDxlibGuiAnchorMiddleX | kDxlibGuiAnchorTop;

constexpr unsigned int
kDxlibGuiAnchorMiddleXMiddleY = kDxlibGuiAnchorMiddleX | kDxlibGuiAnchorMiddleY;

constexpr unsigned int
kDxlibGuiAnchorMiddleXBottom = kDxlibGuiAnchorMiddleX | kDxlibGuiAnchorBottom;

constexpr unsigned int
kDxlibGuiAnchorRightTop = kDxlibGuiAnchorRight | kDxlibGuiAnchorTop;

constexpr unsigned int
kDxlibGuiAnchorRightMiddleY = kDxlibGuiAnchorRight | kDxlibGuiAnchorMiddleY;

constexpr unsigned int
kDxlibGuiAnchorRightBottom = kDxlibGuiAnchorRight | kDxlibGuiAnchorBottom;


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
    //! @return GUIの表示を行うかどうか．
    [[nodiscard]] virtual bool IsVisible() const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_DXLIB_GUI_H_
