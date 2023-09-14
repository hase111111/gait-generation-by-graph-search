#include "com_type.h"

#include <iostream>


namespace
{
	// このように無名名前空間の中に変数を宣言することで，このファイル内でのみ使用可能になる．
	// アクセスするには，先頭に::をつける．

	const dl_com::LegGroundedMap LEG_GROUNDED_PATTERN_MAP = dl_com::makeLegGroundedMap();		//!< 脚の接地パターンに数値を割り振ったマップ．

	const size_t LEG_GROUNDED_PATTERN_NUM = LEG_GROUNDED_PATTERN_MAP.size();	//!< 脚の接地パターンの数．

	const std::unordered_map<EDiscreteComPos, std::vector<int>> LEG_GROUNDE_PATTERN_BAN_LIST = dl_com::makeLegGroundedPatternBanList();	//!< 重心位置から使用不可能な接地パターンをmapで管理する．

	const std::vector<std::vector<int>> LEG_GROUNDED_PATTERN_BAN_LIST_FROM_LEG = dl_com::makeLegGroundedPatternBanListFromLeg();		//!< その脚が遊脚のとき，取り得ない脚の接地パターンを管理する．
}



namespace dl_com
{
	LegGroundedMap dl_com::makeLegGroundedMap()
	{
		LegGroundedMap res;
		int counter = 0;


		// 脚が接地している場合1，遊脚の場合0として，6bitの数値を作成する．0番脚が遊脚，残り接地の場合 111 110 ．
		// そしてそれに 0 から始まる番号を割り振る．(管理しやすくするため．)
		// 全パターンを総当りで書いてあるけど，本当はこのリストを作成する関数を作りたい．


		// 6脚全て接地している場合
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("111111"), counter++));


		// 5脚接地している場合

		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("011111"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("101111"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("110111"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("111011"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("111101"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("111110"), counter++));


		// 4脚接地している場合

		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("001111"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("010111"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("011011"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("011101"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("011110"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("100111"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("101011"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("101101"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("101110"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("110011"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("110101"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("110110"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("111001"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("111010"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("111100"), counter++));


		// 3脚接地している場合．隣り合う3脚が遊脚している場合は除く(転倒してしまうため)．

		//res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("000111"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("001011"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("001101"), counter++));
		//res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("001110"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("010011"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("010101"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("010110"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("011001"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("011010"), counter++));
		//res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("011100"), counter++));
		//res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("100011"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("100101"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("100110"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("101001"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("101010"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("101100"), counter++));
		//res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("110001"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("110010"), counter++));
		res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("110100"), counter++));
		//res.insert(LegGroundedMapValue(dl_leg::LegGroundedBit("111000"), counter++));

		return std::move(res);
	}


	bool isLegPairFree(int leg_index, int leg_ground_pattern_index)
	{
		dl_leg::LegGroundedBit leg_ground_pattern;

		// indexから遊脚のパターンを取得する．
		try
		{
			leg_ground_pattern = ::LEG_GROUNDED_PATTERN_MAP.right.at(leg_ground_pattern_index);
		}
		catch (...)
		{
			return false;
		}

		// 両隣が遊脚の場合はtrueを返す．
		if (!leg_ground_pattern[leg_index % HexapodConst::LEG_NUM] && !leg_ground_pattern[(leg_index + 1) % HexapodConst::LEG_NUM])
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	std::unordered_map<EDiscreteComPos, std::vector<int>> makeLegGroundedPatternBanList()
	{
		std::unordered_map<EDiscreteComPos, std::vector<int>> res;


		// ロボットの体が前に寄っている時に前足が両方とも遊脚だと転倒してしまう．
		// そのため，離散化された重心から，連続する脚が両方とも遊脚になるパターンを禁止するのがこの関数の目的である．
		std::unordered_map<EDiscreteComPos, std::vector<int>> ban_leg_index_list;
		ban_leg_index_list[EDiscreteComPos::FRONT] = { 0,4,5 };
		ban_leg_index_list[EDiscreteComPos::FRONT_RIGHT] = { 0,1,5 };
		ban_leg_index_list[EDiscreteComPos::FRONT_LEFT] = { 3,4,5 };
		ban_leg_index_list[EDiscreteComPos::BACK] = { 1,2,3 };
		ban_leg_index_list[EDiscreteComPos::BACK_RIGHT] = { 0,1,2 };
		ban_leg_index_list[EDiscreteComPos::BACK_LEFT] = { 2,3,4 };
		ban_leg_index_list[EDiscreteComPos::CENTER_BACK] = { 0,2,4 };
		ban_leg_index_list[EDiscreteComPos::CENTER_FRONT] = { 1,3,5 };


		for (auto i : EDiscreteComPos())
		{
			if (ban_leg_index_list.count(i) == 0) { continue; }

			for (auto& j : ban_leg_index_list[i])
			{
				for (int k = 0; k < ::LEG_GROUNDED_PATTERN_NUM; ++k)
				{
					if (isLegPairFree(j, k))
					{
						res[i].push_back(k);
					}
				}
			}
		}

		return std::move(res);
	}


	std::vector<std::vector<int>> makeLegGroundedPatternBanListFromLeg()
	{
		std::vector<std::vector<int>> res;

		res.resize(HexapodConst::LEG_NUM);	// 脚の数だけvectorを確保する．

		// i 番脚を接地しなければ，取ることができないものを保存する．
		for (int i = 0; i < HexapodConst::LEG_NUM; i++)
		{
			for (int j = 0; j < ::LEG_GROUNDED_PATTERN_NUM; ++j)
			{
				// i番目のbitを確認し，立っているならば(つまり，その脚を接地しなければいけないなら)，そのパターンを禁止する．
				if (LEG_GROUNDED_PATTERN_MAP.right.at(j)[i])
				{
					res[i].push_back(j);
				}
			}
		}

		return std::move(res);
	}



}


void dl_com::getGroundLegFromComType(const int _com_type, bool _output_ground_leg[HexapodConst::LEG_NUM])
{
	switch (_com_type)
	{
	case 0:
		_output_ground_leg[0] = true; _output_ground_leg[1] = true; _output_ground_leg[2] = true;
		_output_ground_leg[3] = true; _output_ground_leg[4] = true; _output_ground_leg[5] = true;
		break;

	case 1:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 2:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 3:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 4:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 5:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	case 6:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 7:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 8:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	case 9:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 10:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 11:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 12:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	case 13:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 14:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	case 15:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 16:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	case 17:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 18:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 19:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 20:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 21:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 22:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = false;
		break;

	case 23:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	case 24:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 25:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	case 26:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 27:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 28:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = false;
		break;

	case 29:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	case 30:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = true;
		break;

	case 31:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 32:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	case 33:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = false;	_output_ground_leg[5] = true;
		break;

	case 34:
		_output_ground_leg[0] = true;	_output_ground_leg[1] = false;	_output_ground_leg[2] = true;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = false;	_output_ground_leg[5] = false;
		break;

	case 35:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = true;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = true;	_output_ground_leg[4] = true;	_output_ground_leg[5] = false;
		break;

	default:
		_output_ground_leg[0] = false;	_output_ground_leg[1] = false;	_output_ground_leg[2] = false;
		_output_ground_leg[3] = false;	_output_ground_leg[4] = false;	_output_ground_leg[5] = false;
		break;
		break;
	}
}

void dl_com::getDonotUseComTypeFromComPattern(const int _com_pattern, std::vector<int>& _output)
{
	//com patternから使用不可能なcom typeを取得して出力する．
	for (const auto& i : COMTYPE_BAN_LIST[_com_pattern])
	{
		_output.push_back(i);
	}
}

void dl_com::checkAbleComTypeFromComPattern(const int _com_pattern, bool _com_type_able_array[COM_TYPE_NUM])
{
	std::vector<int> _cannot_use_type;
	getDonotUseComTypeFromComPattern(_com_pattern, _cannot_use_type);

	for (const auto& i : _cannot_use_type)
	{
		_com_type_able_array[i] = false;
	}
}

void dl_com::checkAbleComTypeFromNotGroundableLeg(const int _not_groundble_leg, bool _com_type_able_array[COM_TYPE_NUM])
{
	//とることのできないcom typeを全てfalseに変更する．
	for (int i = 0; i < BAN_LIST_ARRAY_SIZE; i++)
	{
		_com_type_able_array[BAN_LIST[_not_groundble_leg][i]] = false;
	}
}

void dl_com::checkAbleComTypeFromNotFreeLeg(const int _not_free_leg, bool _com_type_able_array[COM_TYPE_NUM])
{
	bool _reverse[COM_TYPE_NUM];

	for (int i = 0; i < COM_TYPE_NUM; i++)
	{
		_reverse[i] = true;
	}


	//とることのできないcom typeを全てfalseに変更する．
	for (int i = 0; i < BAN_LIST_ARRAY_SIZE; i++)
	{
		_reverse[BAN_LIST[_not_free_leg][i]] = false;
	}

	for (int i = 0; i < COM_TYPE_NUM; i++)
	{
		if (_reverse[i] == true)
		{
			_com_type_able_array[i] = false;
		}
	}
}