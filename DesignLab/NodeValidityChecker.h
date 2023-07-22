#pragma once
#include "Node.h"
#include <deque>


class NodeValidityChecker final
{
public:
	NodeValidityChecker() = default;
	~NodeValidityChecker() = default;

	//! ロボットが行った動作をセットする．
	//! @param [in] _node ロボットの現在の状態
	void setNode(const SNode& _node);

	//!直近に行った動作を比較して，ロボットが正しく動作できているか判断します．
	//! @return bool 動作がループしている，つまり正しく動作していない時にtrueを返す．
	bool isLoopMove() const;

private:
	std::deque<SNode> m_node;		//歩容生成した物を保持する．
	const int MAX_DATA_NUM = 20;	//全てのデータを保持し続けると非常に重たくなるので，個の変数の値の数までデータを持つ．先輩のプログラムでは30個程持つようにしていたので，そのぐらいの数にしとく 
};


//! @file NodeValidityChecker.h
//! @brief 動作のループを感知するクラスの実装．
//! @author 長谷川

//! @class NodeValidityChecker
//! @brief 動作のループを感知するクラス
//! @details グラフ探索を用いてロボットの歩容生成をしていると，最善の行動をとると，同様の操作をし続けて動作がループしてしまう場合がある．<br>
//! それを探知して，歩容生成に失敗していることを通達するクラス．
//! @author 長谷川
