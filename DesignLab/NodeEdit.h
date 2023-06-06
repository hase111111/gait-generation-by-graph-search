#pragma once
#include "Node.h"
#include <string>

//ノードの状態を編集する関数をまとめたもの．
namespace node_edit 
{
	// ノードを初期化する関数．変更するノードを参照渡しで受け取り，第2引数でランダムに変更するかどうか決める．
	void initNode(SNode& _node, const bool _do_random);

	//渡された子ノードを親ノードに変更する．引数_nodeのdepthを0に，parent_numを-1に初期化する．
	void changeParentNode(SNode& _node);

	//渡されたノードの深さを一つ深くして，親と次の動作を設定する．
	void changeNextNode(SNode& _node, const int _parent_num, const EHexapodMove _next_move);

	std::string getTextHexapodMove(const EHexapodMove _move);
}
