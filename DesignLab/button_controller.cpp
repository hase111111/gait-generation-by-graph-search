#include "button_controller.h"

#include <Dxlib.h>

#include "cassert_define.h"
#include "designlab_string_util.h"
#include "font_loader.h"
#include "mouse.h"


namespace dlsu = ::designlab::string_util;


SimpleButton::SimpleButton(const std::string& text, const int pos_x, const int pos_y, const int size_x, const int size_y, const bool fit_size) :
	text_(dlsu::Split(text, "\n")),
	font_handle_(FontLoader::GetIns()->GetFontHandle("font/Yu_Gothic_UI.dft")),
	kXPos(fit_size ? GetFitButtonSizeX(pos_x) : pos_x),
	kYPos(fit_size ? GetFitButtonSizeY(pos_y) : pos_y),
	kSizeX(size_x),
	kSizeY(size_y)
{
	assert(0 < kSizeX);
	assert(0 < kSizeY);

	//text_が2つとも同じならば1つにする
	if (text_.size() == 2 && text_[0] == text_[1])
	{
		text_.pop_back();
	}
}

void SimpleButton::SetActivateFunction(std::function<void()> func)
{
	click_function_ = func;
}

void SimpleButton::Update()
{
}

void SimpleButton::Draw() const
{
	const int kBaseColor = GetColor(20, 20, 20);
	const int kButtomColor = GetColor(255, 255, 255);
	const int kStrColor = GetColor(20, 20, 20);
	const int kFrameSize = 3;

	//ベースを描画
	DrawBox(kXPos - kSizeX / 2, kYPos - kSizeY / 2, kXPos + kSizeX / 2, kYPos + kSizeY / 2, kBaseColor, TRUE);

	//その上にボタンを描画
	DrawBox(kXPos - kSizeX / 2 + kFrameSize, kYPos - kSizeY / 2 + kFrameSize, kXPos + kSizeX / 2 - kFrameSize, kYPos + kSizeY / 2 - kFrameSize, kButtomColor, TRUE);

	//テキストを表示
	for (int i = 0; i < text_.size(); ++i)
	{
		DrawStringToHandle(
			kXPos - GetDrawStringWidthToHandle(text_[i].c_str(), (int)text_[i].size(), font_handle_) / 2,
			kYPos - static_cast<int>(text_.size()) * kFontSize / 2 + i * kFontSize,
			text_[i].c_str(),
			kStrColor,
			font_handle_
		);
	}
}

void SimpleButton::Activate([[maybe_unused]] const int cursor_x, [[maybe_unused]] const int cursor_y)
{
	if (click_function_)
	{
		click_function_();
	}
}

bool SimpleButton::IsCursorInGui(const int cursor_x, const int cursor_y) const noexcept
{
	return (kXPos - kSizeX / 2 < cursor_x && cursor_x < kXPos + kSizeX / 2) && 
		(kYPos - kSizeY / 2 < cursor_y && cursor_y < kYPos + kSizeY / 2);
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
