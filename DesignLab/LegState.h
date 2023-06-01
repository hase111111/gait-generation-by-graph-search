#pragma once
#include <vector>

//このプログラムでは脚状態をint型(32bit)の情報で表す
//そのデータを処理するための関数をまとめたもの

//脚状態って言ってるけど重心タイプも入ってる．C++だからint型は32bit．1脚の脚状態を4bitで表す 最上位0:遊脚,1:接地　
//残り3bitで離散化した脚位置．脚は右前脚を0として時計回りに0~5
// 
//	7   3    (0は使わない)
//	6 4 2
//	5   1
// 
//	1111    1111		1111 1111 1111 1111 1111 1111
//	余り    重心パターン脚5  脚4  脚3  脚2  脚1  脚0

namespace LegState
{
	//脚状態の離散化数．LegState.hの通り7通り．
	constexpr int DISCRETE_NUM = 7;

	//脚位置は4bitの下位三桁で管理されるので，そこをマスクする
	constexpr int LEG_POS_MASKBIT = 0b0111;

	//脚状態は4bitで管理されるので，そこをマスクする
	constexpr int LEG_STATE_MASKBIT = 0b1111;

	//重心パターンを保存するビットまで行くために，どれだけビットをシフトするか．
	constexpr int SHIFT_TO_COM_NUM = 24;

	//重心パターンを保存するビットをマスクするビット．
	constexpr int COM_STATE_MASKBIT = (0b1111 << SHIFT_TO_COM_NUM);

	//脚番号_leg_num 0～5に応じて，その脚が接地しているかを調べる
	bool isGrounded(const int _leg_state, const int _leg_num);

	//接地している脚の本数を返す関数
	int getGroundedLegNum(const int _leg_state);

	//接地している脚の脚番号0～5を，引数_res_numberで参照渡しする関数
	void getGroundedLegNumWithVector(const int _leg_state, std::vector<int>& _res_number);

	//遊脚している脚の脚番号0～5を，引数_res_numberで参照渡しする関数
	void getLiftedLegNumWithVector(const int _leg_state, std::vector<int>& _res_number);

	// 脚状態を取得する．
	int getLegState(const int _leg_state, const int _leg_num);

	//重心パターンを取得する．0b0000 ～ 0b1000 まで
	int getComPatternState(const int _leg_state);

	// 脚の情報を 0～15 の状態に変更する．引数の値がおかしいならばfalseを出力する．
	// _leg_state 変更する脚状態．_leg_num どの脚を変更するか 0～5．_new_state 新しい脚状態 1～15
	bool changeLegState(int &_leg_state, const int _leg_num, const int _new_state);

	// 脚の情報を 0～7 の状態に変更する．引数の値がおかしいならばfalseを出力する．遊脚を表すbitはそのまま
	// _leg_state 変更する脚状態．_leg_num どの脚を変更するか 0～5．_new_state 新しい脚状態 1～7
	bool changeLegStateKeepTopBit(int& _leg_state, const int _leg_num, const int _new_state);

	//脚の接地・遊脚情報を変更する．第1引数の_leg_stateを参照渡しして変更する
	void changeGround(int& _leg_state, const int _leg_num, const bool _ground);

	//脚番号が 0～5 の間に入っているならばtrue
	bool isAbleLegNum(const int _num);

	//脚状態が 1(0001)～15(1111) の間に入っているならばtrue
	bool isAbleLegState(const int _state);

	//重心位置によって
}
