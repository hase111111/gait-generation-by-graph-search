#include "GraphSearchInitializer.h"
#include "GraphTreeCreatorSample.h"
#include "GraphSearcherSample.h"


bool GraphSearchInitializer::init(std::unique_ptr<IGraphTreeCreator>& _p_tree_creator, std::unique_ptr<IGraphSearcher>& _p_graph_searcher)
{
	_p_tree_creator = std::make_unique<GraphTreeCreatorSample>();
	_p_graph_searcher = std::make_unique<GraphSearcherSample>();

	return true;
}


//このクラスの使い方について，
//２つの引数に対してstd::make_uniqur<クラス名>() を渡すことで，クラスを作成してあげることができます．
//例えば，GraphTreeCreatorSampleクラスと，GraphSearcherSampleクラスを作成したい場合は，
//
//	bool GraphSearchInitializer::init(std::unique_ptr<IGraphTreeCreator>& _p_tree_creator, std::unique_ptr<IGraphSearcher>& _p_graph_searcher)
//	{
//		_p_tree_creator = std::make_unique<GraphTreeCreatorSample>();
//		_p_graph_searcher = std::make_unique<GraphSearcherSample>();
//
//		return true;
//	}
//
// こんな感じで処理を書いてみてください．
// 
// 例えば地形の状態を判断して，生成する歩容を変更したり，グラフの評価手法を変更したい場合は，
// まず，setMap() 関数をこのクラスに実装して，GraphSearchクラスから地形データを取得してくる．
// 取得したデータを判断して，それによって make_uniqueするクラスを切り替えてあげればよい．
//
