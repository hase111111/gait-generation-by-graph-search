#include "GraphTreeCreatorHato.h"
#include "Define.h"

bool GraphTreeCreatorHato::createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph)
{
	//マップのポインタを受け取る．
	mp_Map = _p_map;

	m_ComUpDown.init(mp_Map);
	m_LegUpDown.init(mp_Map);

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
				//深さが一つ下で，親が正しく設定されているもののみ追加する．少々冗長だが，安全のため．
				if (i.depth == (_output_graph.at(_cnt).depth + 1) && i.parent_num == _cnt)
				{
					_output_graph.push_back(i);		//子ノードを結果に追加する．
				}
			}
		}

		_cnt++;	//カウンタを進める．
	}

	return true;
}


void GraphTreeCreatorHato::makeNewNodesByCurrentNode(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	_output_graph.clear();

	switch (_current_node.next_move)
	{

	case EHexapodMove::LEG_UP_DOWN:
		//脚を上下移動させ，接地したり遊脚したりする．
		m_LegUpDown.create(_current_node, _current_num, _output_graph);
		break;


	case EHexapodMove::LEG_HIERARCHY_CHANGE:
		//脚の階層を変更する．LegStateを変更し，脚を平行移動する．
		m_LegHierarchy.create(_current_node, _current_num, _output_graph);
		break;


	case EHexapodMove::COM_MOVE:
		//重心を平行移動する．
		m_ComMove.create(_current_node, _current_num, _output_graph);
		break;


	case EHexapodMove::COM_UP_DOWN:
		//重心を上下移動させる．
		m_ComUpDown.create(_current_node, _current_num, _output_graph);
		break;

	default:

		//定義されていないならば，同じノードをそのまま追加する．
		SNode _new_node = _current_node;

		_new_node.changeNextNode(_current_num, _current_node.next_move);

		_output_graph.push_back(_new_node);
		break;
	}
}
