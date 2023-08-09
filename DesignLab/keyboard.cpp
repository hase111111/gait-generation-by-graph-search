#include "keyboard.h"

#include "DxLib.h"


Keyboard::Keyboard()
{
	for (int i = 0; i < KEY_NUM; i++)
	{
		m_releasing_count[i] = 0;
		m_pressing_count[i] = 0;
	}
}


void Keyboard::update()
{
	char now_key_status[KEY_NUM];
	GetHitKeyStateAll(now_key_status);       //今のキーの入力状態を取得

	for (int i = 0; i < KEY_NUM; i++)
	{
		if (now_key_status[i] != 0)
		{
			//i番のキーが押されていたら

			if (m_releasing_count[i] > 0)
			{
				//離されカウンタが0より大きければ
				m_releasing_count[i] = 0;   //0に戻す
			}

			m_pressing_count[i]++;          //押されカウンタを増やす
		}
		else
		{
			//i番のキーが離されていたら
			if (m_pressing_count[i] > 0)
			{
				//押されカウンタが0より大きければ
				m_pressing_count[i] = 0;    //0に戻す
			}

			m_releasing_count[i]++;         //離されカウンタを増やす
		}
	}
}


int Keyboard::getPressingCount(int key_code) const
{
	if (!isAvailableCode(key_code))
	{
		return -1;
	}

	return m_pressing_count[key_code];
}


int Keyboard::getReleasingCount(int key_code) const
{
	if (!isAvailableCode(key_code))
	{
		return -1;
	}

	return m_releasing_count[key_code];
}


bool Keyboard::isAvailableCode(int key_code) const
{
	if (!(0 <= key_code && key_code < KEY_NUM))
	{
		return false;
	}

	return true;
}
