#pragma once


//! @clas GraphSearchConst
//! @date 2023/08/14
//! @author 長谷川
//! @brief グラフ探索の定数をまとめたクラス
class GraphSearchConst final
{
public:

	static constexpr int MAX_DEPTH = 5;	//!< グラフ探索の最大深さ

	static constexpr int MAX_NODE_NUM = 100000000;	//!< グラフ探索の最大ノード数

	static constexpr bool DO_DEBUG_PRINT = false;	//!< デバッグ用の出力をするかどうか

private:

	//実体が生成できないようにコンストラクタをprivateにする
	GraphSearchConst() = default;
	GraphSearchConst(const GraphSearchConst&) = default;
};


//! @file graph_search_const.h
//! @date 2023/08/14
//! @author 長谷川
//! @brief グラフ探索の定数をまとめたクラス
