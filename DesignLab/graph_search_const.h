//! @file graph_search_const.h
//! @brief グラフ探索の定数をまとめたクラス

#ifndef DESIGNLAB_GRAPH_SEARCH_CONST_H_
#define DESIGNLAB_GRAPH_SEARCH_CONST_H_


//! @clas GraphSearchConst
//! @brief グラフ探索の定数をまとめたクラス
class GraphSearchConst final
{
public:

	//実体が生成できないようにコンストラクタをprivateにする
	GraphSearchConst() = default;
	GraphSearchConst(const GraphSearchConst&) = default;
	GraphSearchConst(GraphSearchConst&&) = default;
	GraphSearchConst& operator=(const GraphSearchConst&) = default;


	static const int kMaxDepth;	//!< グラフ探索の最大深さ

	static constexpr int kMaxNodeNum = 10000000;	//!< グラフ探索の最大ノード数

};


#endif	//DESIGNLAB_GRAPH_SEARCH_CONST_H_