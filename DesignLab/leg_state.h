
//! @file      leg_state.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_LEG_STATE_H_
#define DESIGNLAB_LEG_STATE_H_

#include <array>
#include <bitset>
#include <vector>

#include "discrete_com_pos.h"
#include "discrete_leg_pos.h"
#include "hexapod_const.h"


//! @namespace designlab::leg_func
//! @brief このプログラムでは脚状態をビット(28bit)の情報で表す．そのデータを処理するための関数．
//! @details
//! 脚状態って言ってるけど離散化された重心位置も入ってる．
//! @n 1脚の脚状態を4bitで表す 最上位0:遊脚,1:接地．　残り3bitで離散化した脚位置．
//! @n 離散化した脚位置は基準位置を4として，それより前にあるなら4より大きい数字，
//! 後ろにあるならば4より小さい数字．
//! @n
//! @subsubsection [離散化した脚位置]
//! 7___3 (0は使わない)
//! @n 6_4_2
//! @n 5___1
//! @n
//! @n [bitのデータ]
//! @n 重心パターン　脚５　脚４　脚３　脚２　脚１　脚０
//! @n 1111　　　　　1111　1111　1111　1111　1111　1111
//! @n
//! @n 脚は右前脚を0として時計回りに 0 ～ 5 ．
//! @n 定数は基本的には編集しないように．脚の離散化方法を変更する時以外編集する必要はないはず．
namespace designlab::leg_func
{

// 使用する型の定義

//! 脚位置を表すビット数．離散化された脚位置は3bit，遊脚・接地は1bit．あわせて4bit．
constexpr int kLegPosBitNum = 4;

constexpr int kComPosBitNum = 4;  //!< 重心パターンを表すビット数．

//! 脚状態を保存するビット数．28bit．
constexpr int kLegStateBitNum = HexapodConst::kLegNum * kLegPosBitNum + kComPosBitNum;

using LegStateBit = std::bitset<kLegStateBitNum>;  //!< 脚状態を保存する型．28bitのビット型．

//! 脚の遊脚・接地を表す型．6bitのビット型．接地が 1 遊脚が 0．
using LegGroundedBit = std::bitset<HexapodConst::kLegNum>;



// ビットをマスクするための定数

//! 脚位置は4bitの下位三桁で管理されるので，そこをマスクする．
constexpr LegStateBit kLegPosMaskBit(0b0111);

//! 脚が接地しているかを表すビットをマスクする．(接地しているならば1．遊脚ならば0．)
constexpr LegStateBit kLegGrouededMaskBit(0b1000);

//! 脚状態は4bitで管理されるので，そこをマスクする．
constexpr LegStateBit kLegStateMaskBit(0b1111);


//! 重心パターンを保存するビットまで行くために，どれだけビットをシフトするか．
constexpr int kShiftToComNum = HexapodConst::kLegNum * 4;

//! 重心パターンを保存するビットをマスクするビット．
constexpr LegStateBit kComStateMaskBit = (0b1111 << kShiftToComNum);



//! @brief 脚状態を作成して返す関数．脚状態は重心パターン，
//! 脚の接地・遊脚，離散化した脚位置のデータが含まれる．
//! @param [in] discrete_com_pos どの重心パターンか．詳しくは com_type.h に記述．
//! @param [in] is_ground 脚が接地しているかを表す bool型の配列．
//! 接地しているならば true．遊脚しているならば false．
//! @param [in] discretized_leg_pos 離散化した脚位置を表す変数．
//! @return LegStateBit 作成した脚状態を返す．
LegStateBit MakeLegStateBit(
  enums::DiscreteComPos discrete_com_pos,
  const std::array<bool, HexapodConst::kLegNum>& is_ground,
  const std::array<enums::DiscreteLegPos, HexapodConst::kLegNum>& discretized_leg_pos);


//! @brief 脚番号 leg_index 0 ～ 5 に応じて，その脚が接地しているかを調べる．
//! @n 脚は右前脚を0番として，時計回りに0,1,2,3,4,5となる．左前足が5番．
//! @param[in] leg_state_bit 現在の脚状態．
//! @param[in] leg_index どの脚を調べるか． 0 ～ 5 の整数で入力する．
//! @retval true 脚が接地している．
//! @retval false 脚が遊脚している．
bool IsGrounded(const LegStateBit& leg_state_bit, int leg_index);

//! @brief 脚が接地しているなら1，遊脚を0としたビット列で遊脚・接地脚の状態を返す．
//! @n 例えば 0 番脚のみが遊脚しているなら 0b111 110 を返す．
//! @param[in] leg_state 現在の脚状態．
//! @return 脚が接地しているなら1，遊脚を0としたビット列で遊脚・接地脚の状態を返す．
LegGroundedBit GetLegGroundedBit(const LegStateBit& leg_state);

//! @brief 接地している脚の本数を返す関数．
//! @param[in] leg_state 現在の脚状態．
//! @return 接地している脚の本数．
int GetGroundedLegNum(const LegStateBit& leg_state);

//! @brief 遊脚している脚の本数を返す関数．
//! @param[in] leg_state 現在の脚状態．
//! @return 遊脚している脚の本数．
int GetLiftedLegNum(const LegStateBit& leg_state);

//! @brief 接地している脚の脚番号0～5を，引数で参照渡しする関数．
//! @param[in] leg_state 現在の脚状態．
//! @param[out] res_index 接地している脚の脚番号を格納する変数．空であること．
void GetGroundedLegIndexByVector(const LegStateBit& leg_state, std::vector<int>* res_index);

//! @brief 遊脚している脚の脚番号0～5を，引数_res_numberで参照渡しする関数．
//! @param[in] leg_state 現在の脚状態．
//! @param[out] res_index 遊脚している脚の脚番号を格納する変数．空であること．
void GetLiftedLegIndexByVector(const LegStateBit& leg_state, std::vector<int>* res_index);


//! @brief 脚状態を取得する．
//! @param[in] leg_state 現在の脚状態．
//! @param[in] leg_index どの脚の状態を取得するか． 0 ～ 5 の整数で入力する．
//! @return 離散化された脚の位置を返す．
enums::DiscreteLegPos GetDiscreteLegPos(const LegStateBit& leg_state, int leg_index);

//! @brief 現在の脚状態から重心パターンを取得する．
//! @param[in] leg_state 現在の脚状態．
//! @return 重心パターンを返す．
enums::DiscreteComPos GetDiscreteComPos(const LegStateBit& leg_state);


//! @brief 脚の情報を変更する．
//! @param[in] leg_index 脚の番号 0～5．
//! @param[in] new_discretized_leg_pos 新しい脚状態．
//! @param[in] is_ground 脚が接地しているかを表す．接地しているならば true．
//! 遊脚しているならば false．
//! @param[in,out] leg_state 現在の脚状態，これをもとに新しい脚状態を作成する．
void ChangeLegState(int leg_index, enums::DiscreteLegPos new_discretized_leg_pos,
                    bool is_ground, LegStateBit* leg_state);

//! @brief 脚の状態を変更する．遊脚を表すビットはそのまま．
//! @param[in] leg_index 脚の番号．0～5の範囲内でなければアサートで止まる．
//! @param[in] new_discretized_leg_pos 新しい脚状態．
//! @param[in,out] leg_state 現在の脚状態，これをもとに新しい脚状態を作成する．
void ChangeDiscreteLegPos(int leg_index, enums::DiscreteLegPos new_discretized_leg_pos,
                          LegStateBit* leg_state);

//! @brief 脚の接地・遊脚情報を変更する．
//! @param[in] leg_index 脚の番号．0～5の範囲内でなければアサートで止まる．
//! @param[in] is_ground 脚が接地しているかを表す．接地しているならば true．
//! 遊脚しているならば false．
//! @param[in,out] leg_state 現在の脚状態，これをもとに新しい脚状態を作成する．
void ChangeGround(int leg_index, bool is_ground, LegStateBit* leg_state);

//! @brief 全ての脚の接地・遊脚情報を変更する．
//! @param[in] is_ground_list 脚が接地しているかを表す．接地しているならば true．
//! 遊脚しているならば false．
//! @param[in,out] leg_state 現在の脚状態，これをもとに新しい脚状態を作成する．
void ChangeAllLegGround(const LegGroundedBit& is_ground_list, LegStateBit* leg_state);

//! @brief 重心のデータを変更する．
//! @param[in] leg_state 現在の脚状態．
//! @param[in] new_com_pattern 新しい重心パターン．
//! @return 変更した重心パターンを返す．
void ChangeDiscreteComPos(enums::DiscreteComPos new_com_pattern, LegStateBit* leg_state);


}  // namespace designlab::leg_func


#endif  // DESIGNLAB_LEG_STATE_H_
