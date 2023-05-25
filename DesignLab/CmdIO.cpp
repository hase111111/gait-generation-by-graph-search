#include "CmdIO.h"
#include <iostream>
#include "LegState.h"

using namespace LegState;

void CmdIO::outputString(const std::string _str)
{
	std::cout << _str << std::endl;
}

void CmdIO::outputLNODE(const SNode _node)
{
	std::cout << "-------------------------グラフ探索で得られた歩容パターン---------------------------" << std::endl;
	std::cout << std::endl;
	
	//重心パターン
	std::cout << "COM_type = " << LegState::getComPatternState(_node.leg_state) << std::endl;

	//脚の遊脚・接地状態
	std::cout << "Legs(0,1,2,3,4,5)" << std::endl;
	std::cout << "Leg is Grounded : ";
	for (int i = 0; i < HexapodConst::LEG_NUM; ++i) { std::cout << (isGrounded(_node.leg_state, i) ? "ground " : "lifted "); }
	std::cout << std::endl;

	//脚の階層
	std::cout << "Hierarchy : ";
	for (int i = 0; i < HexapodConst::LEG_NUM; ++i) { std::cout << LegState::getLegState(_node.leg_state, i); }
	std::cout << std::endl;

	//脚位置
	std::cout << "Leg Postion : " << std::endl;
	for (int i = 0; i < HexapodConst::LEG_NUM; i++) 
	{
		std::cout << "Leg[" << i << "] = " << _node.Leg[i] << std::endl;
	}
	std::cout << std::endl;

	//重心位置
	std::cout << "global_center_of_mass = " << _node.global_center_of_mass << std::endl;

	std::cout << "node_height = " << _node.node_height << "\n";
	std::cout << "debug = " << _node.debug << "\n";
	if (_node.debug % 100 / 10 == 2) std::cout << "今回2 脚の踏みかえ動作" << std::endl;
	if (_node.debug % 100 / 10 == 3) std::cout << "今回3 重心の水平移動" << std::endl;
	if (_node.debug % 100 / 10 == 4) std::cout << "今回4 脚の水平移動" << std::endl;
	if (_node.debug % 100 / 10 == 6) std::cout << "今回6 重心の上下移動" << std::endl;
	if (_node.debug % 10 == 2) std::cout << "前回2 脚の踏みかえ動作\t";
	if (_node.debug % 10 == 3) std::cout << "前回3 重心の水平移動\t";
	if (_node.debug % 10 == 4) std::cout << "前回4 脚の水平移動\t";
	if (_node.debug % 10 == 6) std::cout << "前回6 重心の上下移動\t";

	std::cout << "必要な高さ移動量 = " << _node.target_delta_comz << "\n";
	std::cout << "実際の高さ移動量 = " << _node.delta_comz << "\n";
	std::cout << "探索したノード数 = " << _node.last_node_num << "\n";
	std::cout << "探索時間 = " << _node.time << "\n";

	std::cout << "---------------------------------1動作分終了----------------------------------------------" << std::endl;
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
