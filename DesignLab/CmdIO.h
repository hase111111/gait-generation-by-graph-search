#pragma once
#include "SimulateResult.h"
#include "Node.h"
#include <string>

//コマンドラインに文字を出力するクラス．mainに直接std::coutを書いてもいいのだが，見やすく整理する意味をかねてこのクラスに分けておいた．
class CmdIO final 
{
public:
	CmdIO() = default;
	~CmdIO() = default;

	void outputString(const std::string _str);

	//ノードの様子をコマンドラインに出力する．_numに整数値を入れると，現在の動作が何動作目か出力してくれる．
	void outputNode(const SNode& _node, const int _num = -1) const;

	//グラフ探索が失敗した時のエラーメッセージを出力する．
	void outputErrorMessageInGraphSearch(const std::string _err_mes) const;

	//グラフ探索開始時のメッセージを出力する．引数にて，何回目のシミュレーションなのかを取得する．
	void outputGraphSearchStaretMessage(const int _simu_num) const;

	//シミュレーションの結果をコマンドラインに出力する
	void outputSimulateResult(const int _loop_num, const SimulateResult& _res);

private:

	//英語の数字は序数 ( 1st，2nd，3rd，4thみたいなやつ)がつくので，受け取った数値に序数を付けた文字列を返す関数．
	std::string getOrdinalNumber(const int _num) const;
};
