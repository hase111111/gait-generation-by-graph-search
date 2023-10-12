//! @file cmdio_util.h
//! @brief コマンドラインに文字を出力する関数．

#ifndef DESIGNLAB_CMDIO_UTIL_H_
#define DESIGNLAB_CMDIO_UTIL_H_

#include <string>

#include "output_detail.h"
#include "boot_mode.h"


namespace designlab
{
	//! @namespace designlab::cmdio
	//! @brief コマンドラインに文字を出力する関数をまとめた名前空間．command line input/output の略．
	//! @details コマンドラインに文字を出力する関数をまとめた名前空間．
	//! @n 設定ファイルで出力の許可をしていない場合は，出力しない機能を実装したくて作成した．
	//! @n 結果的に，コマンドラインに出力する関数がすべてこの名前空間に入っている←
	namespace cmdio
	{
		constexpr int kHorizontalLineLength = 100; //!< 水平線の長さ．


		//! @brief 出力するメッセージをどこまで許可するかを設定する関数．
		//! @n この関数を呼び出してから出ないと，他の関数を使えない．
		//! @n 例えば kError に設定すると，kError 未満の出力( kInfo とか kDebug とか)はされない．
		//! @n 逆に kDebug に設定すると，すべての出力がされる．
		//! @n 1度呼び出したら，プログラム終了まで設定は有効となる．
		//! @param [in] limit 出力するメッセージをどこまで許可するか
		void SetOutputLimit(OutputDetail limit);

		//! @brief そもそも出力をするかを設定する関数．
		//! @n falseに設定しても システムメッセージは出力される．
		//! @param [in] do_output 出力をするかどうか
		void SetDoOutput(bool do_output);


		//! @brief コマンドラインに文字を出力する関数．
		//! @n SetOutputLimit() で設定した出力の許可範囲内であれば出力される．
		//! @n 必ずSetOutputLimit()を呼び出してから使うこと．
		//! @param [in] str 出力する文字列
		//! @param [in] detail 出力する文字列の詳細
		void Output(const std::string& str, OutputDetail detail);

		//! @brief 中央に文字を出力する関数．文字列が長すぎる場合は普通に出力される．
		//! @param [in] str 出力する文字列
		//! @param [in] detail 出力する文字列の詳細
		void OutputCenter(const std::string& str, OutputDetail detail);

		//! @brief 右端に文字を出力する関数．文字列が長すぎる場合は普通に出力される．
		//! @param [in] str 出力する文字列
		//! @param [in] detail 出力する文字列の詳細
		void OutputRight(const std::string& str, OutputDetail detail);


		//! @brief コマンドラインで改行をする関数．
		//! @param [in] num 改行する回数．0以下の値を入れると何もしない．
		//! @param [in] detail 出力する文字列の詳細
		void OutputNewLine(int num, OutputDetail detail);

		//! @brief コマンドラインに水平線を出力する関数．
		//! @param [in] line_visual 水平線の見た目．'-'ならば水平線，'='ならば二重水平線
		//! @n 2文字以上の文字列を入れると動作しない．
		//! @param [in] detail 出力する文字列の詳細
		void OutputHorizontalLine(const std::string& line_visual, OutputDetail detail);

		//! @brief コマンドラインにこのソフトのタイトルを出力する関数．
		//! @n 必ず，OutputDetailがkSystemで出力される．
		//! @param [in] str 出力する文字列
		//! @param [in] output_copy_right コピーライトを出力するかどうか (デフォルトではfalse)
		void OutputTitle(const std::string& title_name, bool output_copy_right = false);


		//! @brief 入力待ちをする関数．
		//! @param [in] str 入力待ちをする際に出力する文字列
		void WaitAnyKey(const std::string& str = "入力待ち，Enterキーを押してください．");

		//! @brief 整数を入力する関数．
		//! @n 必ず，OutputDetailがkSystemで出力される．
		//! @param [in] min 入力する整数の最小値
		//! @param [in] max 入力する整数の最大値
		//! @param [in] default_num デフォルトで入力する整数
		//! @param [in] str 入力待ちをする際に出力する文字列
		//! @return int 入力された整数
		int InputInt(int min, int max, int default_num, const std::string& str = "整数を入力してください．");

		//! @brief yesかnoを入力する関数．返り値でyesならtrue，noならfalseを受け取る．
		//! @n 必ず，OutputDetailがkSystemで出力される．
		//! @param [in] str 入力待ちをする際に出力する文字列
		//! @return bool yesならばtrue，noならばfalse
		bool InputYesNo(const std::string& str = "よろしいですか？");

		//! @brief このアプリの起動モードを選択する関数．
		//! @n 必ず，OutputDetailがkSystemで出力される．
		//! @return BootMode 選択した起動モード
		BootMode SelectBootMode();

	}	// namespace dl_cio

}	// namespace designlab


#endif	// DESIGNLAB_CMDIO_UTIL_H_