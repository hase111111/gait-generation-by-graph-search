#include "leg_state.h"

#include "cassert_define.h"


designlab::leg_func::LegStateBit designlab::leg_func::MakeLegStateBit(
	const DiscreteComPos discrete_com_pos, 
	const std::array<bool, HexapodConst::kLegNum>& is_ground,
	const std::array<DiscreteLegPos, HexapodConst::kLegNum>& discretized_leg_pos
)
{
	LegStateBit res = 0;

	LegStateBit discrete_com_pos_bit = static_cast<unsigned int>(discrete_com_pos);		//bitに変換する
	res |= discrete_com_pos_bit << kShiftToComNum;	//重心パターンの数値だけbitを立てる


	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		//接地しているならば上位bitを立てる
		size_t ground_bit_index = static_cast<size_t>(i + 1) * 4 - 1;
		if (is_ground[i]) { res[ground_bit_index] = true; }

		// 脚のbitを立てる
		LegStateBit discrete_leg_pos_bit = static_cast<unsigned int>(discretized_leg_pos[i]);	//bitに変換する
		size_t shift_num = static_cast<size_t>(i) * 4;	//4bitずつずらす

		res |= discrete_leg_pos_bit << shift_num;	//脚の位置の数値だけbitを立てる
	}

	return res;
}


bool designlab::leg_func::IsGrounded(const LegStateBit& leg_state, const int leg_index)
{
	// leg_indexは0～5の範囲にある必要がある．
	assert(0 <= leg_index);
	assert(leg_index < HexapodConst::kLegNum);

	//指定された脚の接地脚のbitが立っているか調べる
	size_t ground_bit_index = static_cast<size_t>(leg_index + 1) * 4 - 1;

	if (leg_state[ground_bit_index])
	{
		return true;
	}
	else
	{
		return false;
	}
}

designlab::leg_func::LegGroundedBit designlab::leg_func::GetLegGroundedBit(const LegStateBit& leg_state)
{
	LegGroundedBit res;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (IsGrounded(leg_state, i))
		{
			res[i] = true;
		}
		else
		{
			res[i] = false;
		}
	}

	return res;
}

int designlab::leg_func::GetGroundedLegNum(const LegStateBit& leg_state)
{
	int res = 0;

	//脚の本数分ループする
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (IsGrounded(leg_state, i))
		{
			//接地している脚があればカウントアップする
			res++;
		}
	}

	return res;
}

int designlab::leg_func::GetLiftedLegNum(const LegStateBit& leg_state)
{
	return HexapodConst::kLegNum - GetGroundedLegNum(leg_state);
}

void designlab::leg_func::GetGroundedLegIndexByVector(const LegStateBit& leg_state, std::vector<int>* res_index)
{
	// res_indexはnullptrでないこと，かつ空である必要がある
	assert(res_index != nullptr);
	assert((*res_index).size() == 0);

	//脚は6本あるので6回ループする
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (IsGrounded(leg_state, i))
		{
			//接地している脚の脚番号をvectorに代入
			(*res_index).push_back(i);
		}
	}
}

void designlab::leg_func::GetLiftedLegIndexByVector(const LegStateBit& leg_state, std::vector<int>* res_index)
{
	// res_indexはnullptrでないこと，かつ空である必要がある
	assert(res_index != nullptr);
	assert((*res_index).size() == 0);

	//脚は6本あるので6回ループする
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (!IsGrounded(leg_state, i))
		{
			//浮いている脚の脚番号をvectorに代入
			(*res_index).push_back(i);
		}
	}
}

DiscreteLegPos designlab::leg_func::GetDiscreteLegPos(const LegStateBit& leg_state, const int leg_index)
{
	// leg_indexは0～5の範囲にある必要がある．
	assert(0 <= leg_index);
	assert(leg_index < HexapodConst::kLegNum);

	const int shift_num = 4 * leg_index;	//4bitずつずらす

	const int res = static_cast<int>(((leg_state & (kLegPosMaskbit << shift_num)) >> shift_num).to_ulong());

	return static_cast<DiscreteLegPos>(res);
}

DiscreteComPos designlab::leg_func::GetDiscreteComPos(const LegStateBit& leg_state)
{
	//重心パターンを保存するビットをマスクし，その値だけ取得できるように右へシフトする．
	const int res = static_cast<int>(((leg_state & kComStateMaskbit) >> kShiftToComNum).to_ulong());

	return static_cast<DiscreteComPos>(res);
}


void designlab::leg_func::ChangeLegState(
	const int leg_index, 
	const DiscreteLegPos new_discretized_leg_pos,
	const bool is_ground,
	LegStateBit* leg_state
)
{
	// leg_indexは0～5の範囲にある必要がある．
	assert(0 <= leg_index);
	assert(leg_index < HexapodConst::kLegNum);

	// leg_state は nullptrではない
	assert(leg_state != nullptr);

	ChangeDiscreteLegPos(leg_index, new_discretized_leg_pos, leg_state);
	ChangeGround(leg_index, is_ground, leg_state);
}

void designlab::leg_func::ChangeDiscreteLegPos(
	const int leg_index, 
	const DiscreteLegPos new_discretized_leg_pos,
	LegStateBit* leg_state
)
{
	// leg_indexは0～5の範囲にある必要がある．
	assert(0 <= leg_index);
	assert(leg_index < HexapodConst::kLegNum);

	// leg_state は nullptrではない
	assert(leg_state != nullptr);


	//新しい脚状態を生成する
	const size_t shift_num = static_cast<size_t>(leg_index) * 4;									//4bitずつずらす
	const LegStateBit mask = kLegPosMaskbit << shift_num;											//4bitのデータを変更する地点までマスクをずらす
	const LegStateBit discreate_leg_pos_bit = static_cast<unsigned int>(new_discretized_leg_pos);	//bitに変換する
	const LegStateBit state = discreate_leg_pos_bit << shift_num;	//脚位置のデータは4bitづつ配置されているのでその位置まで移動する

	//浮いている脚の脚位置のみを変更（排他的論理和による特定ビットの交換 https://qiita.com/vivisuke/items/bc707190e008551ca07f）
	LegStateBit res = ((*leg_state) ^ state) & mask;
	(*leg_state) ^= res;
}

void designlab::leg_func::ChangeGround(const int leg_index, const bool is_ground, LegStateBit* leg_state)
{
	// leg_indexは0～5の範囲にある必要がある．
	assert(0 <= leg_index);
	assert(leg_index < HexapodConst::kLegNum);

	// leg_state は nullptrではない
	assert(leg_state != nullptr);


	//指定された脚の接地脚のbitを立てるか消すかする
	const size_t ground_bit_index = static_cast<size_t>(leg_index + 1) * 4 - 1;

	if (is_ground)
	{
		(*leg_state)[ground_bit_index] = true;
	}
	else
	{
		(*leg_state)[ground_bit_index] = false;
	}
}

void designlab::leg_func::ChangeAllLegGround(const LegGroundedBit& is_ground_list, LegStateBit* leg_state)
{
	// leg_state は nullptrではない
	assert(leg_state != nullptr);

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		ChangeGround(i, is_ground_list[i], leg_state);
	}
}

void designlab::leg_func::ChangeDiscreteComPos(const DiscreteComPos new_com_pattern, LegStateBit* leg_state)
{
	// leg_state は nullptrではない
	assert(leg_state != nullptr);

	const LegStateBit state = static_cast<unsigned int>(new_com_pattern) << kShiftToComNum;
	LegStateBit sub = ((*leg_state) ^ state) & kComStateMaskbit;
	(*leg_state) ^= sub;
}