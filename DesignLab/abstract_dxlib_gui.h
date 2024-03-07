
//! @file      abstract_dxlib_gui.h
//! @author    hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_ABSTRACT_DXLIB_GUI_H_
#define DESIGNLAB_ABSTRACT_DXLIB_GUI_H_

#include <memory>
#include <string>
#include <vector>

#include "interface_dxlib_clickable.h"
#include "interface_dxlib_draggable.h"
#include "interface_dxlib_gui.h"
#include "simple_button.h"

namespace designlab
{

//! @class AbstractDxlibGui
//! @brief Dxlibを使ったGUIの抽象クラス．
//! @details
//! このクラスは，Dxlibを使ったGUIの抽象クラスである．
//! このクラスを継承して，GUIを作成する．
class AbstractDxlibGui :
    public IDxlibGui,
    public IDxlibClickable,
    public IDxlibDraggable
{
public:
    AbstractDxlibGui() = delete;
    AbstractDxlibGui(int width, int height);
    virtual ~AbstractDxlibGui() = default;

    //! @brief GUIの位置を設定する．
    //! @n Dxlibの画面の座標は左上を原点とし，右下に行くほど値が大きくなる．
    //! @n 横方向にx軸，縦方向にy軸をとる．
    //! @param[in] pos_x GUIのx座標．
    //! @param[in] pos_y GUIのy座標．
    //! @param[in] option GUIのどの地点を起点に座標を設定するかを指定する．
    //! defaultでは左上を起点とする．
    //! @param[in] this_is_first_time この呼び出しが初めてかどうかを指定する．
    //! defaultでは false．trueを指定すると，GUIの位置を設定するだけでなく，
    //! GUIの初期位置を更新する．
    void SetPos(int pos_x, int pos_y,
                unsigned int option = kDxlibGuiAnchorLeftTop,
                bool this_is_first_time = false);

    // IDxlibGui interface

    virtual void Update() = 0;
    virtual void Draw() const = 0;

    void SetVisible(bool visible) override;

    [[nodiscard]] constexpr bool IsVisible() const override { return visible_; }

    // IDxlibClickable interface

    void ClickedAction(const DxlibMouseState& state) override;

    bool CursorOnGui(int cursor_x, int cursor_y) const noexcept override;

    // IDxlibDraggable interface

    bool IsDraggable(int cursor_x, int cursor_y) const override;

    bool IsDragged() const override
    {
        return is_dragging_;
    };

    void SetDragged(const bool is_dragged) override
    {
        is_dragging_ = is_dragged;
    };

    void DraggedAction(
        int cursor_dif_x, int cursor_dif_y, unsigned int mouse_key_bit) override;

protected:
    void DrawBackground(const std::string& str) const;

    std::vector<std::unique_ptr<SimpleButton>> button_;  //!< ボタンのリスト．

    static constexpr int kTitleBarHeight{ 32 };  //!< タイトルバーの高さ．

    const int width_;  //!< GUIの横幅．
    const int height_;  //!< GUIの縦幅．

    //! GUIが表示されているかどうかのフラグ．
    bool visible_{ true };

    //!< ドラッグ中かどうかのフラグ．
    bool is_dragging_{ false };

    int gui_left_pos_x_{ 0 };  //!< GUIの左端の位置．
    int gui_top_pos_y_{ 0 };   //!< GUIの上端の位置．

    int init_pos_x_{ 0 };  //!< SetされたGUIの左上のX座標．
    int init_pos_y_{ 0 };  //!< SetされたGUIの左上のY座標．

    int font_handle_{ -1 };  //!< フォントハンドル．
};

}  // namespace designlab


#endif  // DESIGNLAB_ABSTRACT_DXLIB_GUI_H_
