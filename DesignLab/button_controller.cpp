#include "button_controller.h"

#include <Dxlib.h>

#include "mouse.h"


ButtomController::ButtomController() : kXPos(0), kYPos(0), kXSize(100), kYSize(50), m_text("NO SETTING")
{
}


ButtomController::ButtomController(const int _xpos, const int _ypos, const int _xsize, const int _ysize, const std::string _text)
	: kXPos(_xpos), kYPos(_ypos), kXSize(_xsize), kYSize(_ysize), m_text(_text)
{
}


void ButtomController::Update()
{
	//マウスカーソルがボタン内にあるかどうか調べる．
	m_is_mouse_in_buttom = false;

	if (kXPos - kXSize / 2 < Mouse::GetIns()->GetCursorPosX() && Mouse::GetIns()->GetCursorPosX() < kXPos + kXSize / 2)
	{
		if (kYPos - kYSize / 2 < Mouse::GetIns()->GetCursorPosY() && Mouse::GetIns()->GetCursorPosY() < kYPos + kYSize / 2)
		{
			m_is_mouse_in_buttom = true;
		}
	}

	//ボタンが押されているか調べる
	if (m_is_mouse_in_buttom && Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_LEFT) > 0)
	{
		m_is_pushed = true;
		m_pushing_frame++;
	}
	else
	{
		m_is_pushed = false;
		m_pushing_frame = 0;
	}
}


void ButtomController::Draw() const
{
	const int kBaseColor = GetColor(20, 20, 20);
	const int kButtomColor = m_is_pushed ? GetColor(40, 40, 40) : GetColor(255, 255, 255);
	const int kStrColor = m_is_pushed ? GetColor(200, 200, 200) : GetColor(20, 20, 20);
	const int kFrameSize = 3;
	const int kStrHeight = 16;

	//ベースを描画
	DrawBox(kXPos - kXSize / 2, kYPos - kYSize / 2, kXPos + kXSize / 2, kYPos + kYSize / 2, kBaseColor, TRUE);

	//その上にボタンを描画
	DrawBox(kXPos - kXSize / 2 + kFrameSize, kYPos - kYSize / 2 + kFrameSize, kXPos + kXSize / 2 - kFrameSize, kYPos + kYSize / 2 - kFrameSize, kButtomColor, TRUE);

	//テキストを表示
	DrawString(kXPos - GetDrawStringWidth(m_text.c_str(), (int)m_text.size()) / 2, kYPos - kStrHeight / 2, m_text.c_str(), kStrColor);
}


bool ButtomController::isPushedNow() const
{
	return m_is_pushed && (m_pushing_frame == 1);
}


bool ButtomController::isPushed() const
{
	return m_is_pushed;
}


int ButtomController::getPushingFlame() const
{
	return m_pushing_frame;
}
