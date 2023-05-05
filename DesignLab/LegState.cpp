#include "LegState.h"

bool LegState::isGrounded(const int _leg_state, const int _leg_num)
{
	int _shift_bit[6] = { 0, 4, 8, 12, 16, 20 };//4bitずつずらすために使用する
	int v_bit = 0b1000;							//遊脚を示すbitの位置だけ立ててある

	//_leg_numは0～5の範囲にある必要があるので
	if (_leg_num < 0 || _leg_num > 5) 
	{
		//範囲外ならばfalseを出力する
		return false; 
	}
	else 
	{
		//指定された脚の遊脚のbitが立っているか調べる
		if ((_leg_state & (v_bit << _shift_bit[_leg_num])))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
}

int LegState::getGroundedLegNum(const int _leg_state)
{
	int _res = 0;

	//脚は6本あるので6回ループする
	for (int i = 0; i < 6; i++)
	{
		if (isGrounded(_leg_state, i) == true)
		{
			//接地している脚があればカウントアップする
			_res++;
		}
	}

	return _res;
}

void LegState::getGroundedLegNumWithVector(const int _leg_state, std::vector<int>& _res_number)
{
	//脚は6本あるので6回ループする
	for (int i = 0; i < 6; i++)
	{
		if (isGrounded(_leg_state, i) == true)
		{
			//接地している脚の脚番号をvectorに代入
			_res_number.push_back(i);
		}
	}
}

void LegState::getLiftedLegNumWithVector(const int _leg_state, std::vector<int>& _res_number)
{
	//脚は6本あるので6回ループする
	for (int i = 0; i < 6; i++)
	{
		if (isGrounded(_leg_state, i) == false)
		{
			//浮いている脚の脚番号をvectorに代入
			_res_number.push_back(i);
		}
	}
}
