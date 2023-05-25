#pragma once
#include "MapState.h"
#include "listFunc.h"
#include "IGraphTreeCreator.h"
#include <memory>

//グラフ探索を行うクラス．名前通り
class GraphSearch final
{
public:
	GraphSearch() = default;
	~GraphSearch() = default;

	//グラフ探索を行い，次の動作として最適なノードを返す．結果は output_node で参照渡しし，戻り値はグラフ探索に成功したかどうかを返す(falseなら失敗．)
	// _current_node　…　現在の状態を表すノード． _map　…　マップの状態へのポインタ．コピーで渡すと重たいのでポインタを渡す．
	//       _target　…　動作の目標を設定したもの．
	bool getNextNodebyGraphSearch(const SNode& _current_node, const MapState* const _p_map, const STarget& _target, SNode& _output_node);

private:
	std::unique_ptr<IGraphTreeCreator> mp_GraphTreeCreator;
};
