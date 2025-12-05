
//! @file dxlib_gui_terminal.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>
#include <vector>

#include "interface_dxlib_clickable.h"
#include "interface_dxlib_gui.h"
#include "simple_button.h"

namespace gaitgen {

//! @class DxlibGuiTerminal
//! @brief ×ボタンを押して消えたGUIを復活させるためのクラス.
class DxlibGuiTerminal final : public IDxlibGui, public IDxlibClickable {
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
  static constexpr int kLeftTopX{0};
  static constexpr int kLeftTopY{10};
  static constexpr int kTerminalHeight{100};
  static constexpr int kClosedTerminalWidth{30};

  static constexpr int kButtonSize{70};

  void DrawClosedTerminal() const;
  void DrawTerminal() const;
  void DrawButtonGuard() const;

  const int kTerminalWidth;

  std::vector<std::shared_ptr<IDxlibGui> > gui_list_;
  std::vector<std::shared_ptr<SimpleButton> > button_list_;

  bool is_closed_{true};
};

}  // namespace gaitgen
