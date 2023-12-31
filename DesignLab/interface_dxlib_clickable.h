
//! @file      interface_dxlib_clickable.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_
#define DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_

#include <memory>

#include "mouse.h"


namespace designlab
{

//! @brief クリック可能なGUIのインターフェース．
//! @details クリック時の動作をGUIに実装したいならばこのインターフェースを継承する．
class IDxlibClickable
{
public:
    virtual ~IDxlibClickable() = default;

    //! @brief GUIがクリックされたときに実行される関数．
    //! @n CursorOnGuiがtrueを返すときに呼び出される．複数のGUIが重なっている場合は，優先度の高いものから順に呼び出される．
    //! @param[in] cursor_x マウスカーソルのX座標．左端を0とし，右に正の値をとる．
    //! @param[in] cursor_y マウスカーソルのY座標．上端を0とし，下に正の値をとる．
    //! @param[in] left_pushing_count マウスの左ボタンが押されているフレーム数．
    //! @param[in] middle_pushing_count マウスの中ボタンが押されているフレーム数．
    //! @param[in] right_pushing_count マウスの右ボタンが押されているフレーム数．
    virtual void ClickedAction(int cursor_x, int cursor_y,
                   int left_pushing_count, int middle_pushing_count, int right_pushing_count) = 0;

    //! @brief GUIの上にカーソルがあるかどうかを返す．
    //! @param[in] cursor_x マウスカーソルのX座標．左端を0とし，右に正の値をとる．
    //! @param[in] cursor_y マウスカーソルのY座標．上端を0とし，下に正の値をとる．
    //! @return GUIの上にカーソルがあるならば trueを返す．
    [[nodiscard]] virtual bool CursorOnGui(int cursor_x, int cursor_y) const noexcept = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_
