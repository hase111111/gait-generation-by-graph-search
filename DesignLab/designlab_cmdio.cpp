#include "designlab_cmdio.h"

#include <cstdlib>
#include <iostream>
#include <string>


namespace
{
	// 出力制限，この値未満のメッセージの出力は行われない
	// なお，この値はこのファイルからのみアクセス可能なグローバル変数
	OutputDetail output_limit = OutputDetail::kSystem;

	// falseの場合，出力を行わない(システムメッセージは除く)
	bool do_output = true;
}


namespace designlab
{
	namespace cmdio
	{
		void SetOutputLimit(const OutputDetail limit)
		{
			::output_limit = limit;
		}

		void SetDoOutput(bool do_output)
		{
			::do_output = do_output;
		}

		void Output(const std::string& str, const OutputDetail detail, const bool wait_cin)
		{
			// 出力を許可している　かつ　出力する文字列の詳細が設定ファイルで許可されている場合　または
			// 出力を許可していない　かつ　出力する文字列の詳細がシステムメッセージの場合

			if ((detail <= ::output_limit && do_output) || (detail == OutputDetail::kSystem && !do_output))
			{
				std::cout << str;

				if (wait_cin)
				{
					std::cout << std::flush;
				}

				std::cout << "\n";
			}
		}

		void OutputNewLine(const int num, const OutputDetail detail)
		{
			if (num < 0) { return; }

			for (int i = 0; i < num; i++)
			{
				Output("", detail);
			}
		}

		void OutputHorizontalLine(const bool double_line, const OutputDetail detail)
		{
			std::string str;

			for (int i = 0; i < HORIZONTAL_LINE_LENGTH; i++)
			{
				if (double_line)
				{
					str += "=";

				}
				else
				{
					str += "-";
				}
			}

			Output(str, detail);
		}

		void OutputCenter(const std::string& str, const OutputDetail detail)
		{
			std::string space;

			for (int i = 0; i < (HORIZONTAL_LINE_LENGTH - str.length()) / 2; i++)
			{
				space += " ";
			}

			Output(space + str, detail);
		}

		void OutputRight(const std::string& str, const OutputDetail detail)
		{
			std::string space;

			for (int i = 0; i < HORIZONTAL_LINE_LENGTH - str.length(); i++)
			{
				space += " ";
			}

			Output(space + str, detail);
		}

		void OutputTitle()
		{
			OutputNewLine();
			OutputHorizontalLine(true, OutputDetail::kSystem);
			OutputNewLine();
			OutputCenter("DesignLab", OutputDetail::kSystem);
			OutputNewLine();
			OutputRight("Created by DesignLab", OutputDetail::kSystem);
			OutputRight("All rights reserved", OutputDetail::kSystem);
			OutputNewLine();
			OutputHorizontalLine(true, OutputDetail::kSystem);
			OutputNewLine();
		}

		void OutputGraphViewerTitle()
		{
			OutputNewLine();
			OutputHorizontalLine(true, OutputDetail::kSystem);
			OutputNewLine();
			OutputCenter("GraphViewer", OutputDetail::kSystem);
			OutputNewLine();
			OutputHorizontalLine(true, OutputDetail::kSystem);
			OutputNewLine();
		}


		void WaitAnyKey(const std::string& str)
		{
			Output(str, OutputDetail::kSystem, true);

			//何かキーを押すまで待機
			system("PAUSE");
		}


		int InputInt(const int min, const int max, const int default_num, const std::string& str)
		{
			Output(str + " (" + std::to_string(min) + " ~ " + std::to_string(max) + ") : ", OutputDetail::kSystem, true);

			std::string input_str;
			std::cin >> input_str;

			int res = default_num;

			try
			{
				res = std::stoi(input_str);

				if (res < min || res > max)
				{
					res = default_num;
				}
			}
			catch (...)
			{
				res = default_num;
			}

			return res;
		}

		bool InputYesNo(const std::string& str)
		{
			Output(str, OutputDetail::kSystem, true);

			while (true)
			{
				std::string input_str;
				std::cin >> input_str;


				if (input_str == "y" || input_str == "yes" || input_str == "Y" || input_str == "Yes" || input_str == "YES")
				{
					return true;
				}
				else if (input_str == "n" || input_str == "no" || input_str == "N" || input_str == "No" || input_str == "NO")
				{
					return false;
				}

				Output("入力された値「" + input_str + "」は評価できません．y / nで入力してください．", OutputDetail::kSystem, true);
			}

		}

		EBootMode SelectBootMode()
		{
			Output("起動モードを選択してください", OutputDetail::kSystem);
			Output("0: シミュレーション", OutputDetail::kSystem);
			Output("1: グラフビューワー", OutputDetail::kSystem);
			Output("2: 表示テスト", OutputDetail::kSystem);
			Output("3: 結果の確認", OutputDetail::kSystem);
			Output("other: デフォルトのモード ( " + std::to_string(EBootMode::SIMULATION) + " )", OutputDetail::kSystem);
			OutputNewLine();

			int input = InputInt(0, static_cast<int>(EBootMode::RESULT_VIEWER), 0);

			if (input == 0)
			{
				return EBootMode::SIMULATION;
			}
			else if (input == 1)
			{
				return EBootMode::VIEWER;
			}
			else if (input == 2)
			{
				return EBootMode::DISPLAY_TEST;
			}
			else if (input == 3)
			{
				return EBootMode::RESULT_VIEWER;
			}
			else
			{
				return EBootMode::SIMULATION;
			}
		}

	}	// namespace cmdio

}	// namespace designlab