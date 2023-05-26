#include "NodeValidityChecker.h"

void NodeValidityChecker::setNode(const SNode& _node)
{
	//ノードを前から追加する
	m_node.push_front(_node);

	//最大保持数を超えているならばその分だけ，後ろから削除する．
	while (m_node.size() > MAX_DATA_NUM) { m_node.pop_back(); }
}

bool NodeValidityChecker::isLoopMove() const
{
	//全ての要素と，一番新しい要素を比較して，同様の動作があればループしているとみなす．

	if (m_node.size() < 1) { return false; }	//比較するためのノードがないならば即終了．

	auto _itr = m_node.begin();		//ノードの最初を指すイテレーターを取得して，
	_itr++;							//一つ進める．

	//イテレーターが最後になるまでループする．
	for (_itr; _itr != m_node.end(); _itr++)
	{
		//同じノードがあれば，動作がループしているとみなし，trueを返す．
		if (isNodeEqual(m_node.front(), (*_itr)) == true) { return true; }
	}

	return false;
}
