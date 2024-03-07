
//! @file      interface_dxlib_clickable.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_
#define DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_

#include <memory>

#include "mouse.h"


namespace designlab
{

//! @struct DxlibMouseState
//! @brief マウスの状態を表す構造体．
//! @details IDxlibClickable に渡される．
//! パラメータが大量にあるため，構造体にまとめている．
struct DxlibMouseState final
{
    //! マウスカーソルのX座標．左端を0とし，右に正の値をとる．
    int cursor_x{ 0 };

    //! マウスカーソルのY座標．上端を0とし，下に正の値をとる．
    int cursor_y{ 0 };

    //! マウスの左ボタンが押されているフレーム数．
    int left_pushing_count{ 0 };

    //! マウスの中ボタンが押されているフレーム数．
    int middle_pushing_count{ 0 };

    //! マウスの右ボタンが押されているフレーム数．
    int right_pushing_count{ 0 };
};

//! @brief クリック可能なGUIのインターフェース．
//! @details クリック時の動作をGUIに実装したいならばこのインターフェースを継承する．
class IDxlibClickable
{
public:
    virtual ~IDxlibClickable() = default;

    //! @brief GUIがクリックされたときに実行される関数．
    //! @n CursorOnGuiが true を返すときに呼び出される．
    //! 複数のGUIが重なっている場合は，優先度の高いものから順に呼び出される．
    //! @param[in] state マウスの状態．
    virtual void ClickedAction(const DxlibMouseState& state) = 0;

    //! @brief GUIの上にカーソルがあるかどうかを返す．
    //! @param[in] cursor_x マウスカーソルのX座標．左端を0とし，右に正の値をとる．
    //! @param[in] cursor_y マウスカーソルのY座標．上端を0とし，下に正の値をとる．
    //! @return GUIの上にカーソルがあるならば trueを返す．
    [[nodiscard]]
    virtual bool CursorOnGui(int cursor_x, int cursor_y) const noexcept = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_DXLIB_CLICKABLE_H_
