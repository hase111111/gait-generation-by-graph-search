#include "GraphSearcherHato.h"
#include "Define.h"

EGraphSearchResult GraphSearcherHato::searchGraphTree(const std::vector<SNode>& _graph, const STarget& _target, SNode& _output_result)
{
	// _targetの値によって，探索方法を変える必要がある．探索方法を抽象化するべき．

	// @todo initializerで初期化する処理を書く

	// ターゲットモードが直進と仮定して処理を書いている

	int _result_index = -1;
	float _max_move_dif = _target.TargetPosition.lengthSquare();

	const size_t _graph_size = _graph.size();

	for (size_t i = 0; i < _graph_size; i++)
	{
		if (_result_index < 0) { _result_index = i; }

		//最大深さのノードのみを評価する
		if (_graph.at(i).depth == Define::GRAPH_SEARCH_DEPTH)
		{
			my_vec::SVector2 _move_dif = _target.TargetPosition.projectedXY() - _graph.at(i).global_center_of_mass.projectedXY();

			if (_max_move_dif > _move_dif.lengthSquare())
			{
				_max_move_dif = _move_dif.lengthSquare();
				_result_index = i;
			}
		}
	}

	// index が範囲外ならば失敗
	if (_result_index < 0 || _result_index >= _graph_size) { return EGraphSearchResult::Failure; }

	//深さ1まで遡って値を返す
	while (_graph.at(_result_index).depth != 1)
	{
		_result_index = _graph.at(_result_index).parent_num;
	}

	_output_result = _graph.at(_result_index);

	return EGraphSearchResult::Success;
}
