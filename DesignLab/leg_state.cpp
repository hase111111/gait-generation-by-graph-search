#include "leg_state.h"


namespace dl_leg
{

	LegStateBit makeLegState(const EDiscreteComPos com_pattern, const bool is_ground[HexapodConst::LEG_NUM], const EDiscreteLegPos discretized_leg_pos[HexapodConst::LEG_NUM])
	{
		LegStateBit res = 0;

		res |= static_cast<int>(com_pattern) << SHIFT_TO_COM_NUM;	//重心パターンの数値だけbitを立てる


		for (int i = 0; i < HexapodConst::LEG_NUM; i++)
		{
			//接地しているならば上位bitを立てる
			if (is_ground[i]) { res[(i + 1) * 4 - 1] = true; }

			// 脚のbitを立てる
			res |= static_cast<int>(discretized_leg_pos[i]) << (i * 4);
		}

		return res;
	}


	bool isGrounded(const LegStateBit& leg_state, const int leg_index)
	{
		//_leg_numは0～5の範囲にある必要があるので，範囲外ならばfalseを出力する
		if (!isAbleLegNum(leg_index))
		{
			return false;
		}

		//指定された脚の接地脚のbitが立っているか調べる
		if (leg_state[(leg_index + 1) * 4 - 1])
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	LegGroundedBit dl_leg::getLegGroundedBit(const LegStateBit& leg_state)
	{
		LegGroundedBit res;

		for (int i = 0; i < HexapodConst::LEG_NUM; i++)
		{
			if (isGrounded(leg_state, i))
			{
				res[i] = true;
			}
			else
			{
				res[i] = false;
			}
		}

		return std::move(res);
	}


	int getGroundedLegNum(const LegStateBit& leg_state)
	{
		int res = 0;

		//脚の本数分ループする
		for (int i = 0; i < HexapodConst::LEG_NUM; i++)
		{
			if (isGrounded(leg_state, i))
			{
				//接地している脚があればカウントアップする
				res++;
			}
		}

		return res;
	}


	int getLiftedLegNum(const LegStateBit& leg_state)
	{
		return HexapodConst::LEG_NUM - getGroundedLegNum(leg_state);
	}


	void getGroundedLegIndexWithVector(const LegStateBit& leg_state, std::vector<int>* res_index)
	{
		if (res_index == nullptr) { return; }

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


	void getLiftedLegIndexWithVector(const LegStateBit& leg_state, std::vector<int>* res_index)
	{
		if (res_index == nullptr) { return; }

		(*res_index).clear();

		//脚は6本あるので6回ループする
		for (int i = 0; i < HexapodConst::LEG_NUM; i++)
		{
			if (!isGrounded(leg_state, i))
			{
				//浮いている脚の脚番号をvectorに代入
				(*res_index).push_back(i);
			}
		}
	}


	EDiscreteLegPos getLegState(const LegStateBit& leg_state, const int leg_index)
	{
		const int shift_num = 4 * leg_index;	//4bitずつずらす

		const int res = static_cast<int>(((leg_state & (LEG_POS_MASKBIT << shift_num)) >> shift_num).to_ulong());

		return static_cast<EDiscreteLegPos>(res);
	}


	EDiscreteComPos getComPatternState(const LegStateBit& leg_state)
	{
		//重心パターンを保存するビットをマスクし，その値だけ取得できるように右へシフトする．
		const int res = static_cast<int>(((leg_state & COM_STATE_MASKBIT) >> SHIFT_TO_COM_NUM).to_ulong());

		return static_cast<EDiscreteComPos>(res);
	}


	bool changeLegState(int leg_index, EDiscreteLegPos new_discretized_leg_pos, bool is_ground, LegStateBit* leg_state)
	{
		//leg_num か _new_state がおかしいならば falseを返す
		if (!isAbleLegNum(leg_index) || leg_state == nullptr)
		{
			return false;
		}

		//新しい脚状態を生成する
		LegStateBit mask = LEG_STATE_MASKBIT << (leg_index * 4);								//4bitのデータを変更する地点までマスクをずらす
		LegStateBit  state = static_cast<int>(new_discretized_leg_pos) << (leg_index * 4);	//脚位置のデータは4bitづつ配置されているのでその位置まで移動する

		//浮いている脚の脚位置のみを変更（排他的論理和による特定ビットの交換 https://qiita.com/vivisuke/items/bc707190e008551ca07f）
		LegStateBit res = ((*leg_state) ^ state) & mask;
		(*leg_state) ^= res;

		return true;
	}


	bool changeLegStateKeepTopBit(const int leg_index, const EDiscreteLegPos new_discretized_leg_pos, LegStateBit* leg_state)
	{
		//leg_num か _new_state がおかしいならば falseを返す
		if (!isAbleLegNum(leg_index) || leg_state == nullptr)
		{
			return false;
		}

		//新しい脚状態を生成する
		LegStateBit mask = LEG_POS_MASKBIT << (leg_index * 4);								//4bitのデータを変更する地点までマスクをずらす
		LegStateBit state = static_cast<int>(new_discretized_leg_pos) << (leg_index * 4);	//脚位置のデータは4bitづつ配置されているのでその位置まで移動する

		//浮いている脚の脚位置のみを変更（排他的論理和による特定ビットの交換 https://qiita.com/vivisuke/items/bc707190e008551ca07f）
		LegStateBit res = ((*leg_state) ^ state) & mask;
		(*leg_state) ^= res;

		return true;
	}


	void changeGround(const int leg_index, const bool is_ground, LegStateBit* leg_state)
	{
		//leg_num がおかしいならば，終了．
		if (!isAbleLegNum(leg_index)) { return; }


		//指定された脚の接地脚のbitを立てるか消すかする
		if (is_ground)
		{
			(*leg_state)[(leg_index + 1) * 4 - 1] = true;
		}
		else
		{
			(*leg_state)[(leg_index + 1) * 4 - 1] = false;
		}
	}


	void changeComPattern(const EDiscreteComPos new_com_pattern, LegStateBit* leg_state)
	{
		if (leg_state == nullptr) { return; }

		const LegStateBit state = static_cast<int>(new_com_pattern) << SHIFT_TO_COM_NUM;
		LegStateBit sub = ((*leg_state) ^ state) & COM_STATE_MASKBIT;
		(*leg_state) ^= sub;
	}

}	// namespace dl_leg


int dl_leg::getLegUpDownCount(const int _leg_state_first, const int _leg_state_second)
{
	int res = 0;

	//for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	//{
	//	int first_state = _leg_state_first & (LEG_GROUNDED_MASKBIT << (i * 4));
	//	int second_state = _leg_state_second & (LEG_GROUNDED_MASKBIT << (i * 4));

	//	if (first_state ^ second_state)
	//	{
	//		res++;
	//	}
	//}

	return res;
}