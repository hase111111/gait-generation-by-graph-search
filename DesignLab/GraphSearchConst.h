#pragma once

class GraphSearchConst final
{
public:

	static constexpr int MAX_DEPTH = 4;	//!< グラフ探索の最大深さ

	static constexpr int MAX_NODE_NUM = 100000000;	//!< グラフ探索の最大ノード数

	static constexpr bool DO_DEBUG_PRINT = true;	//!< デバッグ用の出力をするかどうか

private:

	//実体が生成できないようにコンストラクタをprivateにする
	GraphSearchConst() = default;
	GraphSearchConst(const GraphSearchConst&) = default;
};
