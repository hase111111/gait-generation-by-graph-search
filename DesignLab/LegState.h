#pragma once
#include <vector>

//このプログラムでは脚状態をint型(32bit)の情報で表す
//そのデータを処理するための関数をまとめたもの

namespace LegState
{
	//脚位置は4bitの下位三桁で管理されるので，そこをマスクする
	const int LEG_POS_MASKBIT = 0b0111;

	//脚番号_leg_num 0～5に応じて，その脚が接地しているかを調べる
	bool isGrounded(const int _leg_state, const int _leg_num);

	//接地している脚の本数を返す関数
	int getGroundedLegNum(const int _leg_state);

	//接地している脚の脚番号0～5を，引数_res_numberで参照渡しする関数
	void getGroundedLegNumWithVector(const int _leg_state, std::vector<int>& _res_number);

	//遊脚している脚の脚番号0～5を，引数_res_numberで参照渡しする関数
	void getLiftedLegNumWithVector(const int _leg_state, std::vector<int>& _res_number);
}
