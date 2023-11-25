#include "simple_button.h"

#include <algorithm>

#include <Dxlib.h>

#include "cassert_define.h"
#include "designlab_math_util.h"
#include "designlab_string_util.h"
#include "font_loader.h"


namespace dl = ::designlab;
namespace dlm = ::designlab::math_util;
namespace dlsu = ::designlab::string_util;


SimpleButton::SimpleButton(const std::string& text, const int pos_x, const int pos_y, const int size_x, const int size_y, const bool fit_size) :
	text_(dlsu::Split(text, "\n")),
	font_handle_(FontLoader::GetIns()->GetFontHandle("font/Yu_Gothic_UI.dft")),
	pos_middle_x(pos_x),
	pos_middle_y(pos_y),
	kSizeX(fit_size ? GetFitButtonSizeX(size_x) : size_x),
	kSizeY(fit_size ? GetFitButtonSizeX(size_y) : size_y)
{
	assert(0 < kSizeX);
	assert(0 < kSizeY);

	//text_が2つとも同じならば1つにする．おそらくSplit関数のバグ．
	if (text_.size() == 2 && text_[0] == text_[1])
	{
		text_.pop_back();
	}
}

void SimpleButton::SetPos(const int pos_x, const int pos_y, const unsigned int option)
{
	if (option & dl::kOptionLeft) { pos_middle_x = pos_x - kSizeX / 2; }
	else if (option & dl::kOptionMidleX) { pos_middle_x = pos_x; }
	else if (option & dl::kOptionRight) { pos_middle_x = pos_x + kSizeX / 2; }

	if (option & dl::kOptionTop) { pos_middle_y = pos_y - kSizeY / 2; }
	else if (option & dl::kOptionMidleY) { pos_middle_y = pos_y; }
	else if (option & dl::kOptionBottom) { pos_middle_y = pos_y + kSizeY / 2; }
}

void SimpleButton::Update()
{
	//now_color_blue_をtarget_color_blue_に近づける
	now_color_blue_ = dlm::ApproachTarget(now_color_blue_, target_color_blue_, 0.1f);

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

	//ベースを描画
	DrawBox(
		pos_middle_x - kSizeX / 2,
		pos_middle_y - kSizeY / 2,
		pos_middle_x + kSizeX / 2,
		pos_middle_y + kSizeY / 2,
		base_color,
		TRUE
	);

	//その上にボタンを描画
	DrawBox(
		pos_middle_x - kSizeX / 2 + frame_size,
		pos_middle_y - kSizeY / 2 + frame_size,
		pos_middle_x + kSizeX / 2 - frame_size,
		pos_middle_y + kSizeY / 2 - frame_size,
		button_color,
		TRUE
	);

	//テキストを表示
	for (int i = 0; i < text_.size(); ++i)
	{
		DrawStringToHandle(
			pos_middle_x - GetDrawStringWidthToHandle(text_[i].c_str(), (int)text_[i].size(), font_handle_) / 2,
			pos_middle_y - static_cast<int>(text_.size()) * kFontSize / 2 + i * kFontSize,
			text_[i].c_str(),
			str_color,
			font_handle_
		);
	}
}

void SimpleButton::Activate(const std::shared_ptr<const Mouse> mouse_ptr)
{
	target_color_blue_ = 64;

	if (click_function_ && visible_ && mouse_ptr->GetPressingCount(MOUSE_INPUT_LEFT) == 1)
	{
		now_color_blue_ = 128;

		click_function_();
	}
}

bool SimpleButton::OnCursor(int cursor_x, int cursor_y) const noexcept
{
	return (pos_middle_x - kSizeX / 2 < cursor_x && cursor_x < pos_middle_x + kSizeX / 2) &&
		(pos_middle_y - kSizeY / 2 < cursor_y && cursor_y < pos_middle_y + kSizeY / 2);
}

int SimpleButton::GetFitButtonSizeX(int now_size_x) const noexcept
{
	//文字列の中からもっと横幅が大きいものを探す
	int max_width = 0;
	for (const auto& i : text_)
	{
		int width = GetDrawStringWidthToHandle(i.c_str(), (int)i.size(), font_handle_);

		if (max_width < width)
		{
			max_width = width;
		}
	}

	//横幅が大きい文字列に合わせてボタンの横幅を変更する
	if (now_size_x < max_width)
	{
		return max_width;
	}
	else
	{
		return now_size_x;
	}
}

int SimpleButton::GetFitButtonSizeY(int now_size_y) const noexcept
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
