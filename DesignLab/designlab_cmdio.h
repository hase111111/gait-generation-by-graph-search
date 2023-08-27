#pragma once

#include <string>

#include "node.h"
#include "application_setting_recorder.h"
#include "output_priority.h"



//! @namespace dl_cio
//! @date 2023/08/08
//! @author 長谷川
//! @brief コマンドラインに文字を出力する関数をまとめた名前空間．designlab (dl) command line input/output (cio)の略．
namespace dl_cio
{
	//! @brief コマンドラインに文字を出力する関数．
	//! @param [in] setting 設定ファイルの情報を記録した構造体のポインタ
	//! @param [in] str 出力する文字列
	//! @param [in] priority 出力する際の優先度 (デフォルトではSYSTEM)
	//! @param [in] wait_cin cinを待つかどうか (デフォルトではfalse)，cinとの同期を切っているため，cinを待たないと出力順がおかしくなることがある．
	//! @param [in] display_func_name 関数名を出力するかどうか (デフォルトではfalse)
	inline void output(const SApplicationSettingRecorder* setting, const std::string str,
		const EOutputPriority priority = EOutputPriority::SYSTEM, const bool wait_cin = false, const bool display_func_name = false)
	{
		if ((priority <= (*setting).cmd_permission && (*setting).cmd_output) || (priority == EOutputPriority::SYSTEM && !(*setting).cmd_output))
		{
			if (display_func_name)
			{
				std::cout << __func__ << " : ";
			}

			std::cout << str;

			if (wait_cin)
			{
				std::cout << std::flush;
			}

			std::cout << std::endl;
		}
	}


	//! @brief コマンドラインで改行をする関数．
	//! @param [in] setting 設定ファイルの情報を記録した構造体のポインタ
	//! @param [in] num 改行する回数 (デフォルトでは1回)
	//! @param [in] priority 出力する際の優先度 (デフォルトではSYSTEM)
	void outputNewLine(const SApplicationSettingRecorder* setting, const int num = 1, const EOutputPriority priority = EOutputPriority::SYSTEM);


	//! @brief コマンドラインにタイトルを出力する関数．
	//! @param [in] setting 設定ファイルの情報を記録した構造体のポインタ
	void outputTitle(const SApplicationSettingRecorder* setting);


	//! @brief コマンドラインに水平線を出力する関数．
	//! @param [in] setting 設定ファイルの情報を記録した構造体のポインタ
	//! @param [in] double_line 二重線にするかどうか (デフォルトではfalse)
	//! @param [in] priority 出力する際の優先度 (デフォルトではSYSTEM)
	void outputHorizontalLine(const SApplicationSettingRecorder* setting, const bool double_line = false, const EOutputPriority priority = EOutputPriority::SYSTEM);


	//! @brief 入力待ちをする関数．
	//! @param [in] str 入力待ちをする際に出力する文字列
	void waitAnyKey(const SApplicationSettingRecorder* setting, const std::string str = "入力待ち，Enterキーを押してください．",
		const EOutputPriority priority = EOutputPriority::SYSTEM);


	//! @brief 整数を入力する関数．
	//! @param [in] setting 設定ファイルの情報を記録した構造体のポインタ
	//! @param [in] min 入力する整数の最小値
	//! @param [in] max 入力する整数の最大値
	//! @param [in] default_num デフォルトで入力する整数
	//! @param [in] str 入力待ちをする際に出力する文字列
	//! @return int 入力された整数
	int inputInt(const SApplicationSettingRecorder* setting, const int min, const int max, const int default_num, const std::string str = "整数を入力してください．");


	EBootMode selectBootMode(const SApplicationSettingRecorder* setting);

}	//namespace dl_cio



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

	////シミュレーションの結果をコマンドラインに出力する
	//void outputSimulateResult(const int _loop_num, const SimulateResult& _res);

	static void setOutputPriority(const EOutputPriority priority) { m_output_priority = priority; }

	static EOutputPriority getOutputPriority() { return m_output_priority; }

private:

	//英語の数字は序数 ( 1st，2nd，3rd，4thみたいなやつ)がつくので，受け取った数値に序数を付けた文字列を返す関数．
	std::string getOrdinalNumber(const int _num) const;

	static EOutputPriority m_output_priority;	//コマンドラインに文字を出力する際の優先度
};

//! @file designlab_cmd_io.h
//! @date 2023/08/08
//! @author 長谷川
//! @brief コマンドラインに文字を出力するための名前空間．
//! @n 行数 : @lineinfo
