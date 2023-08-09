#include "CmdIO.h"
#include <iostream>
#include "LegState.h"

using namespace LegStateEdit;

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

void CmdIO::outputSimulateResult(const int _loop_num, const SimulateResult& _res)
{
	std::cout << "歩行の成功率\t" << _res.getClearNum() * 100 / _loop_num << "％" << std::endl;
	std::cout << "1シミュレーション当たりの平均到達距離\t" << _res.getDistanceMoveYSum() / _loop_num << "[mm/シミュレーション]" << std::endl;
	std::cout << "最大到達距離\t" << _res.getDistanceMoveYMax() << "[mm]" << std::endl;
	std::cout << "最小到達距離\t" << _res.getDistanceMoveYMin() << "[mm]" << std::endl;
	std::cout << "1歩容パターン生成あたりの平均探索時間\t" << _res.getGatePatternGenerateTimeSum() / _res.getGatePatternGenerateSum() << "[s]" << std::endl;
	std::cout << "最大探索時間\t" << _res.getGatePatternGenerateTimeMax() << "[s]" << std::endl;
	std::cout << "最小探索時間\t" << _res.getGatePatternGenerateTimeMin() << "[s]" << std::endl << std::endl;

	std::cout << "繰り返し動作で中断した割合\t" << _res.getFailedByGatePatternLoop() * 100 / _loop_num << "％" << std::endl;
	std::cout << "歩容パターンが得られず中断した割合\t" << _res.getFailedByNoGatePattern() * 100 / _loop_num << "％" << std::endl;
	std::cout << "歩容パターンを生成した総回数\t" << _res.getGatePatternGenerateSum() << "回" << std::endl;
	std::cout << "歩容パターン生成に掛かった総時間\t" << _res.getGatePatternGenerateTimeSum() << "秒" << std::endl;
	std::cout << "Y方向への合計移動距離\t" << _res.getDistanceMoveYSum() << "mm" << std::endl;
	std::cout << "1歩容パターン生成あたりの移動距離\t" << _res.getDistanceMoveYSum() / _res.getGatePatternGenerateSum() << "mm/動作" << std::endl;
	std::cout << "Y方向への平均移動速度\t" << _res.getDistanceMoveYSum() / _res.getGatePatternGenerateTimeSum() << "mm/秒" << std::endl;
}

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

void dl_io::setOutputPriority(const EOutputPriority priority)
{
	CmdIO::setOutputPriority(priority);
}

void dl_io::output(const std::string str, const EOutputPriority priority)
{
}

EOutputPriority CmdIO::m_output_priority = EOutputPriority::INFO;