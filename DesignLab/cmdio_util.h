//! @file cmdio_util.h
//! @brief コマンドラインに文字を出力する関数をまとめた名前空間．

#ifndef DESIGNLAB_CMDIO_UTIL_H_
#define DESIGNLAB_CMDIO_UTIL_H_

#include <memory>
#include <string>

#include "output_detail.h"
#include "boot_mode.h"


namespace designlab
{
	//! @namespace cmdio
	//! @brief コマンドラインに文字を出力する関数をまとめた名前空間．command line input/output の略．
	//! @details コマンドラインに文字を出力する関数をまとめた名前空間．
	//! @n 設定ファイルで出力の許可をしていない場合は，出力しない機能を実装したくて作成した．
	//! @n 結果的に，コマンドラインに出力する関数がすべてこの名前空間に入っている←
	namespace cmdio
	{
		constexpr int HORIZONTAL_LINE_LENGTH = 70; //!< 水平線の長さ．


		//! @brief 出力するメッセージをどこまで許可するかを設定する関数．
		//! @n 例えば kError に設定すると，kError 未満の出力( kInfo とか kDebug とか)はされない．
		//! @param [in] limit 出力するメッセージをどこまで許可するかを設定する列挙型
		void SetOutputLimit(OutputDetail limit);

		//! @brief そもそも出力をするかを設定する関数．
		//! @n falseに設定しても システムメッセージは出力される．
		void SetDoOutput(bool do_output);


		//! @brief コマンドラインに文字を出力する関数．
		//! @param [in] str 出力する文字列
		//! @param [in] detail 出力する文字列の詳細 (デフォルトではkSystem)
		//! @param [in] wait_cin cinを待つかどうか (デフォルトではfalse)，
		//! @n cinとの同期を切っているため，cinを待たないと出力順がおかしくなることがある．
		void Output(const std::string& str, OutputDetail detail = OutputDetail::kSystem, bool wait_cin = false);

		//! @brief コマンドラインで改行をする関数．
		//! @param [in] num 改行する回数 (デフォルトでは1回)
		//! @param [in] detail 出力する際の優先度 (デフォルトではkSystem)
		void OutputNewLine(int num = 1, OutputDetail detail = OutputDetail::kSystem);

		//! @brief コマンドラインに水平線を出力する関数．
		//! @param [in] double_line 二重線にするかどうか (デフォルトではfalse)
		//! @param [in] detail 出力する際の優先度 (デフォルトではkSystem)
		void OutputHorizontalLine(bool double_line = false, OutputDetail detail = OutputDetail::kSystem);

		//! @brief 中央に文字を出力する関数
		//! @param [in] str 出力する文字列
		//! @param [in] detail 出力する際の優先度 (デフォルトではkSystem)
		void OutputCenter(const std::string& str, OutputDetail detail = OutputDetail::kSystem);

		//! @brief 右端に文字を出力する関数
		//! @param [in] str 出力する文字列
		//! @param [in] detail 出力する際の優先度 (デフォルトではkSystem)
		void OutputRight(const std::string& str, OutputDetail detail = OutputDetail::kSystem);

		//! @brief コマンドラインにこのソフトのタイトルを出力する関数．
		void OutputTitle();

		//! @brief コマンドラインにGraphViewerのタイトルを出力する関数．
		void OutputGraphViewerTitle();


		//! @brief 入力待ちをする関数．
		//! @param [in] str 入力待ちをする際に出力する文字列
		void WaitAnyKey(const std::string& str = "入力待ち，Enterキーを押してください．");

		//! @brief 整数を入力する関数．
		//! @param [in] min 入力する整数の最小値
		//! @param [in] max 入力する整数の最大値
		//! @param [in] default_num デフォルトで入力する整数
		//! @param [in] str 入力待ちをする際に出力する文字列
		//! @return int 入力された整数
		int InputInt(int min, int max, int default_num, const std::string& str = "整数を入力してください．");

		//! @brief yesかnoを入力する関数．返り値でyesならtrue，noならfalseを受け取る．
		//! @param [in] str 入力待ちをする際に出力する文字列
		//! @return bool yesならばtrue，noならばfalse
		bool InputYesNo(const std::string& str = "y / nで入力してください．");

		//! @brief このアプリの起動モードを選択する関数．
		//! @return EBootMode 選択した起動モード
		EBootMode SelectBootMode();

	}	// namespace dl_cio

}	// namespace designlab


#endif	// DESIGNLAB_CMDIO_UTIL_H_