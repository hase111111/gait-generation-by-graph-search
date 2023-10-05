#include "button_controller.h"

#include <Dxlib.h>

#include "mouse.h"


ButtomController::ButtomController() : ButtomController{0,0,100,50,""}
{
}


ButtomController::ButtomController(const int _xpos, const int _ypos, const int _xsize, const int _ysize, const std::string& _text) : 
	kXPos(_xpos), 
	kYPos(_ypos), 
	kXSize(_xsize), 
	kYSize(_ysize), 
	is_mouse_in_button_(false),
	is_pushed_(false),
	pushing_frame_(0),
	text_(_text)
{
}


void ButtomController::Update()
{
	//マウスカーソルがボタン内にあるかどうか調べる．
	is_mouse_in_button_ = false;

	if (kXPos - kXSize / 2 < Mouse::GetIns()->GetCursorPosX() && Mouse::GetIns()->GetCursorPosX() < kXPos + kXSize / 2)
	{
		if (kYPos - kYSize / 2 < Mouse::GetIns()->GetCursorPosY() && Mouse::GetIns()->GetCursorPosY() < kYPos + kYSize / 2)
		{
			is_mouse_in_button_ = true;
		}
	}

	//ボタンが押されているか調べる
	if (is_mouse_in_button_ && Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_LEFT) > 0)
	{
		is_pushed_ = true;
		pushing_frame_++;
	}
	else
	{
		is_pushed_ = false;
		pushing_frame_ = 0;
	}
}


void ButtomController::Draw() const
{
	const int kBaseColor = GetColor(20, 20, 20);
	const int kButtomColor = is_pushed_ ? GetColor(40, 40, 40) : GetColor(255, 255, 255);
	const int kStrColor = is_pushed_ ? GetColor(200, 200, 200) : GetColor(20, 20, 20);
	const int kFrameSize = 3;
	const int kStrHeight = 16;

	//ベースを描画
	DrawBox(kXPos - kXSize / 2, kYPos - kYSize / 2, kXPos + kXSize / 2, kYPos + kYSize / 2, kBaseColor, TRUE);

	//その上にボタンを描画
	DrawBox(kXPos - kXSize / 2 + kFrameSize, kYPos - kYSize / 2 + kFrameSize, kXPos + kXSize / 2 - kFrameSize, kYPos + kYSize / 2 - kFrameSize, kButtomColor, TRUE);

	//テキストを表示
	DrawString(kXPos - GetDrawStringWidth(text_.c_str(), (int)text_.size()) / 2, kYPos - kStrHeight / 2, text_.c_str(), kStrColor);
}


bool ButtomController::IsPushedNow() const
{
	return is_pushed_ && (pushing_frame_ == 1);
}


bool ButtomController::IsPushed() const
{
	return is_pushed_;
}


int ButtomController::GetPushingFlame() const
{
	return pushing_frame_;
}
