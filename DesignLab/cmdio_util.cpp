#include "cmdio_util.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "cassert_define.h"


namespace
{
	// このような名前のない名前空間を匿名名前空間という

	// 匿名名前空間にいれた値は，このファイルからのみアクセス可能なグローバル変数となる
	// アクセスする場合は :: を先頭につける

	// (ここまでするならクラスにしときゃよかったかも)


	// 出力制限，この値未満のメッセージの出力は行われない
	OutputDetail output_limit = OutputDetail::kSystem;

	// falseの場合，出力を行わない(システムメッセージは除く)
	bool do_output = true;

	// 初期化を既に行ったかどうか
	bool is_initialized = false;
}


namespace designlab
{
	namespace cmdio
	{
		void SetOutputLimit(const OutputDetail limit)
		{
			::output_limit = limit;

			if (!::is_initialized) 
			{
				//これを記述しておくと実行速度が早くなる．そのかわりprintfを使用できない．詳しはReferenceを参照
				std::cin.tie(&std::cout);
				std::ios_base::sync_with_stdio(true);

				::is_initialized = true;
			}
		}

		void SetDoOutput(const bool do_output_)
		{
			::do_output = do_output_;
		}


		void Output(const std::string& str, const OutputDetail detail)
		{
			assert(is_initialized);	// SetOutputLimitを呼んでから使用すること.


			// 出力を許可している　かつ　出力する文字列の詳細が設定ファイルで許可されている場合　または
			// 出力を許可していない　かつ　出力する文字列の詳細がシステムメッセージの場合

			if ((detail <= ::output_limit && do_output) || (detail == OutputDetail::kSystem && !do_output))
			{
				std::cout << str << std::endl;
			}
		}

		void OutputCenter(const std::string& str, const OutputDetail detail)
		{
			//改行ごとに文字列を取り出す
			std::stringstream ss(str);
			std::string line;

			while (std::getline(ss, line))
			{
				if (kHorizontalLineLength > line.length())
				{
					std::string space;

					for (int i = 0; i < (kHorizontalLineLength - line.length()) / 2; i++)
					{
						space += " ";
					}

					Output(space + line, detail);
				}
				else
				{
					Output(line, detail);
				}

			}
		}

		void OutputRight(const std::string& str, const OutputDetail detail)
		{
			//改行ごとに文字列を取り出す
			std::stringstream ss(str);
			std::string line;

			while (std::getline(ss, line))
			{
				if (kHorizontalLineLength > line.length())
				{
					std::string space;

					for (int i = 0; i < kHorizontalLineLength - line.length(); i++)
					{
						space += " ";
					}

					Output(space + line, detail);
				}
				else
				{
					Output(line, detail);
				}

			}
		}


		void OutputNewLine(const int num, const OutputDetail detail)
		{
			if (num <= 0) { return; }

			for (int i = 0; i < num; i++)
			{
				Output("", detail);
			}
		}

		void OutputHorizontalLine(const std::string& line_visual, const OutputDetail detail)
		{
			if (line_visual.size() != 1) { return; }

			std::string str;

			for (int i = 0; i < kHorizontalLineLength; i++)
			{
				str += line_visual;
			}

			Output(str, detail);
		}

		void OutputTitle(const std::string& title_name, bool output_copy_right)
		{
			OutputDetail detail = OutputDetail::kSystem;

			OutputNewLine(1, detail);
			OutputHorizontalLine("=", detail);
			OutputNewLine(1, detail);
			OutputCenter(title_name, detail);
			OutputNewLine(1, detail);

			if (output_copy_right) 
			{
				OutputRight("Coprright 2015 - 2023 埼玉大学 設計工学研究室  ", detail);
				OutputNewLine(1, detail);
			}

			OutputHorizontalLine("=", detail);
			OutputNewLine(1, detail);
		}


		void WaitAnyKey(const std::string& str)
		{
			Output(str, OutputDetail::kSystem);

			//何かキーを押すまで待機
			system("PAUSE");
		}

		int InputInt(const int min, const int max, const int default_num, const std::string& str)
		{
			assert(min <= max);	// minはmaxより小さい．

			Output(str + " (" + std::to_string(min) + " ～ " + std::to_string(max) + ") : ", OutputDetail::kSystem);

			std::string input_str;
			std::cout << std::flush;
			std::cin >> input_str;

			int res = default_num;

			try
			{
				res = std::stoi(input_str);	// 入力された文字列をint型に変換

				if (res < min || res > max)
				{
					Output(
						"入力された値「" + input_str + "」は範囲外です．デフォルトの値，「" + std::to_string(default_num) + "」を使用します．",
						OutputDetail::kSystem
					);

					res = default_num;
				}
			}
			catch (...)
			{
				// stoiで例外が発生した場合，ここに処理が飛ぶ

				Output(
					"入力された値「" + input_str + "」は評価できません．デフォルトの値，「" + std::to_string(default_num) + "」を使用します．", 
					OutputDetail::kSystem
				);

				res = default_num;
			}

			return res;
		}

		bool InputYesNo(const std::string& str)
		{
			Output(str + " ( y / n ) ", OutputDetail::kSystem);

			while (true)
			{
				std::string input_str;
				std::cout << std::flush;
				std::cin >> input_str;


				if (input_str == "y" || input_str == "yes" || input_str == "Y" || input_str == "Yes" || input_str == "YES")
				{
					return true;
				}
				else if (input_str == "n" || input_str == "no" || input_str == "N" || input_str == "No" || input_str == "NO")
				{
					return false;
				}

				Output("入力された値「" + input_str + "」は評価できません．y / nで入力してください．", OutputDetail::kSystem);
			}

		}

		BootMode SelectBootMode()
		{
			Output("起動モードを選択してください", OutputDetail::kSystem);
			Output("0: シミュレーション", OutputDetail::kSystem);
			Output("1: グラフビューワー", OutputDetail::kSystem);
			Output("2: 表示テスト", OutputDetail::kSystem);
			Output("3: 結果の確認", OutputDetail::kSystem);
			Output("other: デフォルトのモード ( " + std::to_string(BootMode::kSimulation) + " )", OutputDetail::kSystem);
			OutputNewLine(1,OutputDetail::kSystem);

			int input = InputInt(0, static_cast<int>(BootMode::kResultViewer), 0);

			if (input == 0)
			{
				return BootMode::kSimulation;
			}
			else if (input == 1)
			{
				return BootMode::kViewer;
			}
			else if (input == 2)
			{
				return BootMode::kDisplayTest;
			}
			else if (input == 3)
			{
				return BootMode::kResultViewer;
			}
			else
			{
				return BootMode::kSimulation;
			}
		}

	}	// namespace cmdio

}	// namespace designlab