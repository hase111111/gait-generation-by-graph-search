#include "designlab_cmdio.h"

#include <iostream>
#include <string>
#include <cstdlib>



void dl_cio::outputNewLine(const SApplicationSettingRecorder* setting, const int num, const EOutputPriority priority)
{
	if (num < 0) { return; }

	if (priority <= (*setting).cmd_permission)
	{
		for (int i = 0; i < num; i++)
		{
			output(setting, "", priority);
		}
	}
}


void dl_cio::outputHorizontalLine(const SApplicationSettingRecorder* setting, const bool double_line, const EOutputPriority priority)
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

	output(setting, str, priority);
}


void dl_cio::outputCenter(const SApplicationSettingRecorder* setting, const std::string str, const EOutputPriority priority)
{
	std::string space;

	for (int i = 0; i < (HORIZONTAL_LINE_LENGTH - str.length()) / 2; i++)
	{
		space += " ";
	}

	output(setting, space + str, priority);
}


void dl_cio::outputRight(const SApplicationSettingRecorder* setting, const std::string str, const EOutputPriority priority)
{
	std::string space;

	for (int i = 0; i < HORIZONTAL_LINE_LENGTH - str.length(); i++)
	{
		space += " ";
	}

	output(setting, space + str, priority);
}


void dl_cio::waitAnyKey(const SApplicationSettingRecorder* setting, const std::string str, const EOutputPriority priority)
{
	output(setting, str, priority, true);

	if (priority <= (*setting).cmd_permission)
	{
		//何かキーを押すまで待機
		system("PAUSE");
	}
}


int dl_cio::inputInt(const SApplicationSettingRecorder* setting, const int min, const int max, const int default_num, const std::string str)
{
	output(setting, str + " (" + std::to_string(min) + " ~ " + std::to_string(max) + ") : ", EOutputPriority::SYSTEM, true);

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


bool dl_cio::inputYesNo(const SApplicationSettingRecorder* setting, const std::string str)
{
	output(setting, str, EOutputPriority::SYSTEM, true);

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

		output(setting, "入力された値「" + input_str + "」は評価できません．y / nで入力してください．", EOutputPriority::SYSTEM, true);
	}

}


EBootMode dl_cio::selectBootMode(const SApplicationSettingRecorder* setting)
{
	dl_cio::output(setting, "起動モードを選択してください", EOutputPriority::SYSTEM);
	dl_cio::output(setting, "0: シミュレーション", EOutputPriority::SYSTEM);
	dl_cio::output(setting, "1: グラフビューワー", EOutputPriority::SYSTEM);
	dl_cio::output(setting, "2: 表示テスト", EOutputPriority::SYSTEM);
	dl_cio::output(setting, "3: 結果の確認", EOutputPriority::SYSTEM);
	dl_cio::output(setting, "other: デフォルトのモード ( " + std::to_string((*setting).default_mode) + " )", EOutputPriority::SYSTEM);
	dl_cio::outputNewLine(setting, 1, EOutputPriority::SYSTEM);

	int input = dl_cio::inputInt(setting, 0, static_cast<int>(EBootMode::RESULT_VIEWER), 0);

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
		return (*setting).default_mode;
	}
}


void dl_cio::outputTitle(const SApplicationSettingRecorder* setting)
{
	outputNewLine(setting, 1);
	outputHorizontalLine(setting, true, EOutputPriority::SYSTEM);
	outputNewLine(setting, 1);
	outputCenter(setting, "DesignLab", EOutputPriority::SYSTEM);

	std::string str = "Version " + std::to_string((*setting).version_major) + '.' + std::to_string((*setting).version_minor) + '.' + std::to_string((*setting).version_patch);
	outputRight(setting, str, EOutputPriority::SYSTEM);
	outputNewLine(setting, 1);
	outputRight(setting, "Created by DesignLab", EOutputPriority::SYSTEM);
	outputRight(setting, "All rights reserved", EOutputPriority::SYSTEM);
	outputNewLine(setting, 1);
	outputHorizontalLine(setting, true, EOutputPriority::SYSTEM);
	outputNewLine(setting, 1);
}

void dl_cio::outputGraphViewerTitle(const SApplicationSettingRecorder* setting)
{
	outputNewLine(setting, 1);
	outputHorizontalLine(setting, true, EOutputPriority::SYSTEM);
	outputNewLine(setting, 1);
	outputCenter(setting, "GraphViewer", EOutputPriority::SYSTEM);
	outputNewLine(setting, 1);
	outputHorizontalLine(setting, true, EOutputPriority::SYSTEM);
	outputNewLine(setting, 1);
}
