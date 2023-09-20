//#pragma once
//
//#include "abstract_graph_searcher.h"
//
//
//
////! @class GraphSearcherSample
////! @date 2023/09/04
////! @author 長谷川
////! @brief GraphSearcherSampleクラスはテキトーに次の動作を選んで返す．あくまでこのクラスはIGraphSearcherの説明用．
////! @details AbstractGraphSearcher を継承したい場合．以下のように @n
////! @n
////!	class 好きな名前 final : public AbstractGraphSearcher @n
////!	{														@n
////!	}														@n
////! @n
////! と宣言すること．finalはもうこれ以上継承しないよという意味．public AbstractGraphSearcherはこのクラスを継承したよという意味．@n
////! AbstractGraphSearcherを継承したクラスに課せられる制約はただ一つ，AbstractGraphSearcherの純粋仮想関数，searchGraphTreeを実装すること．@n
////! 以下のようにsearchGraphTreeをpublicなところに宣言して，後ろにoverrideとつければよい．@n
//class GraphSearcherSample final : public AbstractGraphSearcher
//{
//public:
//	GraphSearcherSample(const std::shared_ptr<const AbstractHexapodStateCalculator>& calc) : AbstractGraphSearcher(calc) {}
//	~GraphSearcherSample() = default;
//
//	EGraphSearchResult searchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result) override;
//
//private:
//
//};
//
//
////! @file graph_searcher_sample.h
////! @date 2023/09/04
////! @author 長谷川
////! @brief GraphSearcherSampleクラスはテキトーに次の動作を選んで返す．あくまでこのクラスはIGraphSearcherの説明用．
////! @n 行数 : @lineinfo
