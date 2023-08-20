#include "mouse.h"

#include "DxLib.h"


Mouse::Mouse()
{
	//変数を初期化する
	m_posx = 0;
	m_poy = 0;
	m_past_posx = 0;
	m_past_posy = 0;
	m_pushing_count_left = 0;
	m_pushing_count_middle = 0;
	m_pushing_count_right = 0;
	m_releasing_count_left = 0;
	m_releasing_count_middle = 0;
	m_releasing_count_right = 0;
	m_wheel_rot = 0;
}


void Mouse::update()
{
	//マウスの位置取得
	m_past_posx = m_posx;
	m_past_posy = m_poy;
	GetMousePoint(&m_posx, &m_poy);

	//左クリック
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		//押されているなら
		m_pushing_count_left++;
		m_releasing_count_left = 0;
	}
	else
	{
		//離されているなら
		m_pushing_count_left = 0;
		m_releasing_count_left++;
	}

	//右クリック
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		//押されているなら
		m_pushing_count_right++;
		m_releasing_count_right = 0;
	}
	else
	{
		//離されているなら
		m_pushing_count_right = 0;
		m_releasing_count_right++;
	}

	//ホールドボタン
	if ((GetMouseInput() & MOUSE_INPUT_MIDDLE) != 0)
	{
		//押されているなら
		m_pushing_count_middle++;
		m_releasing_count_middle = 0;
	}
	else
	{
		//離されているなら
		m_pushing_count_middle = 0;
		m_releasing_count_middle++;
	}

	//ホイール回転
	m_wheel_rot = GetMouseWheelRotVol();
}


int Mouse::getPosX() const
{
	return m_posx;
}


int Mouse::getDiffPosX() const
{
	return m_posx - m_past_posx;
}


int Mouse::getPosY() const
{
	return m_poy;
}


int Mouse::getDiffPosY() const
{
	return m_poy - m_past_posy;
}


int Mouse::getPushingCountLeft() const
{
	return m_pushing_count_left;
}


int Mouse::getPushingCountRight() const
{
	return m_pushing_count_right;
}


int Mouse::getPushingCountMiddle() const
{
	return m_pushing_count_middle;
}


int Mouse::getReleasingCountLeft() const
{
	return m_releasing_count_left;
}


int Mouse::getReleasingCountRight() const
{
	return m_releasing_count_right;
}


int Mouse::getReleasingCountMiddle() const
{
	return m_releasing_count_middle;
}


int Mouse::getWheelRot() const
{
	return m_wheel_rot;
}
