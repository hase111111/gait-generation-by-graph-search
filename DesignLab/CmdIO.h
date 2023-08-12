#pragma once

#include <string>

#include "SimulateResult.h"
#include "Node.h"


//! @enum EOutputPriority
//! @date 2023/08/08
//! @author 長谷川
//! @brief コマンドラインに文字を出力する際の優先度．
enum class EOutputPriority
{
	ALWAYS,		//常に出力する
	FATAL,		//致命的なエラー
	ERROR_MES,		//エラー
	WARNING,	//警告
	INFO,		//情報
};


//! @namespace dl_io
//! @date 2023/08/08
//! @author 長谷川
//! @brief コマンドラインに文字を出力する関数をまとめた名前空間．
namespace dl_io
{
	//! @brief コマンドラインに文字を出力する際の優先度を設定する関数．
	//! @param [in] priority 出力する際の優先度
	void setOutputPriority(const EOutputPriority priority);

	//! @brief コマンドラインに文字を出力する関数．
	//! @param [in] str 出力する文字列
	//! @param [in] priority 出力する際の優先度 (デフォルトではALWAYS)
	void output(const std::string str, const EOutputPriority priority = EOutputPriority::ALWAYS);

}	//namespace dl_io

//! @class CmdIO
//! @date 2023/06/17
//! @author 長谷川
//! @brief コマンドラインに文字を出力するクラス．mainに直接std::coutを書いてもいいのだが，見やすく整理する意味をかねてこのクラスに分けておいた．
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

	static void setOutputPriority(const EOutputPriority priority) { m_output_priority = priority; }

	static EOutputPriority getOutputPriority() { return m_output_priority; }

private:

	//英語の数字は序数 ( 1st，2nd，3rd，4thみたいなやつ)がつくので，受け取った数値に序数を付けた文字列を返す関数．
	std::string getOrdinalNumber(const int _num) const;

	static EOutputPriority m_output_priority;	//コマンドラインに文字を出力する際の優先度
};

//! @file cmd_io.h
//! @date 2023/08/08
//! @author 長谷川
//! @brief コマンドラインに文字を出力するための名前空間．
//! @n 行数 : @lineinfo
