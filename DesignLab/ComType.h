#pragma once
#include "HexapodConst.h"
#include <vector>

namespace ComType
{
	constexpr int COM_PATTERN_NUM = 10;		//!< 重心パターンの数

	constexpr int COM_TYPE_NUM = 36;		//!< 重心タイプの数

	constexpr int BAN_LIST_ARRAY_SIZE = 23;

	constexpr char BAN_LIST[HexapodConst::LEG_NUM][BAN_LIST_ARRAY_SIZE] =
	{

	{ 0, 1, 2, 3, 4, 5,       8, 9,10,11,12,   14,   16,      19,20,21,22,      25,   27,28,      31,32,   34    },	//右前脚
	{ 0,    2, 3, 4, 5, 6, 7, 8,      11,12,13,   15,   17,      20,21,22,23,      26,   28,29,      32,33,   35 },	//右中脚
	{ 0, 1,    3, 4, 5, 6, 7,    9,10,   12,13,14,   16,   18,      21,22,23,24,      27,   29,30,      33,34    },	//右後脚
	{ 0, 1, 2,    4, 5, 6,    8, 9,   11,   13,14,15,   17,18,19,      22,23,24,25,      28,      31,      34,35 },	//左後脚
	{ 0, 1, 2, 3,    5, 6, 7, 8,   10,   12,   14,15,16,   18,19,20,      23,   25,26,      29,30,   32,      35 },	//左中脚
	{ 0, 1, 2, 3, 4,    6, 7,    9,10,11,   13,   15,   17,18,19,20,21,      24,   26,27,      30,31,   33       }	//左前脚

	};

	const std::vector< std::vector<int> > COMTYPE_BAN_LIST =
	{
		{ 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 },	//パターン0 どの隣りあった足も上げることができない
		{ 18, 19,             23, 24, 25,             29, 30, 31,             35 },	//パターン6
		{ 18, 19, 20,             24, 25, 26,             30, 31, 32 },				//パターン1
		{ 19, 20, 21,             25, 26, 27,             31, 32, 33 },			//パターン2
		{ 20, 21, 22,             26, 27, 28,             32, 33, 34 },		//パターン3
		{ 21, 22, 23,             27, 28, 29,             33, 34, 35 },	//パターン4
		{ 18,             22, 23, 24,             28, 29, 30,             34, 35 },	//パターン5
		{ 18,     20,     22,     24,     26,     28,     30,     32,     34, },		//パターン7
		{ 19,     21,     23,     25,     27,     29,     31,     33,     35 }	//パターン8
	};

	//! @enum 重心パターンの列挙型
	//! @brief 重心がどこにあるかを表す．
	enum class EComPattern : char
	{
		front,			//!< 重心が前方にある
		left_front,		//!< 重心が左前方にある
		left_back,		//!< 重心が左後方にある
		back,			//!< 重心が後方にある
		right_back,		//!< 重心が右後方にある
		right_front,	//!< 重心が右前方にある
		center_front,	//!< 重心が中央前方にある．三角形
		center_back,	//!< 重心が中央後方にある．逆三角径
		Error			//!< エラー
	};

	//! @brief 重心パターンをbitに変換する関数
	//! @param[in] _com_pattern 重心パターン
	//! @return 重心パターンをbitに変換した値
	//! @details 重心パターンはleg_stateの上位bitにて表現されている <br>
	//! 4bitに収まるように値を変換する．
	//! @attention 重心パターンの数が変わった場合は，この関数も変更する必要がある．
	int convertComPatternToBit(const EComPattern _com_pattern);

	//! @brief 重心パターンをbitから変換する関数
	//! @param[in] _bit 重心パターンをbitに変換した値
	//! @return 重心パターン
	EComPattern convertBitToComPattern(const int _bit);

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

	//遊脚することのできない脚から，とりえないcom typeをすべてfalseにする．_com_type_able_arrayは全36個のcom typeが使用可能かどうかを表すbool型の配列．この値を編集する．
	void checkAbleComTypeFromNotFreeLeg(const int _not_free_leg, bool _com_type_able_array[COM_TYPE_NUM]);
}

//! @namespace ComType
//! @brief 重心タイプに関する名前空間
//! @details leg_stateの上位bitにて表されているもの．詳細は波東さんの修論で，<br>
//! BFSinHierarchy・CreateComCandidate・PassFindingと様々なファイルに跨る処理をまとめたくて作ったもの.<br>
//! <br>
//!・重心パターン … 10通りある．leg_state の上位bitにて表現されるもの <br>
//!・重心タイプ  … 36通りある．ground_leg から可能なものに数字が割り振ってある．<br>
//!		と，長谷川は定義した．波東先輩はこれをこれらを全てタイプ(COMType)として読んでいるので区別するためにそう定義した.<br>
//! <br>
//!　脚の遊脚のパターンは，<br>
//!　　全接地  1通り <br>
//!　　1本遊脚 6通り <br>
//!　　2本遊脚 15通り <br>
//!　　3本遊脚 20通り → 実現可能なものは14通り <br>
//!		なので全部で36通りある． <br>
//! @note 拡張性皆無なので全面的に書き直したいが，根が深い問題なので，とりあえずこのままにしておく．
//! @date 2023/07/11