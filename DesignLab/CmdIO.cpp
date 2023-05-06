#include "CmdIO.h"
#include <iostream>
#include "LegState.h"

using namespace LegState;

void CmdIO::outputString(const std::string _str)
{
	std::cout << _str << std::endl;
}

void CmdIO::outputLNODE(const LNODE _node)
{
	std::cout << "-------------------------�O���t�T���œ���ꂽ���e�p�^�[��---------------------------" << std::endl;

	std::cout << "\n";
	//std::cout << "leg_condition = " << std::bitset<24>(LNODE1.leg_state) << "\n";
	std::cout << "COM_type = " << (_node.leg_state >> shift_COM) << "\n";
	std::cout << "Legs(0,1,2,3,4,5) \nv =\t";
	for (int i = 0; i < 6; ++i) std::cout << isGrounded(_node.leg_state, i);
	std::cout << std::endl << "kaisou =";
	for (int i = 0; i < 6; ++i)	std::cout << numOfLegPosi(_node.leg_state, i);
	std::cout << std::endl;
	//std::cout << "center_of_mass = " << LNODE1.center_of_mass << "\n";
	for (int i = 0; i < 6; i++) { std::cout << "Leg[" << i << "] = "; myvector::VectorOutPut(_node.Leg[i]); }
	std::cout << "global_center_of_mass = "; myvector::VectorOutPut(_node.global_center_of_mass);
	//std::cout << "parent = " << LNODE1.parent << "\n";
	std::cout << "node_height = " << _node.node_height << "\n";
	std::cout << "debug = " << _node.debug << "\n";
	if (_node.debug % 100 / 10 == 2) std::cout << "����2 �r�̓��݂�������" << std::endl;
	if (_node.debug % 100 / 10 == 3) std::cout << "����3 �d�S�̐����ړ�" << std::endl;
	if (_node.debug % 100 / 10 == 4) std::cout << "����4 �r�̐����ړ�" << std::endl;
	if (_node.debug % 100 / 10 == 6) std::cout << "����6 �d�S�̏㉺�ړ�" << std::endl;
	if (_node.debug % 10 == 2) std::cout << "�O��2 �r�̓��݂�������\t";
	if (_node.debug % 10 == 3) std::cout << "�O��3 �d�S�̐����ړ�\t";
	if (_node.debug % 10 == 4) std::cout << "�O��4 �r�̐����ړ�\t";
	if (_node.debug % 10 == 6) std::cout << "�O��6 �d�S�̏㉺�ړ�\t";

	std::cout << "�K�v�ȍ����ړ��� = " << _node.target_delta_comz << "\n";
	std::cout << "���ۂ̍����ړ��� = " << _node.delta_comz << "\n";
	std::cout << "�T�������m�[�h�� = " << _node.last_node_num << "\n";
	std::cout << "�T������ = " << _node.time << "\n";
	//std::cout << "pitch = " << LNODE1.pitch << "\n";
	//std::cout << "roll = " << LNODE1.roll << "\n";
	//std::cout << "yaw = " << LNODE1.yaw << "\n";

	std::cout << "---------------------------------1���앪�I��----------------------------------------------" << std::endl;
}

void CmdIO::outputSimulateResult(const int _loop_num, const SimulateResult& _res)
{
	std::cout << "���s�̐�����\t" << _res.getClearNum() * 100 / _loop_num << "��" << std::endl;
	std::cout << "1�V�~�����[�V����������̕��ϓ��B����\t" << _res.getDistanceMoveYSum() / _loop_num << "[mm/�V�~�����[�V����]" << std::endl;
	std::cout << "�ő哞�B����\t" << _res.getDistanceMoveYMax() << "[mm]" << std::endl;
	std::cout << "�ŏ����B����\t" << _res.getDistanceMoveYMin() << "[mm]" << std::endl;
	std::cout << "1���e�p�^�[������������̕��ϒT������\t" << _res.getGatePatternGenerateTimeSum() / _res.getGatePatternGenerateSum() << "[s]" << std::endl;
	std::cout << "�ő�T������\t" << _res.getGatePatternGenerateTimeMax() << "[s]" << std::endl;
	std::cout << "�ŏ��T������\t" << _res.getGatePatternGenerateTimeMin() << "[s]" << std::endl << std::endl;

	std::cout << "�J��Ԃ�����Œ��f��������\t" << _res.getFailedByGatePatternLoop() * 100 / _loop_num << "��" << std::endl;
	std::cout << "���e�p�^�[��������ꂸ���f��������\t" << _res.getFailedByNoGatePattern() * 100 / _loop_num << "��" << std::endl;
	std::cout << "���e�p�^�[���𐶐���������\t" << _res.getGatePatternGenerateSum() << "��" << std::endl;
	std::cout << "���e�p�^�[�������Ɋ|������������\t" << _res.getGatePatternGenerateTimeSum() << "�b" << std::endl;
	std::cout << "Y�����ւ̍��v�ړ�����\t" << _res.getDistanceMoveYSum() << "mm" << std::endl;
	std::cout << "1���e�p�^�[������������̈ړ�����\t" << _res.getDistanceMoveYSum() / _res.getGatePatternGenerateSum() << "mm/����" << std::endl;
	std::cout << "Y�����ւ̕��ψړ����x\t" << _res.getDistanceMoveYSum() / _res.getGatePatternGenerateTimeSum() << "mm/�b" << std::endl;
}