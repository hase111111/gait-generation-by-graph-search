#pragma once
#include "IGraphTreeCreator.h"
#include "MapState.h"
#include "ComMoveNodeCreator.h"
#include "ComUpDownNodeCreator.h"
#include "LegHierarchyNodeCreator.h"
#include "LegUpDownNodeCreator.h"

class GraphTreeCreatorHato final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorHato() = default;
	~GraphTreeCreatorHato() = default;

	//! @brief グラフを作成する
	//! @param _current_node 現在のノード
	//! @param _p_map マップの状態
	//! @param _output_graph 作成したグラフを格納する
	//! @return グラフの作成に成功したかどうか
	//! @details この関数は，_current_nodeの子ノードを生成して，_output_graphに代入する．<br>
	//! _output_graphの値をリセットしてから代入する．<br>
	//! この関数は，_current_nodeの子ノードを生成するために，makeNewNodesByCurrentNode関数を呼び出す．<br>
	bool createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph) override;

private:

	//_out_put_graphの値をリセットしてから，_current_nodeの子ノードを生成して，_output_graphに代入する．
	void makeNewNodesByCurrentNode(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

	const MapState * mp_Map;

	LegHierarchyNodeCreator m_LegHierarchy;
	LegUpDownNodeCreator m_LegUpDown;
	ComUpDownNodeCreator m_ComUpDown;
	ComMoveNodeCreator m_ComMove;
};


//! @file GraphTreeCreatorHato.h 
//! @brief 波東さんのグラフを作成するプログラムを移植したクラスの実装

//! @class GraphTreeCreatorHato
//! @brief 波東さんのグラフを作成するプログラムを移植したもの
//! @details もともとのプログラムで行われた処理の中でコメントアウトされていたものはすべて削除したので，知りたければ過去のプログラムを参照すること．