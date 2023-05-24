#pragma once
#include "Singleton.h"

class Mouse final : public Singleton<Mouse> 
{
private:
	Mouse();
	friend Singleton<Mouse>;

public:
	bool update();	//更新

	int getPosX() const;
	int getPosY() const;

	int getPushingCountLeft() const;
	int getPushingCountRight() const;
	int getPushingCountMiddle() const;

	int getReleasingCountLeft() const;
	int getReleasingCountRight() const;
	int getReleasingCountMiddle() const;

private:

	//クリックカウンタ
	int m_posx, m_poy;
	int m_pushing_count_right, m_pushing_count_left, m_pushing_count_middle;
	int m_releasing_count_right, m_releasing_count_left, m_releasing_count_middle;
};
