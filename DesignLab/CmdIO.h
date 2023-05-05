#pragma once
#include "SimulateResult.h"
#include "listFunc.h"
#include <string>

//コマンドラインに文字を出力するclass 
//main.cpp がごちゃごちゃして見づらいので作ってみた

class CmdIO final 
{
public:
	CmdIO() = default;
	~CmdIO() = default;

	void outputString(const std::string _str);

	//ノードの様子をコマンドラインに出力する
	void outputLNODE(const LNODE _node);

	//シミュレーションの結果をコマンドラインに出力する
	void outputSimulateResult(const int _loop_num, const SimulateResult& _res);
};
