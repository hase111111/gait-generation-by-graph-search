#pragma once

#include <vector>
#include <unordered_map>
#include <bitset>

#include <boost/bimap.hpp>
#include <boost/dynamic_bitset.hpp>

#include "hexapod_const.h"
#include "discrete_com_pos.h"
#include "leg_state.h"


namespace std
{
	// boost bimapのために比較演算子を定義している．若干邪悪な解決法
	template <size_t N>
	bool operator<(const std::bitset<N>& lhs, const std::bitset<N>& rhs)
	{
		return lhs.to_ulong() < rhs.to_ulong();
	}
}


//! @namespace dl_com
//! @date 2023/08/09
//! @author 長谷川
//! @brief 重心タイプに関する名前空間．DesignLab Center of Mass Positon Functions の略．
//! @details leg_stateの上位bitにて表されているもの．詳細は波東さんの修論で
//! @n BFSinHierarchy・CreateComCandidate・PassFindingと様々なファイルに跨る処理をまとめたくて作ったもの. 
//! @n 重心位置はざっくり離散化して，8通り，
//! @n 脚の接地パターンは36通りある．離散化した重心位置から取ることができないものを予め取り除くのがこれら関数の役割．@n
//! @n 脚の接地パターンは，
//! @n ・全接地  1通り 
//! @n ・1本遊脚 6通り 
//! @n ・2本遊脚 15通り 
//! @n ・3本遊脚 20通り → 実現可能なものは14通り 
//!	@n なので全部で36通りある． 
namespace dl_com
{

	//前述の通り脚の接地パターンは36通りある．それぞれの遊脚に対応する数値を割り振って管理をしやすくするために，bimapを用いている．

	using LegGroundedMap = boost::bimaps::bimap<dl_leg::LegGroundedBit, int>;		//!< 脚の接地パターンを表す型．leftがbitのデータ，rightがint型の番号．

	using LegGroundedMapValue = LegGroundedMap::value_type;							//!< 脚の接地パターンを表すマップの値の型．

	//! @brief leg_indexと leg_index + 1 番の脚がともに遊脚になる時にtrueを返す関数．初期化用に使用している
	//! @param leg_index 脚の番号．
	//! @param leg_ground_pattern_index 脚の接地パターンの番号．
	//! @return leg_indexと leg_index + 1 番の脚がともに遊脚になる時にtrue．
	bool isLegPairFree(int leg_index, int leg_ground_pattern_index);

	//! @brief 脚の接地パターンを表すマップを作成する関数．初期化時に一度だけ呼び出す．
	LegGroundedMap makeLegGroundedMap();

	//! @brief 重心位置から使用不可能な接地パターンを作成する関数．初期化時に一度だけ呼び出す．
	std::unordered_map<EDiscreteComPos, std::vector<int>> makeLegGroundedPatternBanList();

	//! @brief 特定の脚が接地できない場合に取り得ない接地パターンを作成する関数の．初期化時に一度だけ呼び出す． 
	std::vector<std::vector<int>> makeLegGroundedPatternBanListFromLeg();



	//! @brief 脚の接地パターンの総数を返す．
	//! @return int 脚の接地パターンの総数．
	int getLegGroundPatternNum();


	//! @brief 脚の接地パターンの番号から，その脚の接地パターンを返す．
	//! @param [in] leg_ground_pattern_index 脚の接地パターンの番号．
	//! @return dl_leg::LegGroundedBit 脚の接地パターン．
	dl_leg::LegGroundedBit getLegGroundedBitFromLegGroundPatternIndex(int leg_ground_pattern_index);


	//! @brief 離散化された重心位置から，その重心位置では取り得ない脚接地パターンをfalseにする．
	//! @param [in] discrete_com_pos 離散化された重心位置．
	//! @param [in,out] output 脚接地パターンが実行可能ならばtrue，不可能ならfalseにしたbool型の配列．重心位置では取り得ない脚接地パターンをfalseに変更する．
	void banLegGroundPatternFromCom(EDiscreteComPos discrete_com_pos, boost::dynamic_bitset<>* output);


	//! @breif 接地できない脚番号から，その脚が接地できない場合に取り得ない接地パターンをfalseにする．
	//! @param [in] not_groundble_leg_index 接地できない脚番号．
	//! @param [in,out] output 脚接地パターンが実行可能ならばtrue，不可能ならfalseにしたbool型の配列．接地できない脚が接地できない場合に取り得ない接地パターンをfalseに変更する．
	void banLegGroundPatternFromNotGroundableLeg(int not_groundble_leg_index, boost::dynamic_bitset<>* output);


	//! @brief 遊脚できない脚番号から，その脚が遊脚できない場合に取り得ない接地パターンをfalseにする．
	//! @param [in] not_lift_leg_index 遊脚できない脚番号．
	//! @param [in,out] output 脚接地パターンが実行可能ならばtrue，不可能ならfalseにしたbool型の配列．遊脚できない脚が遊脚できない場合に取り得ない接地パターンをfalseに変更する．
	void banLegGroundPatternFromNotFreeLeg(int not_lift_leg_index, boost::dynamic_bitset<>* output);


	//constexpr int COM_PATTERN_NUM = 10;		//!< 重心パターンの数

	//constexpr int COM_TYPE_NUM = 36;		//!< 重心タイプの数

	//constexpr int BAN_LIST_ARRAY_SIZE = 23;

	//constexpr char BAN_LIST[HexapodConst::LEG_NUM][BAN_LIST_ARRAY_SIZE] =
	//{

	//{ 0, 1, 2, 3, 4, 5,       8, 9,10,11,12,   14,   16,      19,20,21,22,      25,   27,28,      31,32,   34    },	//右前脚
	//{ 0,    2, 3, 4, 5, 6, 7, 8,      11,12,13,   15,   17,      20,21,22,23,      26,   28,29,      32,33,   35 },	//右中脚
	//{ 0, 1,    3, 4, 5, 6, 7,    9,10,   12,13,14,   16,   18,      21,22,23,24,      27,   29,30,      33,34    },	//右後脚
	//{ 0, 1, 2,    4, 5, 6,    8, 9,   11,   13,14,15,   17,18,19,      22,23,24,25,      28,      31,      34,35 },	//左後脚
	//{ 0, 1, 2, 3,    5, 6, 7, 8,   10,   12,   14,15,16,   18,19,20,      23,   25,26,      29,30,   32,      35 },	//左中脚
	//{ 0, 1, 2, 3, 4,    6, 7,    9,10,11,   13,   15,   17,18,19,20,21,      24,   26,27,      30,31,   33       }	//左前脚

	//};

	//const std::vector< std::vector<int> > COMTYPE_BAN_LIST =
	//{
	//	{ 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 },	//パターン0 どの隣りあった足も上げることができない
	//	{ 18, 19,             23, 24, 25,             29, 30, 31,             35 },	//パターン6
	//	{ 18, 19, 20,             24, 25, 26,             30, 31, 32 },				//パターン1
	//	{ 19, 20, 21,             25, 26, 27,             31, 32, 33 },				//パターン2
	//	{ 20, 21, 22,             26, 27, 28,             32, 33, 34 },				//パターン3
	//	{ 21, 22, 23,             27, 28, 29,             33, 34, 35 },				//パターン4
	//	{ 18,             22, 23, 24,             28, 29, 30,             34, 35 },	//パターン5
	//	{ 18,     20,     22,     24,     26,     28,     30,     32,     34, },	//パターン7
	//	{ 19,     21,     23,     25,     27,     29,     31,     33,     35 }		//パターン8
	//};


	////重心タイプから，接地脚を1，遊脚を0としたビットを出力する関数．該当しないならば全てfalseを返す．getComTypeFromBitの逆の処理．
	//void getGroundLegFromComType(const int com_type, bool output_ground_leg[HexapodConst::LEG_NUM]);

	//// CCCより得られるcom patternを用いて，とりえないcom typeをvectorで返す
	//void getDonotUseComTypeFromComPattern(const int com_pattern, std::vector<int>& output);

	//// CCCより得られるcom patternを用いて，とりえないcom typeをすべてfalseにする．_com_type_able_arrayは全36個のcom typeが使用可能かどうかを表すbool型の配列．この値を編集する．
	//void checkAbleComTypeFromComPattern(const int com_pattern, bool com_type_able_array[COM_TYPE_NUM]);

	////接地することのできない脚から，とりえないcom typeをすべてfalseにする．_com_type_able_arrayは全36個のcom typeが使用可能かどうかを表すbool型の配列．この値を編集する．
	//void checkAbleComTypeFromNotGroundableLeg(const int not_groundble_leg, bool output_able_comtype[COM_TYPE_NUM]);

	////遊脚することのできない脚から，とりえないcom typeをすべてfalseにする．_com_type_able_arrayは全36個のcom typeが使用可能かどうかを表すbool型の配列．この値を編集する．
	//void checkAbleComTypeFromNotFreeLeg(const int not_free_leg_num, bool output_able_comtype[COM_TYPE_NUM]);

} // namespace dl_com



//! @file com_type.h
//! @date 2023/08/09
//! @author 長谷川
//! @brief 重心タイプを扱うための名前空間．
//! @n 行数 : @lineinfo