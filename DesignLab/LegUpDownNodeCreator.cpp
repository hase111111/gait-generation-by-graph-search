#include "LegUpDownNodeCreator.h"
#include "NodeEdit.h"
#include "ComType.h"
#include "LegState.h"

void LegUpDownNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//まずは重心の変化が一切ないものを追加する．
	{
		SNode _same_node = _current_node;
		node_edit::changeNextNode(_same_node, _current_num, getNextMove(_current_node.next_move));
		_output_graph.push_back(_same_node);
	}


	//脚の遊脚・接地によって生じるとりうる重心をcomtypeとして仕分けている．(詳しくはComtype.hを参照)．まずは全てtrueにしておく．
	bool _is_able_type[ComType::COM_TYPE_NUM];

	for (int i = 0; i < ComType::COM_TYPE_NUM; i++) 
	{
		_is_able_type[i] = true; 
	}

	//重心が現在どこにあるか(前よりか真ん中か...)などのパラメータはこのcom patternで仕分けている．(詳しくはComtype.hを参照)．これを取得する．
	int _com_pattern = LegState::getComPatternState(_current_node.leg_state);

	//com patternよりとることができないcom typeを全てfalseにする．
	ComType::checkAbleComTypeFromComPattern(_com_pattern, _is_able_type);



	//次に脚が地面に接地可能か調べる．
	bool _is_groundable[HexapodConst::LEG_NUM];

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//すでに接地している脚は接地可能に決まっているのでtrueにする．
		if (LegState::isGrounded(_current_node.leg_state, i) == true) 
		{
			_is_groundable[i] = true; 
		}
		else 
		{
			_is_groundable[i] = false; 
		}
	}



}

EHexapodMove LegUpDownNodeCreator::getNextMove(const EHexapodMove& _last_move) const
{
	//重心の上下移動→これ→重心の平行移動
	//脚の平行移動→これ→重心の上下移動

	if (_last_move == EHexapodMove::COM_UP_DOWN) { return EHexapodMove::COM_MOVE; }
	else { return EHexapodMove::COM_UP_DOWN; }
}

bool LegUpDownNodeCreator::isGroundableLeg(const int _leg_num, const SNode& _current_node, SNode& _output_ground_pos)
{
	if (mp_Map == nullptr)return false;

	//Leg2と比較して，どこにあるかによって以下のように離散化している．
	switch (LegState::getLegState(_current_node.leg_state, _leg_num))
	{
	case 1:
		//後ろ，下
		break;

	case 2:
		//後ろ，中
		break;

	case 3:
		//後ろ，上
		break;

	case 4:
		//真ん中
		break;

	case 5:
		//　前，下
		break;

	case 6:
		//　前，中
		break;

	case 7:
		//　前，上
		break;

	default:
		break;
	}

	return false;
}
