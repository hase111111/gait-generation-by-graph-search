#include "com_type.h"

#include <iostream>


namespace
{
	// このように無名名前空間の中に変数を宣言することで，このファイル内でのみ使用可能になる．
	// アクセスするには，先頭に::をつける．

	const dl_com::LegGroundedMap LEG_GROUNDED_PATTERN_MAP = dl_com::makeLegGroundedMap();		//!< 脚の接地パターンに数値を割り振ったマップ．

	const int LEG_GROUNDED_PATTERN_NUM = static_cast<int>(LEG_GROUNDED_PATTERN_MAP.size());	//!< 脚の接地パターンの数．

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


	int getLegGroundPatternNum()
	{
		return LEG_GROUNDED_PATTERN_NUM;
	}


	dl_leg::LegGroundedBit getLegGroundedBitFromLegGroundPatternIndex(int leg_ground_pattern_index)
	{
		dl_leg::LegGroundedBit res;

		// indexから遊脚のパターンを取得する．
		res = ::LEG_GROUNDED_PATTERN_MAP.right.at(leg_ground_pattern_index);

		return std::move(res);
	}


	void banLegGroundPatternFromCom(EDiscreteComPos discrete_com_pos, boost::dynamic_bitset<>* output)
	{
		//異常な値ならばreturn

		if (output == nullptr) { return; }

		if ((*output).size() != getLegGroundPatternNum()) { return; }


		// LEG_GROUNDE_PATTERN_BAN_LISTにキーが存在していないことや，値がgetLegGroundPatternNumを超えてないことを確認していない．エラーが出たらそこが原因かもしれない．

		for (auto& i : LEG_GROUNDE_PATTERN_BAN_LIST.at(discrete_com_pos))
		{
			(*output)[i] = false;
		}
	}


	void banLegGroundPatternFromNotGroundableLeg(int not_groundble_leg_index, boost::dynamic_bitset<>* output)
	{
		//異常な値ならばreturn

		if (output == nullptr) { return; }

		if ((*output).size() != getLegGroundPatternNum()) { return; }


		// LEG_GROUNDED_PATTERN_BAN_LIST_FROM_LEGにキーが存在していないことや，値がgetLegGroundPatternNumを超えてないことを確認していない．エラーが出たらそこが原因かもしれない．

		for (auto& i : LEG_GROUNDED_PATTERN_BAN_LIST_FROM_LEG[not_groundble_leg_index])
		{
			(*output)[i] = false;
		}
	}


	void banLegGroundPatternFromNotFreeLeg(int not_lift_leg_index, boost::dynamic_bitset<>* output)
	{
		//異常な値ならばreturn

		if (output == nullptr) { return; }

		if ((*output).size() != getLegGroundPatternNum()) { return; }


		// LEG_GROUNDED_PATTERN_BAN_LIST_FROM_LEGにキーが存在していないことや，値がgetLegGroundPatternNumを超えてないことを確認していない．エラーが出たらそこが原因かもしれない．
		boost::dynamic_bitset<> inverse_output(getLegGroundPatternNum());

		for (auto& i : LEG_GROUNDED_PATTERN_BAN_LIST_FROM_LEG[not_lift_leg_index])
		{
			inverse_output[i] = true;
		}

		(*output) &= inverse_output;
	}



}