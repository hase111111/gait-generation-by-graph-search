#pragma once

#include <vector>

#include "hexapod_const.h"
#include "com_type.h"


//! @namespace dl_leg
//! @date 2023/08/11
//! @author 長谷川
//! @brief 脚状態を編集するための関数をまとめた名前空間．DesignLab Leg Functionsの略．
//! @details 脚状態って言ってるけど重心タイプも入ってる．C++だからint型は32bit．
//! @n 1脚の脚状態を4bitで表す 最上位0:遊脚,1:接地．　残り3bitで離散化した脚位置．
//! @n 離散化した脚位置は基準位置を4として，それより前にあるなら4より大きい数字，後ろにあるならば4より小さい数字． @n
//! @n [離散化した脚位置] 
//!	@n 7 .. 3    (0は使わない) 
//! @n 6 4 2  
//! @n 5 .. 1 @n
//! @n [bitのデータ]  	
//! @n 余り　重心パターン　脚５　脚４　脚３　脚２　脚１　脚０
//! @n 1111 1111　　　　　1111　1111　1111　1111　1111　1111 @n
//! @n 脚は右前脚を0として時計回りに 0 ～ 5 ．
//! @n 定数は基本的には編集しないように．脚の離散化方法を変更する時以外編集する必要はないはず．
//! @n DISCRETE_NUMは脚の離散化数を表しています．この値は他のファイルから参照することが多いが，それ以外の値は他のファイルから参照しないように，
//! @n どうしても参照したい場合，この名前空間に新しい関数を追加するべき．
namespace dl_leg
{
	constexpr int DISCRETE_NUM = 7;					//!< 脚状態の離散化数．leg_state.hの通り7通り．

	constexpr int LEG_POS_MASKBIT = 0b0111;			//!< 脚位置は4bitの下位三桁で管理されるので，そこをマスクする

	constexpr int LEG_GROUNDED_MASKBIT = 0b1000;	//!< 脚が接地しているかを表すビットをマスクする．(接地しているならば1．遊脚ならば0．)

	constexpr int LEG_STATE_MASKBIT = 0b1111;		//!< 脚状態は4bitで管理されるので，そこをマスクする

	constexpr int SHIFT_TO_COM_NUM = HexapodConst::LEG_NUM * 4;			//!< 重心パターンを保存するビットまで行くために，どれだけビットをシフトするか．

	constexpr int COM_STATE_MASKBIT = (0b1111 << SHIFT_TO_COM_NUM);		//!< 重心パターンを保存するビットをマスクするビット．


	//! @enum EDiscreteLegPos
	//! @date 2023/09/10
	//! @author 長谷川
	//! @breif 離散化された脚位置を表すenum
	enum class EDiscreteLegPos
	{
		LOWER_BACK = 1,		//!< 現在の位置より後方かつ下方にある
		BACK = 2,			//!< 現在の位置より後方にある
		UPPER_BACK = 3,		//!< 現在の位置より後方かつ上方にある
		CENTER = 4,			//!< 現在の位置にある
		LOWER_FRONT = 5,	//!< 現在の位置より前方かつ下方にある
		FRONT = 7,			//!< 現在の位置より前方にある
		UPPER_FRONT = 6,	//!< 現在の位置より前方かつ上方にある
	};


	//! @brief 脚状態を作成して返す関数．脚状態は重心パターン，脚の接地・遊脚，離散化した脚位置のデータが含まれる．
	//! @param [in] com_pattern どの重心パターンか．詳しくはComType.hに記述．
	//! @param [in] is_ground 脚が接地しているかを表すbool型の配列．接地しているならばtrue．遊脚しているならばfalse
	//! @param [in] discretized_leg_pos 離散化した脚位置を表す変数． 1 ～ 7 の範囲で与える．範囲外ならば自動的に 4 になる．
	//! @return int 作成した脚状態を返す．
	int makeLegState(ComType::EComPattern com_pattern, const bool is_ground[HexapodConst::LEG_NUM], const int discretized_leg_pos[HexapodConst::LEG_NUM]);


	//! @brief 脚番号 leg_index 0 ～ 5 に応じて，その脚が接地しているかを調べる．@n 脚は右前脚を0番として，時計回りに0,1,2,3,4,5となる．左前足が5番．
	//! @param [in] leg_state 現在の脚状態
	//! @param [in] leg_index どの脚を調べるか． 0 ～ 5 の整数で入力する．
	//! @return bool 脚が接地しているならばtrueを返す．
	bool isGrounded(int leg_state, int leg_index);


	//! @brief 接地している脚の本数を返す関数．
	//! @param [in] leg_state 現在の脚状態
	//! @return int 接地している脚の本数
	int getGroundedLegNum(int leg_state);


	//! @brief 遊脚している脚の本数を返す関数．
	//! @param [in] leg_state 現在の脚状態
	//! @return int 遊脚している脚の本数
	int getLiftedLegNum(int leg_state);


	//! @brief 接地している脚の脚番号0～5を，引数res_indexで参照渡しする関数
	//! @param [in] leg_state 現在の脚状態
	//! @param [out] res_index 接地している脚の脚番号を格納する変数．
	void getGroundedLegNumWithVector(int leg_state, std::vector<int>* res_index);


	//! @brief 遊脚している脚の脚番号0～5を，引数_res_numberで参照渡しする関数
	//! @param [in] leg_state 現在の脚状態
	//! @param [out] res_index 遊脚している脚の脚番号を格納する変数．
	void getLiftedLegNumWithVector(int leg_state, std::vector<int>* res_index);


	//! @brief 脚状態を取得する．
	//! @param [in] leg_state 現在の脚状態
	//! @param [in] leg_index どの脚の状態を取得するか． 0 ～ 5 の整数で入力する．
	//! @return int 脚の状態を返す．1 ～ 7 の整数．
	int getLegState(int leg_state, int leg_index);


	//! @brief 重心パターンを取得する．0b0000 ～ 0b1000 まで
	//! @param [in] leg_state 現在の脚状態
	//! @return int 重心パターンを返す．0b0000 ～ 0b1000
	int getComPatternState(int leg_state);


	//! @brief 脚の情報を 0～15 の状態に変更する．引数の値がおかしいならばfalseを出力する．
	//! @param [in] leg_index 脚の番号 0～5
	//! @param [in] new_discretized_leg_pos 新しい脚状態 1～15
	//! @param [in,out] leg_state 現在の脚状態，これをもとに新しい脚状態を作成する．
	//! @return bool 変更に成功したらtrue
	bool changeLegState(int leg_index, int new_discretized_leg_pos, int* leg_state);


	// 脚の情報を 0～7 の状態に変更する．引数の値がおかしいならばfalseを出力する．遊脚を表すbitはそのまま
	// _leg_state 変更する脚状態．_leg_num どの脚を変更するか 0～5．_new_state 新しい脚状態 1～7
	bool changeLegStateKeepTopBit(int& _leg_state, int _leg_num, int _new_state);


	//脚の接地・遊脚情報を変更する．第1引数の_leg_stateを参照渡しして変更する
	void changeGround(int& _leg_state, int _leg_num, bool _ground);


	//! @brief 重心のデータを変更する．
	//! @param [in] leg_state 現在の脚状態
	//! @param [in] new_com_pattern 新しい重心パターン
	//! @return int 変更した重心パターンを返す．
	int changeComPattern(int leg_state, ComType::EComPattern new_com_pattern);


	//! @brief 脚を表す番号は，右前脚を 0 として， 0 ～ 5 の範囲．その範囲外ならばfalseを返す．
	//! @param [in] num 脚の本数
	//! @return bool 0 ～ 5 の範囲内ならばtrue
	constexpr bool isAbleLegNum(const int num)
	{
		if (0 <= num && num < HexapodConst::LEG_NUM) { return true; }	// 0 ～ 5なら true

		return false;
	}


	constexpr bool isAbleLegState(const int state)
	{
		if (state == 8) { return false; }		// 8 (0b1000) なら false

		if (0 < state && state < 15) { return true; }		// 1 ～ 15なら true

		return false;
	}

	//! @brief 脚の上下の変化回数を出力する
	//! @param [in] leg_state_first 1つ目の脚状態
	//! @param [in] leg_state_second 2つ目の脚状態
	//! @return int 脚の上下の変化回数
	int getLegUpDownCount(int leg_state_first, int leg_state_second);
}


//! @file leg_state.h
//! @date 2023/08/11
//! @author 長谷川
//! @brief このプログラムでは脚状態をint型(32bit)の情報で表す．そのデータを処理するための関数をまとめたもの
//! @n 行数 : @lineinfo
