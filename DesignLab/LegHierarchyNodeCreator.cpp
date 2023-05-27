#include "LegHierarchyNodeCreator.h"
#include "LegState.h"
#include "NodeEdit.h"

void LegHierarchyNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//現在，接地している脚の本数を数える
	const int _touch_ground_leg_num = LegState::getGroundedLegNum(_current_node.leg_state);

	//遊脚している脚の本数によって処理をする
	if (_touch_ground_leg_num == HexapodConst::LEG_NUM - 1)
	{
		// 1 本遊脚している．
		create1LegLifted(_current_node, _current_num, _output_graph);
	}
	else if (_touch_ground_leg_num == HexapodConst::LEG_NUM - 2)
	{
		// 2 本遊脚している．
		create2LegLifted(_current_node, _current_num, _output_graph);
	}
	else if (_touch_ground_leg_num == HexapodConst::LEG_NUM - 3)
	{
		// 3 本遊脚している．
		create3LegLifted(_current_node, _current_num, _output_graph);
	}
	else
	{
		//ここに来るのは接地している脚の数が6本 or 1本 or 2本．地面についている脚が3本を切ることはない，何故ならロボットが倒れてしまうため．
		//また6本接地しているならば脚を動かせない(遊脚する必要がある)．よって処理を行わない．(そのままの状態を次のノードにする．)
		SNode _new_node = _current_node;

		node_edit::changeNextNode(_new_node, _current_num, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．
		_output_graph.push_back(_new_node);		//追加する．
	}
}


void LegHierarchyNodeCreator::create1LegLifted(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//遊脚している脚を探す．遊脚数は1なので1つの数字が帰るはず
	std::vector<int> _lifted_leg;
	LegState::getLiftedLegNumWithVector(_current_node.leg_state, _lifted_leg);

	//脚状態 0001(1) から 0111(7)まで 次のパターンを生成する．なお上位bitは遊脚を表す．(0なら遊脚)
	for (int i = 1; i <= LegState::DISCRETE_NUM; i++)
	{
		SNode _new_node = _current_node;		//新しい脚状態を生成する.

		LegState::changeLegState(_new_node.leg_state, _lifted_leg.at(0), i);	//脚状態を変更する．

		node_edit::changeNextNode(_new_node, _current_num, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．

		_output_graph.push_back(_new_node);	//追加する．
	}
}

void LegHierarchyNodeCreator::create2LegLifted(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//遊脚している脚を探す．遊脚数は2なので2つの数字が帰るはず
	std::vector<int> _lifted_leg;
	LegState::getLiftedLegNumWithVector(_current_node.leg_state, _lifted_leg);

	//脚状態 0001(1) から 0111(7)まで 次のパターンを生成する．なお上位bitは遊脚を表す．(0なら遊脚)
	for (int i = 1; i <= LegState::DISCRETE_NUM; i++)
	{
		for (int j = 1; j <= LegState::DISCRETE_NUM; j++)
		{
			SNode _new_node = _current_node;		//新しい脚状態を生成する.

			LegState::changeLegState(_new_node.leg_state, _lifted_leg.at(0), i);			//脚状態を変更する．
			LegState::changeLegState(_new_node.leg_state, _lifted_leg.at(1), j);

			node_edit::changeNextNode(_new_node, _current_num, m_next_move);	//次のノード用に，深さ・親・次の動作を更新する．

			_output_graph.push_back(_new_node);	//追加する．
		}
	}
}

void LegHierarchyNodeCreator::create3LegLifted(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//遊脚している脚を探す．遊脚数は3なので3つの数字が帰るはず
	std::vector<int> _lifted_leg;
	LegState::getLiftedLegNumWithVector(_current_node.leg_state, _lifted_leg);

	//脚状態 0001(1) から 0111(7)まで 次のパターンを生成する．なお上位bitは遊脚を表す．(0なら遊脚)
	for (int i = 1; i <= LegState::DISCRETE_NUM; i++)
	{
		for (int j = 1; j <= LegState::DISCRETE_NUM; j++)
		{
			for (int k = 1; k <= LegState::DISCRETE_NUM; k++)
			{
				SNode _new_node = _current_node;		//新しい脚状態を生成する.

				LegState::changeLegState(_new_node.leg_state, _lifted_leg.at(0), i);	//脚状態を変更する．
				LegState::changeLegState(_new_node.leg_state, _lifted_leg.at(1), j);
				LegState::changeLegState(_new_node.leg_state, _lifted_leg.at(2), k);

				node_edit::changeNextNode(_new_node, _current_num, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．

				_output_graph.push_back(_new_node);	//追加する．
			}
		}
	}
}
