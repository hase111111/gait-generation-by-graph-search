#include "NodeEdit.h"

void node_edit::changeParentNode(SNode& _node)
{
	_node.depth = 0;		//深さを0にする
	_node.parent_num = -1;	//自身が親のため，負の値を代入する．
}
