#include "leg_state.h"


int dl_leg::makeLegState(const ComType::EComPattern _com_pattern, const bool _ground[HexapodConst::LEG_NUM], const int _leg_pos[HexapodConst::LEG_NUM])
{
	int res = 0;

	res |= ComType::convertComPatternToBit(_com_pattern) << SHIFT_TO_COM_NUM;	//重心パターンの数値だけbitを立てる


	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (_ground[i] == true) { res |= 0b1000 << (i * 4); }	//接地しているならば上位bitを立てる

		if (0 < _leg_pos[i] && _leg_pos[i] <= DISCRETE_NUM)
		{
			// 1 ～ 7 の範囲ならばその値だけbitを立てる．
			res |= _leg_pos[i] << (i * 4);
		}
		else
		{
			//範囲外ならば 4 (基準位置)にする．
			res |= 4 << (i * 4);
		}
	}

	return res;
}


bool dl_leg::isGrounded(const int _leg_state, const int _leg_num)
{
	//_leg_numは0～5の範囲にある必要があるので，範囲外ならばfalseを出力する
	if (isAbleLegNum(_leg_num) == false)
	{
		return false;
	}

	//指定された脚の接地脚のbitが立っているか調べる
	if ((_leg_state & (LEG_GROUNDED_MASKBIT << _leg_num * 4)))
	{
		return true;
	}
	else
	{
		return false;
	}
}


int dl_leg::getGroundedLegNum(const int leg_state)
{
	int res = 0;

	//脚の本数分ループする
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (isGrounded(leg_state, i) == true)
		{
			//接地している脚があればカウントアップする
			res++;
		}
	}

	return res;
}


int dl_leg::getLiftedLegNum(const int _leg_state)
{
	return HexapodConst::LEG_NUM - getGroundedLegNum(_leg_state);
}


void dl_leg::getGroundedLegNumWithVector(const int leg_state, std::vector<int>* res_index)
{
	(*res_index).clear();

	//脚は6本あるので6回ループする
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (isGrounded(leg_state, i))
		{
			//接地している脚の脚番号をvectorに代入
			(*res_index).push_back(i);
		}
	}
}


void dl_leg::getLiftedLegNumWithVector(const int _leg_state, std::vector<int>* res_index)
{
	(*res_index).clear();

	//脚は6本あるので6回ループする
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (!isGrounded(_leg_state, i))
		{
			//浮いている脚の脚番号をvectorに代入
			(*res_index).push_back(i);
		}
	}
}


int dl_leg::getLegState(const int _leg_state, const int _leg_num)
{
	const int _shift_num = 4 * _leg_num;	//4bitずつずらす

	return ((_leg_state & (LEG_POS_MASKBIT << _shift_num)) >> _shift_num);
}


int dl_leg::getComPatternState(const int _leg_state)
{
	//重心パターンを保存するビットをマスクし，その値だけ取得できるように右へシフトする．
	return ((_leg_state & COM_STATE_MASKBIT) >> SHIFT_TO_COM_NUM);
}


bool dl_leg::changeLegState(const int leg_index, const int new_discretized_leg_pos, int* leg_state)
{
	//_leg_num か _new_state がおかしいならば falseを返す
	if (!isAbleLegNum(leg_index) || !isAbleLegState(new_discretized_leg_pos))
	{
		return false;
	}

	//新しい脚状態を生成する
	int mask = LEG_STATE_MASKBIT << (leg_index * 4);	//4bitのデータを変更する地点までマスクをずらす
	int state = new_discretized_leg_pos << (leg_index * 4);			//脚位置のデータは4bitづつ配置されているのでその位置まで移動する

	//浮いている脚の脚位置のみを変更（排他的論理和による特定ビットの交換 https://qiita.com/vivisuke/items/bc707190e008551ca07f）
	int res = ((*leg_state) ^ state) & mask;
	(*leg_state) ^= res;

	return true;
}


bool dl_leg::changeLegStateKeepTopBit(int& _leg_state, const int _leg_num, const int _new_state)
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


void dl_leg::changeGround(int& _leg_state, const int _leg_num, const bool _ground)
{
	//_leg_num がおかしいならば，終了．
	if (isAbleLegNum(_leg_num) == false) { return; }

	if (_ground == true)
	{
		_leg_state |= (0b1000 << (_leg_num * 4));
	}
	else
	{
		_leg_state &= ~(0b1000 << (_leg_num * 4));
	}
}


int dl_leg::changeComPattern(int leg_state, const ComType::EComPattern new_com_pattern)
{
	const int state = ComType::convertComPatternToBit(new_com_pattern) << SHIFT_TO_COM_NUM;
	int res = (leg_state ^ state) & COM_STATE_MASKBIT;
	leg_state ^= res;

	return leg_state;
}


int dl_leg::getLegUpDownCount(const int _leg_state_first, const int _leg_state_second)
{
	int _res = 0;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		int _first_state = _leg_state_first & (LEG_GROUNDED_MASKBIT << (i * 4));
		int _second_state = _leg_state_second & (LEG_GROUNDED_MASKBIT << (i * 4));

		if (_first_state ^ _second_state)
		{
			_res++;
		}
	}

	return _res;
}
