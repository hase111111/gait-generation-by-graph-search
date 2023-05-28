#pragma once
#include "Node.h"


//ノードの状態を編集する関数をまとめたもの．
namespace node_edit 
{
	//渡された子ノードを親ノードに変更する．
	void changeParentNode(SNode& _node);

	//渡されたノードの深さを一つ深くして，親と次の動作を設定する．
	void changeNextNode(SNode& _node, const int _parent_num, const EHexapodMove _next_move);
}
