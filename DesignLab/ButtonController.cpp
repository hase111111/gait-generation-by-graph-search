#include "ButtonController.h"
#include "DxLib.h"
#include "GraphicConst.h"
#include "Mouse.h"

ButtomController::ButtomController() : X_POS(GraphicConst::WIN_X / 2), Y_POS(GraphicConst::WIN_Y / 2), X_SIZE(100), Y_SIZE(50), m_text("NO SETTING")
{
}

ButtomController::ButtomController(const int _xpos, const int _ypos, const int _xsize, const int _ysize, const std::string _text)
	: X_POS(_xpos), Y_POS(_ypos), X_SIZE(_xsize), Y_SIZE(_ysize), m_text(_text)
{
}

void ButtomController::update()
{
	//マウスカーソルがボタン内にあるかどうか調べる．
	m_is_mouse_in_buttom = false;

	if (X_POS - X_SIZE / 2 < Mouse::getIns()->getPosX() && Mouse::getIns()->getPosX() < X_POS + X_SIZE / 2) 
	{
		if (Y_POS - Y_SIZE / 2 < Mouse::getIns()->getPosY() && Mouse::getIns()->getPosY() < Y_POS + Y_SIZE / 2)
		{
			m_is_mouse_in_buttom = true;
		}
	}

	//ボタンが押されているか調べる
	if (m_is_mouse_in_buttom == true && Mouse::getIns()->getPushingCountLeft() > 0)
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

void ButtomController::draw() const
{
	const int _base_color = GetColor(20, 20, 20);
	const int _buttom_color = m_is_pushed ? GetColor(40, 40, 40) : GetColor(255, 255, 255);
	const int _str_color = m_is_pushed ? GetColor(200, 200, 200) : GetColor(20, 20, 20);
	const int _frame_size = 3;
	const int _str_height = 16;

	//ベースを描画
	DrawBox(X_POS - X_SIZE / 2, Y_POS - Y_SIZE / 2, X_POS + X_SIZE / 2, Y_POS + Y_SIZE / 2, _base_color, TRUE);

	//その上にボタンを描画
	DrawBox(X_POS - X_SIZE / 2 + _frame_size, Y_POS - Y_SIZE / 2 + _frame_size, X_POS + X_SIZE / 2 - _frame_size, Y_POS + Y_SIZE / 2 - _frame_size, _buttom_color, TRUE);

	//テキストを表示
	DrawString(X_POS - GetDrawStringWidth(m_text.c_str(), (int)m_text.size()) / 2, Y_POS - _str_height / 2, m_text.c_str(), _str_color);
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
