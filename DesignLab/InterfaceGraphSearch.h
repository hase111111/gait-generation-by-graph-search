#pragma once
#include "MapState.h"
#include "Node.h"
#include "Target.h"

class IGraphSearch
{
public:

	IGraphSearch() = default;
	virtual ~IGraphSearch() = default;

	//! @brief グラフ探索を行い，次の動作として最適なノードを返す．
	//! @param [in] _current_node 現在の状態を表すノード
	//! @param [in] _p_map 現在のマップの状態
	//!	@param [in] _target 目標の状態
	//! @param [out] _output_node 結果のノード
	//! @return bool グラフ探索に失敗した場合falseを返す
	virtual bool getNextNodebyGraphSearch(const SNode& _current_node, const MapState* const _p_map, const STarget& _target, SNode& _output_node) = 0;

	//! @brief 作成したグラフの数を返す
	//! @return int 作成したグラフの数
	int getMadeNodeNum() const { return m_made_node_num; }

protected:

	int m_made_node_num = 0;	//!< 作成したグラフの数
};

//! @file IGraphSearch
//! @brief グラフ探索を行うクラスのインターフェイスの実装
//! @author 長谷川
//! @date 2023/07/09

//! @class IGraphSearch
//! @brief グラフ探索を行うクラスのインターフェイス．実体は作成できないのでこれを継承してたクラスを使うこと．
//! @date 2023/07/09
//! @details 継承をするクラスのデストラクタはvirtualにしておく．<br>
//! 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html