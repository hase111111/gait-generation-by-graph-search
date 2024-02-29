
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "dxlib_gui_terminal.h"

#include <string>
#include <typeinfo>

#include <DxLib.h>

#include "string_util.h"


namespace designlab
{

DxlibGuiTerminal::DxlibGuiTerminal(std::vector<std::shared_ptr<IDxlibGui>> gui_list) :
    kTerminalWidth(static_cast<int>(gui_list.size() + 2)* kButtonSize)
{
    //! @todo 処理をまとめる．

    gui_list_ = gui_list;

    // guiの数だけボタンを作成
    for (int i = 0; i < gui_list_.size(); ++i)
    {
        // 型を取得する．
        std::string class_name = string_util::GetTypeName(*gui_list_[i].get());

        // typeidの返り値は"class DxlibGui～"という文字列になっているので，邪魔な文字を削除する．
        const std::vector<std::string> unnecessary_words = { "DxlibGui", " ", "const", "*", "&" };

        for (const auto& word : unnecessary_words)
        {
            // そもそも文字列に word が含まれていない場合は何もしない
            if (class_name.find(word) == std::string::npos) { continue; }

            // 文字列に word が含まれている場合は削除
            class_name.erase(class_name.find(word), word.size());
        }

        // class_nameを7文字ごとに改行
        for (int j = 7; j < class_name.size(); j += 7)
        {
            class_name.insert(j, "\n");
            ++j;
        }

        button_list_.push_back(
          std::make_shared<SimpleButton>(class_name,
            kLeftTopX + kButtonSize + i * 100, kLeftTopY + kTerminalHeight / 2, kButtonSize, kButtonSize));

        button_list_.back()->SetActivateFunction([this, i]() { gui_list_[i]->SetVisible(true); });

        button_list_.back()->SetVisible(false);
    }
}

void DxlibGuiTerminal::Update()
{
    // 各ボタンの更新．
    for (auto& button : button_list_)
    {
        button->Update();
    }
}

void DxlibGuiTerminal::Draw() const
{
    if (is_closed_)
    {
        DrawClosedTerminal();
    }
    else
    {
        DrawTerminal();

        // ボタンの描画．
        for (const auto& button : button_list_)
        {
            button->Draw();
        }

        DrawButtonGuard();
    }
}

void DxlibGuiTerminal::SetVisible([[maybe_unused]] bool visible)
{
    // 消えない．なので何もしない．
}

bool DxlibGuiTerminal::IsVisible() const
{
    // 消えない．なので常に true．
    return true;
}

void DxlibGuiTerminal::ClickedAction(const DxlibMouseState& state)
{
    bool is_clicked = false;

    // 各ボタンの処理．
    for (auto& button : button_list_)
    {
        if (button->CursorOnGui(state.cursor_x, state.cursor_y))
        {
            button->ClickedAction(state);
            is_clicked = true;
            break;
        }
    }

    // ボタンが押された場合は終了．
    if (is_clicked)
    {
        return;
    }

    // ターミナルの処理．
    if (state.left_pushing_count == 1 && is_closed_)
    {
        is_closed_ = false;

        // ボタンを表示．
        for (auto& button : button_list_)
        {
            button->SetVisible(true);
        }
    }
    else if (state.left_pushing_count == 1 && !is_closed_)
    {
        is_closed_ = true;

        // ボタンを非表示．
        for (auto& button : button_list_)
        {
            button->SetVisible(false);
        }
    }
}

bool DxlibGuiTerminal::CursorOnGui(int cursor_x, int cursor_y) const noexcept
{
    if (is_closed_)
    {
        return cursor_x >= kLeftTopX && cursor_x <= kLeftTopX + kClosedTerminalWidth &&
            cursor_y >= kLeftTopY && cursor_y <= kLeftTopY + kTerminalHeight;
    }
    else
    {
        return cursor_x >= kLeftTopX && cursor_x <= kLeftTopX + kTerminalWidth &&
            cursor_y >= kLeftTopY && cursor_y <= kLeftTopY + kTerminalHeight;
    }
}

void DxlibGuiTerminal::DrawClosedTerminal() const
{
    const int closed_box_width = kClosedTerminalWidth - 20;  // 四角形部分の幅．

    const unsigned int base_color = GetColor(255, 255, 255);
    const unsigned int frame_color = GetColor(30, 30, 30);
    const unsigned int alpha = 200;

    const int frame_width = 1;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    DrawBox(kLeftTopX - frame_width, kLeftTopY - frame_width,
        kLeftTopX + closed_box_width, kLeftTopY + kTerminalHeight + frame_width, frame_color, TRUE);
    DrawTriangleAA(
      kLeftTopX + closed_box_width, kLeftTopY - frame_width,
      kLeftTopX + closed_box_width, kLeftTopY + kTerminalHeight + frame_width,
      kLeftTopX + kClosedTerminalWidth + frame_width, kLeftTopY + kTerminalHeight / 2,
      frame_color, TRUE);

    DrawBox(kLeftTopX, kLeftTopY, kLeftTopX + closed_box_width, kLeftTopY + kTerminalHeight, base_color, TRUE);
    DrawTriangleAA(kLeftTopX + closed_box_width, kLeftTopY, kLeftTopX + closed_box_width, kLeftTopY + kTerminalHeight,
             kLeftTopX + kClosedTerminalWidth, kLeftTopY + kTerminalHeight / 2, base_color, TRUE);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DxlibGuiTerminal::DrawTerminal() const
{
    const int closed_box_width = kTerminalWidth - 20;  // 四角形部分の幅．

    const unsigned int base_color = GetColor(255, 255, 255);
    const unsigned int frame_color = GetColor(30, 30, 30);
    const unsigned int alpha = 200;

    const int frame_width = 1;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    DrawBox(kLeftTopX - frame_width, kLeftTopY - frame_width,
        kLeftTopX + closed_box_width, kLeftTopY + kTerminalHeight + frame_width, frame_color, TRUE);
    DrawTriangleAA(
      static_cast<float>(kLeftTopX + closed_box_width), static_cast<float>(kLeftTopY - frame_width),
      static_cast<float>(kLeftTopX + closed_box_width), static_cast<float>(kLeftTopY + kTerminalHeight + frame_width),
      static_cast<float>(kLeftTopX + kTerminalWidth + frame_width), static_cast<float>(kLeftTopY + kTerminalHeight / 2),
      frame_color, TRUE);

    DrawBox(kLeftTopX, kLeftTopY, kLeftTopX + closed_box_width, kLeftTopY + kTerminalHeight, base_color, TRUE);
    DrawTriangleAA(
      static_cast<float>(kLeftTopX + closed_box_width), static_cast<float>(kLeftTopY),
      static_cast<float>(kLeftTopX + closed_box_width), static_cast<float>(kLeftTopY + kTerminalHeight),
      static_cast<float>(kLeftTopX + kTerminalWidth), static_cast<float>(kLeftTopY + kTerminalHeight / 2),
      base_color, TRUE);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DxlibGuiTerminal::DrawButtonGuard() const
{
    for (int i = 0; i < button_list_.size(); ++i)
    {
        if (gui_list_[i]->IsVisible())
        {
            const unsigned int color = GetColor(45, 45, 45);
            const int alpha = 200;

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

            DrawBox(button_list_[i]->GetPosMiddleX() - kButtonSize / 2, button_list_[i]->GetPosMiddleY() - kButtonSize / 2,
                button_list_[i]->GetPosMiddleX() + kButtonSize / 2, button_list_[i]->GetPosMiddleY() + kButtonSize / 2, color, TRUE);

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}

}  // namespace designlab
