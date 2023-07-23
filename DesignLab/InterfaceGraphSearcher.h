#pragma once
#include <vector>
#include "Node.h"
#include "Target.h"
#include "GraphSearchResult.h"


class IGraphSearcher
{
public:
	IGraphSearcher() = default;
	virtual ~IGraphSearcher() = default;		//!< 継承をするクラスのデストラクタはvirtualにしておく．参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html

	//! @brief グラフを受け取り，その中から最適な次の動作を出力する．
	//! @param _graph [in] グラフ木
	//! @param _target [in] 目標地点
	//! @param _output_result [out] 出力されるノード
	//! @return EGraphSearchResult 探索の結果
	virtual EGraphSearchResult searchGraphTree(const std::vector<SNode>& _graph, const STarget& _target, SNode& _output_result) = 0;
};

//! @file InterfaceGraphSearcher.h
//! @brief グラフ木を作成するクラスのインターフェース．
//! @date 2023/07/23
//! @auther 長谷川

//! @class IGraphSearcher
//! @brief グラフ木を作成するクラスのインターフェース．実体は作成できないのでこれを継承してたクラスを使ってください．<br>
//! 継承の仕方や使い方が分からない場合は，GraphSearcherSampleを見てみてください．
//! @date 2023/07/23
//! @author 長谷川