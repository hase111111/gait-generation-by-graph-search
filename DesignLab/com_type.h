
//! @file      com_type.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_COM_TYPE_H_
#define DESIGNLAB_COM_TYPE_H_

#include <bitset>
#include <unordered_map>
#include <vector>

#include <boost/bimap.hpp>
#include <boost/dynamic_bitset.hpp>

#include "discrete_com_pos.h"
#include "hexapod_const.h"
#include "leg_state.h"


namespace std
{

// boost bimapのために比較演算子を定義している．
template <size_t N>
bool operator<(const std::bitset<N>& lhs, const std::bitset<N>& rhs)
{
    return lhs.to_ulong() < rhs.to_ulong();
}

}  // namespace std


//! @namespace designlab::com_func
//! @brief 重心タイプに関する名前空間．Center of Mass Function の略．
//! @details
//! leg_stateの上位ビットにて表されているもの．詳細は波東さんの修論で
//! BFSinHierarchy・CreateComCandidate・PassFindingと様々なファイルに跨る処理
//! をまとめたくて作ったもの．
//! @n 重心位置はざっくり離散化して，8通り，
//! @n 脚の接地パターン(どの脚を上げるか，下げるか)は36通りある．
//! @n 離散化した重心位置から取ることができない接地パターンを予め取り除くのがこれら関数の役割．
//! @n
//! @n 脚の接地パターンは，
//! @n ・全接地  1通り．
//! @n ・1本遊脚 6通り．
//! @n ・2本遊脚 15通り．
//! @n ・3本遊脚 20通り → 実現可能なものは14通り．
//! @n なので全部で36通りある．
namespace designlab::com_func
{

//! @brief 脚の接地パターンを表す型．leftがビットのデータ，rightが int型の番号．
//! @n bimaps::bimap は，左右の型の両方からアクセスできる map．
//! key→value，value→keyの両方が可能．
//! @n 前述の通り脚の接地パターンは36通りある．
//! @n それぞれの接地パターンに対応する数値を割り振って管理をしやすくするために，
//! bimapを用いている．
using LegGroundedMap = boost::bimaps::bimap<leg_func::LegGroundedBit, int>;

//! 脚の接地パターンを表すマップの値の型．
using LegGroundedMapValue = LegGroundedMap::value_type;


//! @brief 脚の接地パターンの総数を返す．
//! @return 脚の接地パターンの総数．
int GetLegGroundPatternNum();

//! @brief 脚の接地パターンの番号から，その番号に該当する接地パターンを返す．
//! @param[in] leg_ground_pattern_index 脚の接地パターンの番号．
//! @return 脚の接地パターン．
//! @n 6bitのビット列．1が接地で，0が遊脚となる．
leg_func::LegGroundedBit GetLegGroundedBitFromLegGroundPatternIndex(
    int leg_ground_pattern_index);


//! @brief 離散化された重心位置から，その重心位置では取り得ない脚接地パターンを falseにする．
//! @param[in] discrete_com_pos 離散化された重心位置．
//! @param[in,out] output 脚接地パターンが実行可能ならば true，
//! 不可能なら falseにした bool型の配列．
//! @n 引数の重心位置では取り得ない脚接地パターンを falseに変更する．
void RemoveLegGroundPatternFromCom(enums::DiscreteComPos discrete_com_pos,
                                   boost::dynamic_bitset<>* output);

//! @brief 接地できない脚番号から，
//! その脚が接地できない場合に取り得ない接地パターンを falseにする．
//! @param[in] not_groundable_leg_index 接地できない脚の脚番号．
//! @param[in,out] output 脚接地パターンが実行可能ならば true，
//! 不可能なら falseにした bool型の配列．
//! @n 引数で指定した脚が接地できない場合に，取り得ない接地パターンを falseに変更する．
void RemoveLegGroundPatternFromNotGroundableLeg(int not_groundable_leg_index,
                                                boost::dynamic_bitset<>* output);

//! @brief 遊脚できない脚番号から，
//! その脚が遊脚できない場合に取り得ない接地パターンを falseにする．
//! @param[in] not_lift_leg_index 遊脚できない脚の脚番号．
//! @param[in,out] output 脚接地パターンが実行可能ならば true，
//! 不可能なら falseにした bool型の配列．
//! @n 引数で指定した脚が遊脚できない場合に取り得ない接地パターンを falseに変更する．
void RemoveLegGroundPatternFromNotFreeLeg(int not_lift_leg_index, boost::dynamic_bitset<>* output);

}  // namespace designlab::com_func


#endif  // DESIGNLAB_COM_TYPE_H_
