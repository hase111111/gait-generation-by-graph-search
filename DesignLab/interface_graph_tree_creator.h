//! @file interface_graph_tree_creator.h
//! @brief グラフ木の探索を行うクラスのインターフェイス．

#ifndef DESIGNLAB_INTERFACE_GRAPH_TREE_CREATOR_H_
#define DESIGNLAB_INTERFACE_GRAPH_TREE_CREATOR_H_


#include <vector>

#include "graph_search_result.h"
#include "node.h"
#include "map_state.h"


//! @class IGraphTreeCreator
//! @brief グラフ木を作成するクラスのインターフェース．実体は作成できないのでこれを継承してたクラスを使うこと．
//! @details 継承をするクラスのデストラクタはvirtualにしておく．
//! @n 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html
class IGraphTreeCreator
{
public:

	IGraphTreeCreator() = default;
	virtual ~IGraphTreeCreator() = default;

	//! @brief グラフ木を作成するクラスの初期化を行う．
	//! @param map_state [in] 分割されたマップ
	virtual void Init(const DevideMapState& map_state) = 0;

	//! @brief グラフ木を作成するクラス．完成したグラフは参照渡しする．
	//! @param current_node [in] 現在の状態を表すノード．深さが0のノードを渡すこと．
	//! @param max_depth [in] 作成するグラフ木の最大深さ．大きすぎる値を渡すと重くなる or 動かなくなるので注意．
	//! @param output_graph [out] 出力されるグラフ木
	//! @return EGraphSearchResult 生成の結果，生成に成功したかどうか，失敗した場合は失敗の理由を返す
	virtual EGraphSearchResult CreateGraphTree(const SNode& current_node, int max_depth, std::vector<SNode>* output_graph) = 0;
};


#endif