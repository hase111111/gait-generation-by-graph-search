#include "mouse.h"

#include <cmath>

#include "DxLib.h"

#include "designlab_math.h"


Mouse::Mouse()
{
	//変数を初期化する
	cursor_pos_x_ = 0;
	cursor_pos_y_ = 0;
	cursor_past_pos_x_ = 0;
	cursor_past_pos_y_ = 0;
	left_pushing_counter_ = 0;
	middle_pushing_counter_ = 0;
	right_pushing_counter_ = 0;
	left_releasing_counter_ = 0;
	middle_releasing_counter_ = 0;
	right_releasing_counter_ = 0;
	wheel_rot_ = 0;
}


void Mouse::Update()
{
	//マウスの位置取得
	cursor_past_pos_x_ = cursor_pos_x_;
	cursor_past_pos_y_ = cursor_pos_y_;
	GetMousePoint(&cursor_pos_x_, &cursor_pos_y_);

	//左クリック
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		//押されているなら
		left_pushing_counter_++;
		left_releasing_counter_ = 0;
	}
	else
	{
		//離されているなら
		left_pushing_counter_ = 0;
		left_releasing_counter_++;
	}

	//右クリック
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		//押されているなら
		right_pushing_counter_++;
		right_releasing_counter_ = 0;
	}
	else
	{
		//離されているなら
		right_pushing_counter_ = 0;
		right_releasing_counter_++;
	}

	//ホールドボタン
	if ((GetMouseInput() & MOUSE_INPUT_MIDDLE) != 0)
	{
		//押されているなら
		middle_pushing_counter_++;
		middle_releasing_counter_ = 0;
	}
	else
	{
		//離されているなら
		middle_pushing_counter_ = 0;
		middle_releasing_counter_++;
	}

	//ホイール回転
	wheel_rot_ = GetMouseWheelRotVol();
}


int Mouse::GetDiffPosX() const
{
	return cursor_pos_x_ - cursor_past_pos_x_;
}


int Mouse::GetDiffPosY() const
{
	return cursor_pos_y_ - cursor_past_pos_y_;
}


double Mouse::getDiffPos() const
{
	return sqrt(static_cast<double>(dl_math::squared(GetDiffPosY()) + dl_math::squared(GetDiffPosX())));
}