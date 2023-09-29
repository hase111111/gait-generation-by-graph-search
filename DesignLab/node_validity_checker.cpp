#include "node_validity_checker.h"


void NodeValidityChecker::setNode(const RobotStateNode& node)
{
	//ノードを前から追加する
	m_node.push_front(node);

	//最大保持数を超えているならばその分だけ，後ろから削除する．
	while (m_node.size() > kMaxDataNum) { m_node.pop_back(); }
}


bool NodeValidityChecker::isLoopMove() const
{
	//全ての要素と，一番新しい要素を比較して，同様の動作があればループしているとみなす．

	if (m_node.size() < 1) { return false; }	//比較するためのノードがないならば即終了．

	auto itr = m_node.begin();		//ノードの最初を指すイテレーターを取得して，
	itr++;							//一つ進める．

	for (size_t i = 0; i < 10; i++)
	{
		if (itr != m_node.end())itr++;
	}

	//イテレーターが最後になるまでループする．
	for (itr; itr != m_node.end(); itr++)
	{
		//同じノードがあれば，動作がループしているとみなし，trueを返す．
		if (m_node.front() == (*itr)) { return true; }
	}

	return false;
}
