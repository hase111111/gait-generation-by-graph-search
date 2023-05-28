#include "NodeEdit.h"

void node_edit::changeParentNode(SNode& _node)
{
	_node.depth = 0;		//深さを0にする
	_node.parent_num = -1;	//自身が親のため，負の値を代入する．
}

void node_edit::changeNextNode(SNode& _node, const int _parent_num, const EHexapodMove _next_move)
{
	_node.depth += 1;
	_node.parent_num = _parent_num;
	_node.next_move = _next_move;
}
