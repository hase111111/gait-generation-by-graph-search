#pragma once

#include <deque>

#include "robot_state_node.h"


//! @class NodeValidityChecker
//! @author 長谷川
//! @date 2023/08/06
//! @brief 動作のループを感知するクラス
//! @details グラフ探索を用いてロボットの歩容生成をしていると，最善の行動をとると，同様の操作をし続けて動作がループしてしまう場合がある．
//! @n それを探知して，歩容生成に失敗していることを通達するクラス．
//! @n [deque(デック)について] @n std::vectorの亜種．@n vectorとの違いとして，先頭と末尾の要素の追加・削除が高速である．
//! @n 参考 : https://cpprefjp.github.io/reference/deque/deque.html
class NodeValidityChecker final
{
public:
	NodeValidityChecker() = default;
	~NodeValidityChecker() = default;

	//! ロボットが行った動作をセットする．
	//! @param [in] _node ロボットの現在の状態
	void setNode(const RobotStateNode& node);

	//!直近に行った動作を比較して，ロボットが正しく動作できているか判断します．
	//! @return bool 動作がループしている，つまり正しく動作していない時にtrueを返す．
	bool isLoopMove() const;

private:
	std::deque<RobotStateNode> m_node;		//歩容生成した物を保持する．

	const int kMaxDataNum = 20;		//全てのデータを保持し続けると非常に重たくなるので，個の変数の値の数までデータを持つ．先輩のプログラムでは30個程持つようにしていたので，そのぐらいの数にしとく 
};


//! @file node_validity_checker.h
//! @author 長谷川
//! @date 2023/08/06
//! @brief 動作のループを感知するクラス．
//! @n 行数 : @lineinfo