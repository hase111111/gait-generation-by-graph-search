#include "designlab_cmdio.h"

#include <iostream>
#include <string>

#include "leg_state.h"


using namespace dl_leg;


void CmdIO::outputString(const std::string _str)
{
	std::cout << _str << std::endl;
}

void CmdIO::outputNode(const SNode& _node, const int _num) const
{
	std::cout << std::endl;
	if (_num < 0) { std::cout << "Node parameter is ..." << std::endl; }
	else if (_num == 0) { std::cout << "First node parameter is ..." << std::endl; }
	else { std::cout << "[Node Number " << _num << " ] Node parameter is ..." << std::endl; }

	std::cout << _node << std::endl;

	std::cout << std::endl;
}

void CmdIO::outputErrorMessageInGraphSearch(const std::string _err_mes) const
{
	std::cout << "Graph search failed." << std::endl;
	std::cout << "\t" << "by " << _err_mes << std::endl;
	std::cout << std::endl;
}

void CmdIO::outputGraphSearchStaretMessage(const int _simu_num) const
{
	std::cout << std::endl;
	std::cout << "---------------------- Starting new graph search ----------------------" << std::endl;
	std::cout << "This is the " << getOrdinalNumber(_simu_num) << " simulation." << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

//void CmdIO::outputSimulateResult(const int _loop_num, const SimulateResult& _res)
//{
//	std::cout << "歩行の成功率\t" << _res.getClearNum() * 100 / _loop_num << "％" << std::endl;
//	std::cout << "1シミュレーション当たりの平均到達距離\t" << _res.getDistanceMoveYSum() / _loop_num << "[mm/シミュレーション]" << std::endl;
//	std::cout << "最大到達距離\t" << _res.getDistanceMoveYMax() << "[mm]" << std::endl;
//	std::cout << "最小到達距離\t" << _res.getDistanceMoveYMin() << "[mm]" << std::endl;
//	std::cout << "1歩容パターン生成あたりの平均探索時間\t" << _res.getGatePatternGenerateTimeSum() / _res.getGatePatternGenerateSum() << "[s]" << std::endl;
//	std::cout << "最大探索時間\t" << _res.getGatePatternGenerateTimeMax() << "[s]" << std::endl;
//	std::cout << "最小探索時間\t" << _res.getGatePatternGenerateTimeMin() << "[s]" << std::endl << std::endl;
//
//	std::cout << "繰り返し動作で中断した割合\t" << _res.getFailedByGatePatternLoop() * 100 / _loop_num << "％" << std::endl;
//	std::cout << "歩容パターンが得られず中断した割合\t" << _res.getFailedByNoGatePattern() * 100 / _loop_num << "％" << std::endl;
//	std::cout << "歩容パターンを生成した総回数\t" << _res.getGatePatternGenerateSum() << "回" << std::endl;
//	std::cout << "歩容パターン生成に掛かった総時間\t" << _res.getGatePatternGenerateTimeSum() << "秒" << std::endl;
//	std::cout << "Y方向への合計移動距離\t" << _res.getDistanceMoveYSum() << "mm" << std::endl;
//	std::cout << "1歩容パターン生成あたりの移動距離\t" << _res.getDistanceMoveYSum() / _res.getGatePatternGenerateSum() << "mm/動作" << std::endl;
//	std::cout << "Y方向への平均移動速度\t" << _res.getDistanceMoveYSum() / _res.getGatePatternGenerateTimeSum() << "mm/秒" << std::endl;
//}

std::string CmdIO::getOrdinalNumber(const int _num) const
{
	//参考 https://qiita.com/Seiten_Minagawa/items/58a0b2b0cfd578c9bedc

	std::string res = std::to_string(_num);

	if (_num / 10 % 10 == 1) { res += "th"; }
	else
	{
		switch (_num % 10)
		{
		case 1:
			res += "st";
			break;

		case 2:
			res += "nd";
			break;

		case 3:
			res += "rd";
			break;

		default:
			res += "th";
			break;
		}
	}

	return res;
}


void dl_cio::outputNewLine(const SApplicationSettingRecorder* setting, const int num, const EOutputPriority priority)
{
	if (num < 0)return;

	if (priority <= (*setting).cmd_permission)
	{
		for (int i = 0; i < num; i++)
		{
			std::cout << std::endl;
		}
	}
}


void dl_cio::outputHorizontalLine(const SApplicationSettingRecorder* setting, const bool double_line, const EOutputPriority priority)
{
	if (double_line)
	{
		output(setting, "======================================================================", priority);
	}
	else
	{
		output(setting, "----------------------------------------------------------------------", priority);
	}
}


void dl_cio::waitAnyKey(const SApplicationSettingRecorder* setting, const std::string str, const EOutputPriority priority)
{
	output(setting, str, priority, true);

	if (priority <= (*setting).cmd_permission)
	{
		std::string wait;
		std::cin >> wait;
	}
}


int dl_cio::inputInt(const SApplicationSettingRecorder* setting, const int min, const int max, const int default_num, const std::string str)
{
	int res = default_num;

	output(setting, str + " (" + std::to_string(min) + " ~ " + std::to_string(max) + ") : ", EOutputPriority::SYSTEM, true);

	std::string input_str;
	std::cin >> input_str;

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

EBootMode dl_cio::selectBootMode(const SApplicationSettingRecorder* setting)
{
	dl_cio::output(setting, "起動モードを選択してください", EOutputPriority::SYSTEM);
	dl_cio::output(setting, "0: シミュレーション", EOutputPriority::SYSTEM);
	dl_cio::output(setting, "1: グラフビューワー", EOutputPriority::SYSTEM);
	dl_cio::output(setting, "2: デフォルトのモード ( " + std::to_string((*setting).default_mode) + " )", EOutputPriority::SYSTEM);

	int input = dl_cio::inputInt(setting, 0, 2, 0);

	if (input == 0)
	{
		return EBootMode::SIMULATION;
	}
	else if (input == 1)
	{
		return EBootMode::VIEWER;
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
	output(setting, "                           DesignLab                        ", EOutputPriority::SYSTEM);

	std::string str = "Version " + std::to_string((*setting).version_major) + '.' + std::to_string((*setting).version_minor) + '.' + std::to_string((*setting).version_patch);
	output(setting, "                                              " + str, EOutputPriority::SYSTEM);
	outputNewLine(setting, 1);
	output(setting, "                                        Created by DesignLab", EOutputPriority::SYSTEM);
	output(setting, "                                         All rights reserved", EOutputPriority::SYSTEM);
	outputHorizontalLine(setting, true, EOutputPriority::SYSTEM);
	outputNewLine(setting, 1);
}
