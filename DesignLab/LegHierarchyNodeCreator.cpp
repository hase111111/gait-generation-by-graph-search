#include "LegHierarchyNodeCreator.h"
#include "LegState.h"
#include "NodeEdit.h"

void LegHierarchyNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//現在，接地している脚の本数を数える
	const int _touch_ground_leg_num = LegState::getGroundedLegNum(_current_node.leg_state);

	//地面についている脚の本数によって処理をする
	if (_touch_ground_leg_num == 5)
	{
		//遊脚している脚を探す．遊脚数は1なので1つの数字が帰るはず
		std::vector<int> _lifted_leg;
		LegState::getLiftedLegNumWithVector(_current_node.leg_state, _lifted_leg);

		//可動範囲内の点を計算する処理は中身がなかったので全削除した．

		//脚状態 0001(1) から 0111(7)まで 次のパターンを生成する．なお上位bitは遊脚を表す．(0なら遊脚)
		for (int i = 1; i <= LegState::DISCRETE_NUM; i++)
		{
			////新しい脚状態を生成する
			int _new_leg_state = _current_node.leg_state;
			LegState::changeLegState(_new_leg_state, _lifted_leg.at(0), i);

			if (Define::FLAG_DO_PRUNING == true)
			{
				if (no_use_kaisou[LegState::getLegState(_new_leg_state, 0) - 1][LegState::getLegState(_new_leg_state, 1) - 1][LegState::getLegState(_new_leg_state, 2) - 1][LegState::getLegState(_new_leg_state, 3) - 1][LegState::getLegState(_new_leg_state, 4) - 1][LegState::getLegState(_new_leg_state, 5) - 1] == 0) continue;	/*int版*/
			}

			//結果を代入する．移動後の階層がどこなのかを記録する
			_res_transition_hierarchy.push_back(_new_leg_state);
		}
	}
	else if (_touch_ground_leg_num == 4)
	{
		//遊脚している脚を探す．遊脚数は2なので2つの数字が帰るはず
		std::vector<int> _lifted_leg;
		LegState::getLiftedLegNumWithVector(_current_node.leg_state, _lifted_leg);

		//可動範囲内の点を計算する処理は中身がなかったので全削除した．

		//脚状態 0001(1) から 0111(7)まで 次のパターンを生成する．なお上位bitは遊脚を表す．(0なら遊脚)
		for (int i = 1; i <= LegState::DISCRETE_NUM; i++)
		{
			for (int j = 1; j <= LegState::DISCRETE_NUM; j++)
			{
				//新しい脚状態を生成する
				int _new_leg_state = _current_node.leg_state;
				LegState::changeLegState(_new_leg_state, _lifted_leg.at(0), i);
				LegState::changeLegState(_new_leg_state, _lifted_leg.at(1), j);

				if (Define::FLAG_DO_PRUNING)
				{
					if (no_use_kaisou[LegState::getLegState(_new_leg_state, 0) - 1][LegState::getLegState(_new_leg_state, 1) - 1][LegState::getLegState(_new_leg_state, 2) - 1][LegState::getLegState(_new_leg_state, 3) - 1][LegState::getLegState(_new_leg_state, 4) - 1][LegState::getLegState(_new_leg_state, 5) - 1] == 0) continue;
				}

				//結果を代入する
				_res_transition_hierarchy.push_back(_new_leg_state);	// 移動後の階層がどこなのかを記録する
			}
		}
	}
	else if (_touch_ground_leg_num == 3)
	{
		//遊脚している脚を探す．遊脚数は3なので3つの数字が帰るはず
		std::vector<int> _lifted_leg;
		LegState::getLiftedLegNumWithVector(_current_node.leg_state, _lifted_leg);

		//可動範囲内の点を計算する処理は中身がなかったので全削除した．

		//脚状態 0001(1) から 0111(7)まで 次のパターンを生成する．なお上位bitは遊脚を表す．(0なら遊脚)
		for (int i = 1; i <= LegState::DISCRETE_NUM; i++)
		{
			for (int j = 1; j <= LegState::DISCRETE_NUM; j++)
			{
				for (int k = 1; k <= LegState::DISCRETE_NUM; k++)
				{
					//新しい脚状態を生成する
					int _new_leg_state = _current_node.leg_state;
					LegState::changeLegState(_new_leg_state, _lifted_leg.at(0), i);
					LegState::changeLegState(_new_leg_state, _lifted_leg.at(1), j);
					LegState::changeLegState(_new_leg_state, _lifted_leg.at(2), k);

					if (Define::FLAG_DO_PRUNING == true)
					{
						if (no_use_kaisou[LegState::getLegState(_new_leg_state, 0) - 1][LegState::getLegState(_new_leg_state, 1) - 1][LegState::getLegState(_new_leg_state, 2) - 1][LegState::getLegState(_new_leg_state, 3) - 1][LegState::getLegState(_new_leg_state, 4) - 1][LegState::getLegState(_new_leg_state, 5) - 1] == 0) continue;

					}

					//結果を代入する
					_res_transition_hierarchy.push_back(_new_leg_state);	// 移動後の階層がどこなのかを記録する
				}
			}
		}
	}
	else
	{
		//ここに来るのは接地している脚の数が6本 or 1本 or 2本．地面についている脚が3本を切ることはない，何故ならロボットが倒れてしまうため．
		//また6本接地しているならば脚を動かせない(遊脚する必要がある)．よって処理を行わない．(そのままの状態を次のノードにする．)
		SNode _new_node = _current_node;

		//次のノード用に，深さ・親・次の動作を更新する．
		_new_node.depth++;
		_new_node.parent_num = _current_num;
		_new_node.next_move = EHexapodMove::LEG_UP_DOWN;

		//追加する．
		_output_graph.push_back(_new_node);
	}
}
