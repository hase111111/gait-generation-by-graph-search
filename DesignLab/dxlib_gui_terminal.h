
//! @file      dxlib_gui_terminal.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_DXLIB_GUI_TERMINAL_H_
#define DESIGNLAB_DXLIB_GUI_TERMINAL_H_

#include <memory>
#include <vector>

#include "interface_dxlib_gui.h"
#include "interface_dxlib_clickable.h"
#include "simple_button.h"


namespace designlab
{

//! @class DxlibGuiTerminal
//! @brief ×ボタンを押して消えたGUIを復活させるためのクラス．
class DxlibGuiTerminal final : public IDxlibGui, public IDxlibClickable
{
public:
    DxlibGuiTerminal() = delete;
    explicit DxlibGuiTerminal(std::vector<std::shared_ptr<IDxlibGui> > gui_list);

    void Update() override;
    void Draw() const override;

    void SetVisible(bool visible);
    bool IsVisible() const;

    void ClickedAction(const DxlibMouseState& state);

    bool CursorOnGui(int cursor_x, int cursor_y) const noexcept;

private:
    static constexpr int kLeftTopX{ 0 };
    static constexpr int kLeftTopY{ 10 };
    static constexpr int kTerminalHeight{ 100 };
    static constexpr int kClosedTerminalWidth{ 30 };

    static constexpr int kButtonSize{ 70 };

    void DrawClosedTerminal() const;
    void DrawTerminal() const;
    void DrawButtonGuard() const;

    const int kTerminalWidth;

    std::vector<std::shared_ptr<IDxlibGui> > gui_list_;
    std::vector<std::shared_ptr<SimpleButton> > button_list_;

    bool is_closed_{ true };
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_GUI_TERMINAL_H_
