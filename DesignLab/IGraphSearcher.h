#pragma once
#include <vector>
#include "Node.h"
#include "Target.h"

//グラフ木を作成するクラスのインターフェース．実体は作成できないのでこれを継承してたクラスを使ってください．
//継承の仕方や使い方が分からない場合は，GraphSearcherSampleを見てみてください．
class IGraphSearcher
{
public:
	IGraphSearcher() = default;
	virtual ~IGraphSearcher() = default;		//継承をするクラスのデストラクタはvirtualにしておく．参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html

	//グラフを受け取り，その中から最適な次の動作を出力する．
	virtual bool searchGraphTree(const std::vector<SNode>& _graph, const STarget& _target, SNode& _output_result) = 0;

};
