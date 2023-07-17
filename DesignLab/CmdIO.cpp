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

	////重心パターン
	//std::cout << "\tCOM_type = " << LegStateEdit::getComPatternState(_node.leg_state) << std::endl;
	//std::cout << std::endl;

	////脚の遊脚・接地状態
	//std::cout << "\tLegs(0,1,2,3,4,5)" << std::endl;
	//std::cout << "\tGround : ";
	//for (int i = 0; i < HexapodConst::LEG_NUM; ++i) { std::cout << (isGrounded(_node.leg_state, i) ? "ground " : "lifted "); }
	//std::cout << std::endl;

	////脚の階層
	//std::cout << "\tHierarchy : ";
	//for (int i = 0; i < HexapodConst::LEG_NUM; ++i) { std::cout << LegStateEdit::getLegState(_node.leg_state, i); }
	//std::cout << std::endl;

	////脚位置
	//std::cout << "\tLeg Position : " << std::endl;
	//for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	//{
	//	std::cout << "\t\tLeg[" << i << "] = " << _node.leg_pos[i] << "\t\tLeg2[" << i << "] = " << _node.Leg2[i] << std::endl;
	//}
	//std::cout << std::endl;

	////重心位置
	//std::cout << "\tglobal_center_of_mass = " << _node.global_center_of_mass << std::endl;

	////回転姿勢
	//std::cout << "\tRotate : roll = " << _node.rot.roll << ", pitch = " << _node.rot.pitch << ", yaw = " << _node.rot.yaw << std::endl;

	////次動作
	//std::cout << std::endl;
	//std::cout << "\t(Next Move : " << std::to_string(_node.next_move) << ")" << std::endl;
	//std::cout << "\t(Depth : " << static_cast<int>(_node.depth) << ")" << std::endl;
	//std::cout << "\t(parent number : " << _node.parent_num << ")" << std::endl;


	//std::cout << "\tnode_height = " << _node.node_height << "\n";
	//std::cout << "\tdebug = " << _node.debug << "\n";
	//if (_node.debug % 100 / 10 == 2) std::cout << "\t今回2 脚の踏みかえ動作" << std::endl;
	//if (_node.debug % 100 / 10 == 3) std::cout << "\t今回3 重心の水平移動" << std::endl;
	//if (_node.debug % 100 / 10 == 4) std::cout << "\t今回4 脚の水平移動" << std::endl;
	//if (_node.debug % 100 / 10 == 6) std::cout << "\t今回6 重心の上下移動" << std::endl;
	//if (_node.debug % 10 == 2) std::cout << "\t前回2 脚の踏みかえ動作\t";
	//if (_node.debug % 10 == 3) std::cout << "\t前回3 重心の水平移動\t";
	//if (_node.debug % 10 == 4) std::cout << "\t前回4 脚の水平移動\t";
	//if (_node.debug % 10 == 6) std::cout << "\t前回6 重心の上下移動\t";
	//std::cout << "\t必要な高さ移動量 = " << _node.target_delta_comz << std::endl;
	//std::cout << "\t実際の高さ移動量 = " << _node.delta_comz << std::endl;
	//std::cout << "\t探索したノード数 = " << _node.last_node_num << std::endl;
	//std::cout << "\t探索時間 = " << _node.time << std::endl;
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
