#include "node_validity_checker.h"


void NodeValidityChecker::SetNode(const RobotStateNode& node)
{
	//ノードを前から追加する
	node_.push_front(node);

	//最大保持数を超えているならばその分だけ，後ろから削除する．
	while (node_.size() > kMaxDataNum) { node_.pop_back(); }
}


bool NodeValidityChecker::IsLoopMove() const
{
	//全ての要素と，一番新しい要素を比較して，同様の動作があればループしているとみなす．

	if (node_.size() < 1) { return false; }	//比較するためのノードがないならば即終了．

	auto itr = node_.begin();		//ノードの最初を指すイテレーターを取得して，
	itr++;							//一つ進める．

	for (size_t i = 0; i < 10; i++)
	{
		if (itr != node_.end())itr++;
	}

	//イテレーターが最後になるまでループする．
	for (itr; itr != node_.end(); itr++)
	{
		//同じノードがあれば，動作がループしているとみなし，trueを返す．
		if (node_.front() == (*itr)) { return true; }
	}

	return false;
}
