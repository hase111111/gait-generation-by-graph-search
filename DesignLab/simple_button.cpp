
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "simple_button.h"

#include <algorithm>

#include <Dxlib.h>

#include "cassert_define.h"
#include "math_util.h"
#include "string_util.h"
#include "font_loader.h"


namespace designlab
{

SimpleButton::SimpleButton(const std::string& text,
                           const int pos_x, const int pos_y, const int size_x, const int size_y,
                           const bool fit_size) :
    text_(string_util::Split(text, "\n")),
    pos_middle_x(pos_x),
    pos_middle_y(pos_y),
    kSizeX(fit_size ? GetFitButtonSizeX(size_x) : size_x),
    kSizeY(fit_size ? GetFitButtonSizeX(size_y) : size_y)
{
    assert(0 < kSizeX);
    assert(0 < kSizeY);
}

void SimpleButton::SetPos(const int pos_x, const int pos_y, const unsigned int option)
{
    if (option & kDxlibGuiAnchorLeft)
    {
        pos_middle_x = pos_x - kSizeX / 2;
    }
    else if (option & kDxlibGuiAnchorMiddleX)
    {
        pos_middle_x = pos_x;
    }
    else if (option & kDxlibGuiAnchorRight)
    {
        pos_middle_x = pos_x + kSizeX / 2;
    }

    if (option & kDxlibGuiAnchorTop)
    {
        pos_middle_y = pos_y - kSizeY / 2;
    }
    else if (option & kDxlibGuiAnchorMiddleY)
    {
        pos_middle_y = pos_y;
    }
    else if (option & kDxlibGuiAnchorBottom)
    {
        pos_middle_y = pos_y + kSizeY / 2;
    }
}

void SimpleButton::Update()
{
    // now_color_blue_を target_color_blue_ に近づける．
    now_color_blue_ = math_util::ApproachTarget(now_color_blue_, target_color_blue_, 0.1f);

    target_color_blue_ -= 4;
    target_color_blue_ = target_color_blue_ < 0 ? 0 : target_color_blue_;
}

void SimpleButton::Draw() const
{
    if (!visible_) { return; }

    const int base_color = GetColor(128, 128, 128);
    const int button_color = GetColor(255 - now_color_blue_, 255 - now_color_blue_ / 2, 255);
    const int str_color = GetColor(20, 20, 20);
    const int frame_size = 1;

    // ベースを描画．
    DrawBox(
      pos_middle_x - kSizeX / 2,
      pos_middle_y - kSizeY / 2,
      pos_middle_x + kSizeX / 2,
      pos_middle_y + kSizeY / 2,
      base_color,
      TRUE);

    // その上にボタンを描画．
    DrawBox(
      pos_middle_x - kSizeX / 2 + frame_size,
      pos_middle_y - kSizeY / 2 + frame_size,
      pos_middle_x + kSizeX / 2 - frame_size,
      pos_middle_y + kSizeY / 2 - frame_size,
      button_color,
      TRUE);

    // テキストを表示．
    const int font_handle = FontLoader::GetIns()->GetFontHandle("font/Yu_Gothic_UI.dft");

    for (int i = 0; i < text_.size(); ++i)
    {
        DrawStringToHandle(
            pos_middle_x -
                GetDrawStringWidthToHandle(text_[i].c_str(), static_cast<int>(text_[i].size()), font_handle) / 2,
            pos_middle_y - static_cast<int>(text_.size()) * kFontSize / 2 + i * kFontSize,
            text_[i].c_str(),
            str_color,
            font_handle);
    }
}

void SimpleButton::ClickedAction(
    [[maybe_unused]] const int cursor_x, [[maybe_unused]] const int cursor_y,
    const int left_pushing_count, [[maybe_unused]] const int middle_pushing_count,
    [[maybe_unused]] const int right_pushing_count)
{
    target_color_blue_ = 64;

    if (click_function_ && visible_ && left_pushing_count == 1)
    {
        now_color_blue_ = 128;

        click_function_();
    }
}

bool SimpleButton::CursorOnGui(const int cursor_x, const int cursor_y) const noexcept
{
    return (pos_middle_x - kSizeX / 2 < cursor_x && cursor_x < pos_middle_x + kSizeX / 2) &&
        (pos_middle_y - kSizeY / 2 < cursor_y && cursor_y < pos_middle_y + kSizeY / 2);
}

int SimpleButton::GetFitButtonSizeX(const int now_size_x) const noexcept
{
    // 文字列の中からもっと横幅が大きいものを探す．
    const int font_handle = FontLoader::GetIns()->GetFontHandle("font/Yu_Gothic_UI.dft");

    int max_width = 0;
    for (const auto& i : text_)
    {
        int width = GetDrawStringWidthToHandle(i.c_str(), static_cast<int>(i.size()), font_handle);

        if (max_width < width)
        {
            max_width = width;
        }
    }

    // 横幅が大きい文字列に合わせてボタンの横幅を変更する．
    if (now_size_x < max_width)
    {
        return max_width;
    }
    else
    {
        return now_size_x;
    }
}

int SimpleButton::GetFitButtonSizeY(const int now_size_y) const noexcept
{
    int height = static_cast<int>(text_.size()) * kFontSize;

    if (now_size_y < height)
    {
        return height;
    }
    else
    {
        return now_size_y;
    }
}

}  // namespace designlab
