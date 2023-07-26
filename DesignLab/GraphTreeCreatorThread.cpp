#include "GraphTreeCreatorThread.h"
#include "Define.h"

EGraphSearchResult GraphTreeCreatorThread::createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph, int& _make_node_num)
{
	//現在のノードを親にする．
	SNode _parent_node = _current_node;

	_parent_node.changeParentNode();
	_output_graph.clear();					//出力する結果を空にする．
	_output_graph.push_back(_parent_node);	//親を追加する．

	int _cnt = 0;	//カウンタを用意

	//カウンタがvectorのサイズを超えるまでループする．
	while (_cnt < _output_graph.size())
	{
		//探索深さが足りていないノードにのみ処理をする．
		if (_output_graph.at(_cnt).depth < Define::GRAPH_SEARCH_DEPTH)
		{
			std::vector<SNode> _res_vec;	// _cnt番目のノードの子ノードを入れるベクター

			makeNewNodesByCurrentNode(_output_graph.at(_cnt), _cnt, _res_vec);		//子ノードを生成する．

			for (const auto& i : _res_vec)
			{
				_output_graph.push_back(i);		//子ノードを結果に追加する．
			}
		}

		_cnt++;	//カウンタを進める．
	}

	return EGraphSearchResult::Success;
}

void GraphTreeCreatorThread::makeNewNodesByCurrentNode(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	_output_graph.clear();

	if (m_node_creator_map.count(_current_node.next_move) > 0)
	{
		m_node_creator_map.at(_current_node.next_move)->create(_current_node, _current_num, _output_graph);
		return;
	}
	else
	{
		//定義されていないならば，同じノードをそのまま追加する．
		SNode _new_node = _current_node;

		_new_node.changeNextNode(_current_num, _current_node.next_move);

		_output_graph.push_back(_new_node);
	}
}
