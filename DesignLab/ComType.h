#pragma once
#include "HexapodConst.h"
#include <vector>

//
//leg_stateの上位bitにて表されているもの．詳細は波東さんの修論で．
//BFSinHierarchy・CreateComCandidate・PassFindingと様々なファイルに跨る処理をまとめたくて作ったもの.
//
//・重心パターン … 10通りある．leg_state の上位bitにて表現されるもの
//・重心タイプ  … 36通りある．ground_leg から可能なものに数字が割り振ってある．
//		と，長谷川は定義した．波東先輩はこれをこれらを全てタイプ(COMType)として読んでいるので区別するためにそう定義した.
//
//　脚の遊脚のパターンは，
//　　全接地  1通り
//　　1本遊脚 6通り
//　　2本遊脚 15通り → 実現可能なものは??通り
//　　3本遊脚 20通り → 実現可能なものは??通り
//		なので全部で36通りある．
//

namespace ComType 
{
	constexpr int COM_PATTERN_NUM = 10;		//重心パターンの数

	constexpr int COM_TYPE_NUM = 36;		//重心タイプの数


	//接地している脚をtrueとしたbool型の配列と，重心パターンから，可能なものかを出力する
	bool isAbleCoM(const int _com_pattern, const bool _ground_leg[HexapodConst::LEG_NUM]);

	//接地している脚をtrueした配列から，重心タイプを出力する関数．該当しないならば負の値を返す
	char getComTypeFromGroundLeg(const bool _ground_leg[HexapodConst::LEG_NUM]);

	//脚状態から，重心タイプを出力する関数．該当しないならば負の値を返す
	char getComTypeFromLegState(const int _leg_state);

	//接地脚を1，遊脚を0としたビットから，重心タイプを出力する関数．該当しないならば負の値を返す
	char getComTypeFromBit(const int _bit);

	//重心タイプから，接地脚を1，遊脚を0としたビットを出力する関数．該当しないならば全てfalseを返す．getComTypeFromBitの逆の処理．
	void getGroundLegFromComType(const int _com_type, bool _output_ground_leg[HexapodConst::LEG_NUM]);

	// CCCより得られるcom patternを用いて，とりえないcom typeをvectorで返す
	void getDonotUseComTypeFromComPattern(const int _com_pattern, std::vector<int> _output);

	// CCCより得られるcom patternを用いて，とりえないcom typeをすべてfalseにする．_com_type_able_arrayは全36個のcom typeが使用可能かどうかを表すbool型の配列．この値を編集する．
	void checkAbleComTypeFromComPattern(const int _com_pattern, bool _com_type_able_array[COM_TYPE_NUM]);

	//接地することのできない脚から，とりえないcom typeをすべてfalseにする．_com_type_able_arrayは全36個のcom typeが使用可能かどうかを表すbool型の配列．この値を編集する．
	void checkAbleComTypeFromNotGroundableLeg(const int _not_groundble_leg, bool _com_type_able_array[COM_TYPE_NUM]);
}
