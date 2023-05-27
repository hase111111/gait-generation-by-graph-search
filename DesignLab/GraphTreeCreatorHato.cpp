#include "GraphTreeCreatorHato.h"
#include "NodeEdit.h"
#include "Define.h"

bool GraphTreeCreatorHato::createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph)
{
	//マップのポインタを受け取る．
	mp_Map = _p_map;

	//現在のノードを親にする．
	SNode _parent_node = _current_node;

	node_edit::changeParentNode(_parent_node);
	_output_graph.clear();					//出力する結果を空にする．
	_output_graph.push_back(_parent_node);	//親を追加する．

	int _cnt = 0;	//カウンタを用意

	//カウンタがvectorのサイズを超えるまでループする．
	while (_cnt < _output_graph.size())
	{
		//探索深さが足りていないノードにのみ処理をする．
		if (_output_graph.at(_cnt).depth < Define::GRAPH_SEARCH_DEPTH)
		{
			//現在のノードから次の動作を生成して，後ろにどんどん追加する．
			pushNewNodesByCurrentNode(_output_graph.at(_cnt), _cnt, _output_graph);
		}

		_cnt++;	//カウンタを進める．
	}

	return true;
}


void GraphTreeCreatorHato::pushNewNodesByCurrentNode(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	switch (_current_node.next_move)
	{
	case EHexapodMove::LEG_UP_DOWN:

		//脚を上下移動させ，接地したり遊脚したりする．
		break;

	case EHexapodMove::LEG_HIERARCHY_CHANGE:

		//脚の階層を変更する．LegStateを変更し，脚を平行移動する．
		m_LegHierarchy.create(_current_node, _current_num, _output_graph);
		break;

	case EHexapodMove::COM_TRANSLATION:

		//重心を平行移動する．
		break;

	case EHexapodMove::COM_UP_DOWN:

		m_ComUpDown.create(_current_node, _current_num, _output_graph);
		break;

	default:

		//定義されていないならば，同じノードをそのまま追加する．
		SNode _new_node = _current_node;

		node_edit::changeNextNode(_new_node, _current_num, _current_node.next_move);

		_output_graph.push_back(_new_node);
		break;
	}
}
