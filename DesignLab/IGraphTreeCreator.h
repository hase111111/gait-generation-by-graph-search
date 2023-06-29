#pragma once
#include "MapState.h"
#include "Node.h"
#include <vector>

class IGraphTreeCreator
{
public:

	IGraphTreeCreator() = default;
	virtual ~IGraphTreeCreator() = default;	

	//! @brief 木グラフを作成するクラス．完成したグラフは参照渡しする．
	//! @param _current_node [in] 現在の状態を表すノード
	//! @param _p_map [in] 現在のマップの状態
	//! @param _output_graph [out] 出力される木グラフ
	//! @return bool 生成に成功したならばtrue
	virtual bool createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph) = 0;

};


//! @file IGraphTreeCreator.h
//! @brief グラフ木の探索を行うクラスのインターフェイス．
//! @author 長谷川
 
//! @class IGraphTreeCreator
//! @brief グラフ木を作成するクラスのインターフェース．実体は作成できないのでこれを継承してたクラスを使うこと．
//! @details 継承をするクラスのデストラクタはvirtualにしておく．<br> 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html
