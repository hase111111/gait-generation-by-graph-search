#include "LegState.h"
#include "HexapodConst.h"

bool LegState::isGrounded(const int _leg_state, const int _leg_num)
{
	int _shift_bit[HexapodConst::LEG_NUM] = { 0, 4, 8, 12, 16, 20 };//4bitずつずらすために使用する
	int v_bit = 0b1000;												//接地脚を示すbitの位置だけ立ててある

	//_leg_numは0～5の範囲にある必要があるので
	if (isAbleLegNum(_leg_num) == false) 
	{
		//範囲外ならばfalseを出力する
		return false; 
	}
	else 
	{
		//指定された脚の接地脚のbitが立っているか調べる
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

	//脚の本数分ループする
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
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
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
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
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (isGrounded(_leg_state, i) == false)
		{
			//浮いている脚の脚番号をvectorに代入
			_res_number.push_back(i);
		}
	}
}

int LegState::getLegState(const int _leg_state, const int _leg_num)
{
	const int _shift_bit[HexapodConst::LEG_NUM] = { 0, 4, 8, 12, 16, 20 };//4bitずつずらすために使用する
	const int kaisou_bit = 0b0111;	//脚位置を示す部分
	return ((_leg_state & (kaisou_bit << _shift_bit[_leg_num])) >> _shift_bit[_leg_num]);
}

int LegState::getComPatternState(const int _leg_state)
{
	//重心パターンを保存するビットをマスクし，その値だけ取得できるように右へシフトする．
	return ((_leg_state & COM_STATE_MASKBIT) >> SHIFT_TO_COM_NUM);
}

bool LegState::changeLegState(int& _leg_state, const int _leg_num, const int _new_state)
{
	//_leg_num か _new_state がおかしいならば falseを返す
	if (isAbleLegNum(_leg_num) == false ||isAbleLegState(_new_state) == false ) 
	{
		return false;
	}

	//新しい脚状態を生成する
	int _mask = LEG_STATE_MASKBIT << (_leg_num * 4);	//4bitのデータを変更する地点までマスクをずらす
	int _state = _new_state << (_leg_num * 4);			//脚位置のデータは4bitづつ配置されているのでその位置まで移動する

	//浮いている脚の脚位置のみを変更（排他的論理和による特定ビットの交換 https://qiita.com/vivisuke/items/bc707190e008551ca07f）
	int _res = (_leg_state ^ _state) & _mask;
	_leg_state ^= _res;

	return true;
}

bool LegState::changeLegStateKeepTopBit(int& _leg_state, const int _leg_num, const int _new_state)
{
	//_leg_num か _new_state がおかしいならば falseを返す
	if (isAbleLegNum(_leg_num) == false || isAbleLegState(_new_state) == false)
	{
		return false;
	}

	//新しい脚状態を生成する
	int _mask = LEG_POS_MASKBIT << (_leg_num * 4);	//4bitのデータを変更する地点までマスクをずらす
	int _state = _new_state << (_leg_num * 4);			//脚位置のデータは4bitづつ配置されているのでその位置まで移動する

	//浮いている脚の脚位置のみを変更（排他的論理和による特定ビットの交換 https://qiita.com/vivisuke/items/bc707190e008551ca07f）
	int _res = (_leg_state ^ _state) & _mask;
	_leg_state ^= _res;

	return true;
}

bool LegState::isAbleLegNum(const int _num)
{
	// 0 ～ 5なら true
	if (0 <= _num && _num < HexapodConst::LEG_NUM) { return true; }

	return false;
}

bool LegState::isAbleLegState(const int _state)
{
	// 8 (0b1000) なら false
	if (_state == 8) { return false; }

	// 1 ～ 15なら true
	if (0 < _state && _state < 15) { return true; }

	return false;
}
