#pragma once
#include "listFunc.h"


//ノードの状態を編集する関数をまとめたもの．
namespace node_edit 
{
	//渡された子ノードを親ノードに変更する．
	void changeParentNode(SNode& _node);

	void changeNextNode(SNode& _node);
}
