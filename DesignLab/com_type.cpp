
//! @author    Hasegawa
//! @copyright 埼玉大学 設計工学研究室 2023. All right reserved.

#include "com_type.h"

#include <utility>

#include <magic_enum.hpp>

#include "cassert_define.h"


// com_func 内の関数は実際に処理を行う際に計算を行うと遅くなるため，
// 初期化時に一度だけ呼び出して，結果を保存しておく．
// その値を呼び出すことで速度を上げている．
// このように無名名前空間の中に変数を宣言することで，
// このファイル内でのみ使用可能になる．こうしてできた変数に結果を保存する．
// アクセスするには，先頭に::をつける．
//! @todo クラスにするべき.
namespace
{
using designlab::com_func::LegGroundedMap;
using designlab::com_func::LegGroundedMapValue;
using designlab::leg_func::LegGroundedBit;
using designlab::enums::DiscreteComPos;
using designlab::HexapodConst;

//! @brief 脚の接地パターンを表すマップを作成する関数．初期化時に一度だけ呼び出す．
LegGroundedMap MakeLegGroundedMap()
{
    LegGroundedMap res;
    int counter = 0;


    // 脚が接地している場合1，遊脚の場合0として，6bitの数値を作成する．
    // 0番脚が遊脚，残り接地の場合 111 110 ．
    // そしてそれに 0 から始まる番号を割り振る．(管理しやすくするため．)
    // 全パターンを総当りで書いてあるけど，本当はこのリストを作成する関数を作りたい．

    // トライポット歩容に使用するパターン．
    res.insert(LegGroundedMapValue(LegGroundedBit("010101"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("101010"), counter++));

    // 6脚全て接地している場合．
    res.insert(LegGroundedMapValue(LegGroundedBit("111111"), counter++));


    // 5脚接地している場合．

    res.insert(LegGroundedMapValue(LegGroundedBit("011111"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("101111"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("110111"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("111011"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("111101"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("111110"), counter++));


    // 4脚接地している場合

    res.insert(LegGroundedMapValue(LegGroundedBit("001111"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("010111"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("011011"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("011101"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("011110"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("100111"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("101011"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("101101"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("101110"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("110011"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("110101"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("110110"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("111001"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("111010"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("111100"), counter++));


    // 3脚接地している場合．隣り合う3脚が遊脚している場合は除く(転倒してしまうため)．

    // res.insert(LegGroundedMapValue(LegGroundedBit("000111"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("001011"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("001101"), counter++));
    // res.insert(LegGroundedMapValue(LegGroundedBit("001110"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("010011"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("010110"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("011001"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("011010"), counter++));
    // res.insert(LegGroundedMapValue(LegGroundedBit("011100"), counter++));
    // res.insert(LegGroundedMapValue(LegGroundedBit("100011"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("100101"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("100110"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("101001"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("101100"), counter++));
    // res.insert(LegGroundedMapValue(LegGroundedBit("110001"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("110010"), counter++));
    res.insert(LegGroundedMapValue(LegGroundedBit("110100"), counter++));
    // res.insert(LegGroundedMapValue(LegGroundedBit("111000"), counter++));

    return std::move(res);
}

//! 脚の接地パターンに数値を割り振ったマップ．接地を1，遊脚を0として，
//! { 111111 , 0 } のような形式で代入されている．
const LegGroundedMap kLegGroundedPatternMap = MakeLegGroundedMap();

//! 脚の接地パターンの数．
const int kLegGroundedPatternNum = static_cast<int>(kLegGroundedPatternMap.size());



//! @brief leg_indexと leg_index + 1 番の脚がともに遊脚になる時に trueを返す関数．
//! 初期化用に使用している
//! @param leg_index 脚の番号．
//! @param leg_ground_pattern_index 脚の接地パターンの番号．
//! @return leg_indexと leg_index + 1 番の脚がともに遊脚になる時に true．
bool IsLegPairFree(int leg_index, int leg_ground_pattern_index)
{
    LegGroundedBit leg_ground_pattern;

    // indexから遊脚のパターンを取得する．
    try
    {
        leg_ground_pattern =
            ::kLegGroundedPatternMap.right.at(leg_ground_pattern_index);
    }
    catch (...)
    {
        return false;
    }

    // 両隣が遊脚の場合は true を返す．
    if (!leg_ground_pattern[leg_index % HexapodConst::kLegNum] &&
        !leg_ground_pattern[(leg_index + 1) % HexapodConst::kLegNum])
    {
        return true;
    }
    else
    {
        return false;
    }
}

//! @brief 重心位置から使用不可能な接地パターンを作成する関数．
//! 初期化時に一度だけ呼び出す．
std::unordered_map<DiscreteComPos, std::vector<int>> MakeLegGroundedPatternBanList()
{
    std::unordered_map<DiscreteComPos, std::vector<int>> res;


    // ロボットの体が前に寄っている時に前足が両方とも遊脚だと転倒してしまう．
    // そのため，離散化された重心から，とることができない，
    // 連続する脚が両方とも遊脚になるパターンを禁止するのがこの関数の目的である．
    std::unordered_map<DiscreteComPos, std::vector<int>> ban_leg_index_list;
    ban_leg_index_list[DiscreteComPos::kFront] = { 0, 4, 5 };
    ban_leg_index_list[DiscreteComPos::kFrontRight] = { 0, 1, 5 };
    ban_leg_index_list[DiscreteComPos::kFrontLeft] = { 3, 4, 5 };
    ban_leg_index_list[DiscreteComPos::kBack] = { 1, 2, 3 };
    ban_leg_index_list[DiscreteComPos::kBackRight] = { 0, 1, 2 };
    ban_leg_index_list[DiscreteComPos::kBackLeft] = { 2, 3, 4 };
    ban_leg_index_list[DiscreteComPos::kCenterBack] = { 0, 2, 4 };
    ban_leg_index_list[DiscreteComPos::kCenterFront] = { 1, 3, 5 };


    // DiscreteComPosの要素数だけループする．
    // magic_enum::enum_values<DiscreteComPos>() は，
    // DiscreteComPos の要素を列挙した array を返す．
    for (const auto& i : magic_enum::enum_values<DiscreteComPos>())
    {
        if (ban_leg_index_list.count(i) == 0) { continue; }

        for (auto& j : ban_leg_index_list[i])
        {
            for (int k = 0; k < ::kLegGroundedPatternNum; ++k)
            {
                if (IsLegPairFree(j, k))
                {
                    res[i].push_back(k);
                }
            }
        }
    }

    return std::move(res);
}

//! @brief 特定の脚が接地できない場合に取り得ない接地パターンを作成する関数の
//! 初期化時に一度だけ呼び出す．
std::vector<std::vector<int>> MakeLegGroundedPatternBanListFromLeg()
{
    std::vector<std::vector<int>> res;

    res.resize(designlab::HexapodConst::kLegNum);  // 脚の数だけ vector を確保する．

    // i 番脚を接地しなければ，取ることができないものを保存する．
    for (int i = 0; i < designlab::HexapodConst::kLegNum; i++)
    {
        for (int j = 0; j < ::kLegGroundedPatternNum; ++j)
        {
            // i番目のビットを確認し，立っているならば
            // (つまり，その脚を接地しなければいけないなら)，そのパターンを禁止する．
            if (::kLegGroundedPatternMap.right.at(j)[i])
            {
                res[i].push_back(j);
            }
        }
    }

    return std::move(res);
}


//! 重心位置から使用不可能な接地パターンを map で管理する．
const std::unordered_map<::designlab::enums::DiscreteComPos, std::vector<int>>
kLegGroundedPatternBanList = MakeLegGroundedPatternBanList();

//! その脚が遊脚のとき，取り得ない脚の接地パターンを管理する．
const std::vector<std::vector<int>> kLegGroundedPatternBanListFromLeg =
MakeLegGroundedPatternBanListFromLeg();

}  // namespace


namespace designlab::com_func
{

int GetLegGroundPatternNum()
{
    return kLegGroundedPatternNum;
}

leg_func::LegGroundedBit GetLegGroundedBitFromLegGroundPatternIndex(
    const int leg_ground_pattern_index)
{
    leg_func::LegGroundedBit res;

    // indexから遊脚のパターンを取得する．
    res = ::kLegGroundedPatternMap.right.at(leg_ground_pattern_index);

    return res;
}


void RemoveLegGroundPatternFromCom(enums::DiscreteComPos discrete_com_pos,
                                   boost::dynamic_bitset<>* output)
{
    assert(output != nullptr);
    assert((*output).size() == GetLegGroundPatternNum());

    // kLegGroundedPatternBanListFromLeg にキーが存在していないことや，
   // 値が GetLegGroundPatternNum() を超えてないことを確認していない．
   // エラーが出たら，そこが原因の可能性がある
    for (auto& i : kLegGroundedPatternBanList.at(discrete_com_pos))
    {
        (*output)[i] = false;
    }
}

void RemoveLegGroundPatternFromNotGroundableLeg(int not_groundable_leg_index,
                                                boost::dynamic_bitset<>* output)
{
    assert(output != nullptr);
    assert((*output).size() == GetLegGroundPatternNum());

    // kLegGroundedPatternBanListFromLeg にキーが存在していないことや，
    // 値が GetLegGroundPatternNum() を超えてないことを確認していない．
    // エラーが出たら，そこが原因の可能性がある．

    for (auto& i : kLegGroundedPatternBanListFromLeg[not_groundable_leg_index])
    {
        (*output)[i] = false;
    }
}

void RemoveLegGroundPatternFromNotFreeLeg(
    int not_lift_leg_index,
    boost::dynamic_bitset<>* output)
{
    assert(output != nullptr);
    assert((*output).size() == GetLegGroundPatternNum());

    // kLegGroundedPatternBanListFromLeg にキーが存在していないことや，
    // 値が GetLegGroundPatternNum() を超えてないことを確認していない．
    // エラーが出たら，そこが原因の可能性がある．
    boost::dynamic_bitset<> inverse_output(GetLegGroundPatternNum());

    for (auto& i : kLegGroundedPatternBanListFromLeg[not_lift_leg_index])
    {
        inverse_output[i] = true;
    }

    (*output) &= inverse_output;
}

}  // namespace designlab::com_func
