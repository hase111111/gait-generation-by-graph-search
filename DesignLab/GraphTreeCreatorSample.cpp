#include "GraphTreeCreatorSample.h"
#include "Define.h"

bool GraphTreeCreatorSample::createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph)
{
	//ここにグラフを作成する処理を書く．このクラスはサンプルなので動作をしないノードだけを返します．

	//現在のノードを親にする．
	SNode _parent_node = _current_node;

	_parent_node.changeParentNode();
	_output_graph.push_back(_parent_node);

	//設定された探索深さまでの深さを持つグラフを作る．実際にグラフを作成する時もおそらくこんな感じでループする処理を書く．

	int _cnt = 0;	//カウンタを用意

	//カウンタがvectorのサイズを超えるまでループする．
	while (_cnt < _output_graph.size())
	{
		//探索深さが足りていないノードのみ，処理をする．
		if (_output_graph.at(_cnt).depth < Define::GRAPH_SEARCH_DEPTH)
		{
			SNode _new_node = _output_graph.at(_cnt);

			//ここに新しい姿勢を生成する処理を書く．今回は何の処理もせずに次のノードとする．

			_new_node.depth++;				//深さを一つ深くして，
			_new_node.parent_num = _cnt;	//親は現在処理しているvectorとなる．

			//追加する．
			_output_graph.push_back(_new_node);
		}

		_cnt++;	//カウンタを進める．
	}

	return true;
}
