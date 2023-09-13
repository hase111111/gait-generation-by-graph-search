#pragma once

#include <vector>
#include <string>

#include "hexapod_const.h"
#include "discrete_com_pos.h"


//! @namespace dl_com
//! @date 2023/08/09
//! @author 長谷川
//! @brief 重心タイプに関する名前空間
//! @details leg_stateの上位bitにて表されているもの．詳細は波東さんの修論で
//! @n BFSinHierarchy・CreateComCandidate・PassFindingと様々なファイルに跨る処理をまとめたくて作ったもの. 
//! @n ・重心パターン … 10通りある．leg_state の上位bitにて表現されるもの 
//! @n ・重心タイプ  … 36通りある．ground_leg から可能なものに数字が割り振ってある．
//!	@n 	と，長谷川は定義した．波東先輩はこれをこれらを全てタイプ(COMType)として読んでいるので区別するためにそう定義した. @n
//! @n 脚の遊脚のパターンは，
//! @n ・全接地  1通り 
//! @n ・1本遊脚 6通り 
//! @n ・2本遊脚 15通り 
//! @n ・3本遊脚 20通り → 実現可能なものは14通り 
//!	@n なので全部で36通りある． 
//! @note 拡張性皆無なので全面的に書き直したいが，根が深い問題なので，とりあえずこのままにしておく．
namespace dl_com
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


	//接地している脚をtrueとしたbool型の配列と，重心パターンから，可能なものかを出力する
	bool isAbleCoM(int com_pattern, const bool is_ground[HexapodConst::LEG_NUM]);

	//接地している脚をtrueした配列から，重心タイプを出力する関数．該当しないならば負の値を返す
	char getComTypeFromGroundLeg(const bool is_ground[HexapodConst::LEG_NUM]);

	//脚状態から，重心タイプを出力する関数．該当しないならば負の値を返す
	char getComTypeFromLegState(int leg_state);

	//接地脚を1，遊脚を0としたビットから，重心タイプを出力する関数．該当しないならば負の値を返す
	char getComTypeFromBit(const int bit);

	//重心タイプから，接地脚を1，遊脚を0としたビットを出力する関数．該当しないならば全てfalseを返す．getComTypeFromBitの逆の処理．
	void getGroundLegFromComType(const int com_type, bool output_ground_leg[HexapodConst::LEG_NUM]);

	// CCCより得られるcom patternを用いて，とりえないcom typeをvectorで返す
	void getDonotUseComTypeFromComPattern(const int com_pattern, std::vector<int> output);

	// CCCより得られるcom patternを用いて，とりえないcom typeをすべてfalseにする．_com_type_able_arrayは全36個のcom typeが使用可能かどうかを表すbool型の配列．この値を編集する．
	void checkAbleComTypeFromComPattern(const int com_pattern, bool com_type_able_array[COM_TYPE_NUM]);

	//接地することのできない脚から，とりえないcom typeをすべてfalseにする．_com_type_able_arrayは全36個のcom typeが使用可能かどうかを表すbool型の配列．この値を編集する．
	void checkAbleComTypeFromNotGroundableLeg(const int not_groundble_leg, bool output_able_comtype[COM_TYPE_NUM]);

	//遊脚することのできない脚から，とりえないcom typeをすべてfalseにする．_com_type_able_arrayは全36個のcom typeが使用可能かどうかを表すbool型の配列．この値を編集する．
	void checkAbleComTypeFromNotFreeLeg(const int not_free_leg_num, bool output_able_comtype[COM_TYPE_NUM]);

} // namespace dl_com



//! @file com_type.h
//! @date 2023/08/09
//! @author 長谷川
//! @brief 重心タイプを扱うための名前空間．
//! @n 行数 : @lineinfo